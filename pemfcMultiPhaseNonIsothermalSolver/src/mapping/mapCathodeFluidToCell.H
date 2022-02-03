    // Mapping air subdomain.  Memory management

    // Code changed and adapted to pemfc multiphase flow model (J.P.Kone, 01.06.2017)
    // for coversion to openfoam-4.0
    // call to 'internalField()' is changed to 'ref()'
    // call to 'boundaryField()' is changed to 'boundaryFieldRef()'
    // all functions of class porousZone are replaced with
    // equivalent funcions from new class porosityModel
    // 'forAll' loop is changed to 'for' to avoid any ambiguity

    {
        // Create fields from air side

        // mass based heat capacity cp
        // cp [J/kg-K] = Cp/M where Cp = [J/mol-K], M = [kg/mol]

        // air
        scalarField& cpairIn = cpAir.ref(); 
        cpairIn = 0;
        forAll(airSpecies, i)
        {
            scalar rMWi = 1.0/(airSpecies[i].MW()*1e-3);    // 1/[kg/mol]
            scalarField cpairIni = molarCpAir[i].polyVal(TcathodeFluid.ref())*rMWi;
            cpairIn += Yair[i].ref()*cpairIni;
        }
        // for liquid water
        scalarField& cpcathodeLiquidWaterIn = cpCathodeLiquidWater.ref();
        // porous zones
        for(label iz=0;iz<cathodeZones.UPtrList<myPorosityModel>::size();iz++)
        {
            label znId =
                cathodeFluidMesh.cellZones().findZoneID(cathodeZones.UPtrList<myPorosityModel>::operator[](iz).name());

            scalar porZn = cathodeZones.UPtrList<myPorosityModel>::operator[](iz).porosity();

            labelList znCells(cathodeFluidMesh.cellZones()[znId]);

            forAll(znCells, cellI)
            {

                cpairIn[znCells[cellI]] *= porZn;
                cpcathodeLiquidWaterIn[znCells[cellI]] *= porZn;
            }
        }
        cpAir.correctBoundaryConditions();

        scalarField& rhoCathodeFluidIn = rhoCathodeFluid.ref();
        rhoCathodeFluidIn =
        (
            ((1.0 - ScathodeLiquidWater.ref())
          * rhoAir.ref())
          + (ScathodeLiquidWater.ref()
          * rhoCathodeLiquidWater.ref())
        );
        rhoCathodeFluid.correctBoundaryConditions();

        scalarField& cpCathodeFluidIn = cpCathodeFluid.ref();
        cpCathodeFluidIn =
        (
            ((1.0 - ScathodeLiquidWater.ref())
          * cpAir.ref())
          + (ScathodeLiquidWater.ref()
          * cpCathodeLiquidWater.ref())
        );
        cpCathodeFluid.correctBoundaryConditions();

        scalarField rhoCpCathodeFluid =
        (
            rhoCathodeFluid.ref()
          * cpCathodeFluid.ref()
        ); 

        Info<< "min mean max rhoCpCathodeFluid = " << gMin(rhoCpCathodeFluid) << " "
            << gAverage(rhoCpCathodeFluid) << " " << gMax(rhoCpCathodeFluid) << nl;
        Info << endl;

        // Perform reverse mapping
        rhoCpCellIn.rmap(rhoCpCathodeFluid, cathodeFluidCellMap);

        // thermal conductivity
        // for air
        scalarField& kairIn = kAir.ref();
        // for liquid water
        scalarField& kcathodeLiquidWaterIn = kCathodeLiquidWater.ref();
        // porous zones
        for(label iz=0;iz<cathodeZones.UPtrList<myPorosityModel>::size();iz++)
        {
            label znId =
                cathodeFluidMesh.cellZones().findZoneID(cathodeZones.UPtrList<myPorosityModel>::operator[](iz).name());

            dimensionedScalar kairZn(cathodeZones.UPtrList<myPorosityModel>::operator[](iz).dict().lookup("k"));
            scalar porZn = cathodeZones.UPtrList<myPorosityModel>::operator[](iz).porosity();

            labelList znCells(cathodeFluidMesh.cellZones()[znId]);

            forAll(znCells, cellI)
            {                   
                kairIn[znCells[cellI]] *= porZn;
                kairIn[znCells[cellI]] += (1 - porZn)*kairZn.value();
                kcathodeLiquidWaterIn[znCells[cellI]] *= porZn;
            }
        }

        scalarField kCathodeFluid = 
        (
            ((1.0 - ScathodeLiquidWater.ref())*kairIn)
          + (ScathodeLiquidWater.ref()*kcathodeLiquidWaterIn)
        );

        // Perform reverse mapping
        kCellIn.rmap(kCathodeFluid, cathodeFluidCellMap);

        // add cathode phase change enthalpy source to cathodeFluid
        scalarField TsourceCellCathodeFluid = cathodePhaseChangeEnthalpySource.ref();
        // Perform reverse mapping
        TsourceCellIn.rmap(TsourceCellCathodeFluid, cathodeFluidCellMap);

        // Map cathode fluid fluxes
        labelList cathodeFluidInternalFaceMap
        (
            SubList<label>(cathodeFluidFaceMap, cathodeFluidMesh.nInternalFaces())
        );

        scalarField cathodeFluidInternalFaceMask
        (
            scalarField::subField(cathodeFluidFaceMask, cathodeFluidMesh.nInternalFaces())
        );

        //
        // ** recall phi already incorporates rho **
        //

        scalarField rhoCpPhiA =
        (
            linearInterpolate(cpCathodeFluid)
          * phiAir.ref()
        );

        phiCellIn.rmap
        (
            phiAir.ref()*cathodeFluidInternalFaceMask,
            cathodeFluidInternalFaceMap
        );

        rhoCpPhiCellIn.rmap
        (
            rhoCpPhiA*cathodeFluidInternalFaceMask,
            cathodeFluidInternalFaceMap
        );

        // Do electrolyte interface
        scalarField cathodePhi = phiAir.boundaryFieldRef()[cathodeID];
        forAll(cathodePhi, faceI)
        {
            phiCell[cathodeMask[faceI]] = cathodePhi[faceI];
            rhoCpPhiCell[cathodeMask[faceI]] =
            (
                cpCathodeFluid[faceI]*cathodePhi[faceI]
            );
        }

        // Do flux boundary conditions
        forAll (cathodeFluidPatchesMap, patchI)
        {
            // Map only real matching patches.  HJ, 22/May/2011
            if
            (
                cathodeFluidPatchesMap[patchI] > -1
             && cathodeFluidPatchesMap[patchI] < mesh.boundary().size()
            )
            {
                // Patch maps
                labelField curFpm
                (
                    labelField::subField
                    (
                        cathodeFluidFaceMap,
                        cathodeFluidMesh.boundary()[patchI].size(),
                        cathodeFluidMesh.boundary()[patchI].patch().start()
                    )
                );

                scalarField curMask
                (
                    scalarField::subField
                    (
                        cathodeFluidFaceMask,
                        cathodeFluidMesh.boundary()[patchI].size(),
                        cathodeFluidMesh.boundary()[patchI].patch().start()
                    )
                );

                curFpm -= mesh.boundary()
                    [cathodeFluidPatchesMap[patchI]].patch().start();

                
                phiCell.boundaryFieldRef()[cathodeFluidPatchesMap[patchI]].
                    scalarField::rmap
                    (
                        phiAir.boundaryFieldRef()[patchI]*curMask,
                        curFpm
                    );
                
                rhoCpPhiCell.boundaryFieldRef()[cathodeFluidPatchesMap[patchI]].
                    scalarField::rmap
                    (
                        (
                            cpCathodeFluid.boundaryFieldRef()[patchI]
                            *phiAir.boundaryFieldRef()[patchI]
                        )*curMask,
                        curFpm
                    );
            }
        }
    }
