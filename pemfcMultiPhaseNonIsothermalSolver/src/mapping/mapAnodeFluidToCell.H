    // Mapping fuel subdomain. Memory management

    // Code changed and adapted to pemfc multiphase flow model (J.P.Kone, 01.06.2017)
    // for coversion to openfoam-4.0
    // call to 'internalField()' is changed to 'ref()'
    // call to 'boundaryField()' is changed to 'boundaryFieldRef()'
    // all functions of class porousZone are replaced with
    // equivalent funcions from new class porosityModel
    // 'forAll' loop is changed to 'for' to avoid any ambiguity

    {
        // Create fields from anodeFluid side

        // mass based heat capacity cp
        // cp [J/kg-K] = Cp/M where Cp = [J/mol-K], M = [kg/mol]
        
        // for fuel
        scalarField& cpfuelIn = cpFuel.ref();
        cpfuelIn = 0;
        forAll(fuelSpecies, i)
        {
            // molarCp[i].polyVal() returns [J/(mol K)]
            // species[i].MW() returns {kg/kmol]
            // we require cp [J/(kg K] = [J/(mol K)]/[kg/mol]

            scalar rMWi = 1.0/(fuelSpecies[i].MW()*1e-3);    // 1/[kg/mol]
            scalarField cpfuelIni =
            (
                 molarCpFuel[i].polyVal(TanodeFluid.ref())*rMWi
            );
            cpfuelIn += Yfuel[i].ref()*cpfuelIni;       
        }
        // for liquid water
        scalarField& cpanodeLiquidWaterIn = cpAnodeLiquidWater.ref();
        // porous zones
        for(label iz=0;iz<anodeZones.UPtrList<myPorosityModel>::size();iz++)
        {
            label znId =
                anodeFluidMesh.cellZones().findZoneID(anodeZones.UPtrList<myPorosityModel>::operator[](iz).name());

            scalar porZn = anodeZones.UPtrList<myPorosityModel>::operator[](iz).porosity();

            labelList znCells(anodeFluidMesh.cellZones()[znId]);

            forAll(znCells, cellI)
            {

                cpfuelIn[znCells[cellI]] *= porZn;
                cpanodeLiquidWaterIn[znCells[cellI]] *= porZn;
            }
        }
        cpFuel.correctBoundaryConditions();

        scalarField& rhoAnodeFluidIn = rhoAnodeFluid.ref();
        rhoAnodeFluidIn =
        (
            ((1.0 - SanodeLiquidWater.ref())
          * rhoFuel.ref())
          + (SanodeLiquidWater.ref()
          * rhoAnodeLiquidWater.ref())
        );
        rhoAnodeFluid.correctBoundaryConditions();

        scalarField& cpAnodeFluidIn = cpAnodeFluid.ref();
        cpAnodeFluidIn =
        (
            ((1.0 - SanodeLiquidWater.ref())
          * cpFuel.ref())
          + (SanodeLiquidWater.ref()
          * cpAnodeLiquidWater.ref())
        );
        cpAnodeFluid.correctBoundaryConditions();

        scalarField rhoCpAnodeFluid =
        (
            rhoAnodeFluid.ref()
          * cpAnodeFluid.ref()
        ); 

        Info<< "min mean max rhoCpAnodeFluid = " << gMin(rhoCpAnodeFluid) << " "
            << gAverage(rhoCpAnodeFluid) << " " << gMax(rhoCpAnodeFluid) << nl;
        Info << endl;

        // Perform reverse mapping
        rhoCpCellIn.rmap(rhoCpAnodeFluid, anodeFluidCellMap);

        // thermal conductivity
        // for fuel
        scalarField& kfuelIn = kFuel.ref();
        // for liquid water
        scalarField& kanodeLiquidWaterIn = kAnodeLiquidWater.ref();
        // porous zones
        for(label iz=0;iz<anodeZones.UPtrList<myPorosityModel>::size();iz++)
        {
            label znId =
                anodeFluidMesh.cellZones().findZoneID(anodeZones.UPtrList<myPorosityModel>::operator[](iz).name());

            dimensionedScalar kfuelZn(anodeZones.UPtrList<myPorosityModel>::operator[](iz).dict().lookup("k"));
            scalar porZn = anodeZones.UPtrList<myPorosityModel>::operator[](iz).porosity();

            labelList znCells(anodeFluidMesh.cellZones()[znId]);

            forAll(znCells, cellI)
            {
                kfuelIn[znCells[cellI]] *= porZn;
                kfuelIn[znCells[cellI]] += (1 - porZn)*kfuelZn.value();
                kanodeLiquidWaterIn[znCells[cellI]] *= porZn;
            }
        }
        
        scalarField kAnodeFluid = 
        (
            ((1.0 - SanodeLiquidWater.ref())*kfuelIn)
          + (SanodeLiquidWater.ref()*kanodeLiquidWaterIn)
        );

        // Perform reverse mapping
        kCellIn.rmap(kAnodeFluid, anodeFluidCellMap);

        // add anode phase change enthalpy source to anodeFluid
        scalarField TsourceCellAnodeFluid = anodePhaseChangeEnthalpySource.ref();

        // Perform reverse mapping
        TsourceCellIn.rmap(TsourceCellAnodeFluid, anodeFluidCellMap);

        // Map anode fluid fluxes
        labelList anodeFluidInternalFaceMap
        (
            SubList<label>(anodeFluidFaceMap, anodeFluidMesh.nInternalFaces())
        );

        scalarField anodeFluidInternalFaceMask
        (
            scalarField::subField(anodeFluidFaceMask, anodeFluidMesh.nInternalFaces())
        );

        //
        // ** recall: phi already incorporates rho **
        //
        scalarField rhoCpPhiF =
        (
            linearInterpolate(cpAnodeFluid)
          * phiFuel.ref()
        );

        phiCellIn.rmap
        (
            phiFuel.ref()*anodeFluidInternalFaceMask,
            anodeFluidInternalFaceMap
        );

        rhoCpPhiCellIn.rmap
        (
            rhoCpPhiF*anodeFluidInternalFaceMask,
            anodeFluidInternalFaceMap
        );

        // Do electrolyte interface
        scalarField anodePhi = phiFuel.boundaryFieldRef()[anodeID];
        forAll(anodePhi, faceI)
        {
            phiCell[anodeMask[faceI]] = anodePhi[faceI];
            rhoCpPhiCell[anodeMask[faceI]] =
            (
                cpAnodeFluid[faceI]*anodePhi[faceI]
            );
        }

        // Do flux boundary conditions
        forAll (anodeFluidPatchesMap, patchI)
        {
            // Map only real matching patches.  HJ, 22/May/2011
            if
            (
                anodeFluidPatchesMap[patchI] > -1
             && anodeFluidPatchesMap[patchI] < mesh.boundary().size()
            )
            {
                // Patch maps
                labelField curFpm
                (
                    labelField::subField
                    (
                        anodeFluidFaceMap,
                        anodeFluidMesh.boundary()[patchI].size(),
                        anodeFluidMesh.boundary()[patchI].patch().start()
                    )
                );

                scalarField curMask
                (
                    scalarField::subField
                    (
                        anodeFluidFaceMask,
                        anodeFluidMesh.boundary()[patchI].size(),
                        anodeFluidMesh.boundary()[patchI].patch().start()
                    )
                );

                curFpm -= mesh.boundary()
                    [anodeFluidPatchesMap[patchI]].patch().start();

                phiCell.boundaryFieldRef()[anodeFluidPatchesMap[patchI]].
                    scalarField::rmap
                    (
                        phiFuel.boundaryFieldRef()[patchI]*curMask,
                        curFpm
                    );

                rhoCpPhiCell.boundaryFieldRef()[anodeFluidPatchesMap[patchI]].
                    scalarField::rmap
                    (
                        (
                            cpAnodeFluid.boundaryFieldRef()[patchI]
                            *phiFuel.boundaryFieldRef()[patchI]
                        )*curMask,
                        curFpm
                    );
            }
        }
    }
