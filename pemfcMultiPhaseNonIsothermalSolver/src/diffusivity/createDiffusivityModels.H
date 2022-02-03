// Code changed and adapted to pemfc (J.P.Kone, 11.07.2016)

// for coversion to openfoam-4.0
// type 'porousZones' is changed to 'myIOporosityModelList'
// all functions of class porousZone are replaced with
// equivalent funcions from new class porosityModel
// 'forAll' loop is changed to 'for' to avoid any ambiguity


    // Fuel side
    // ---------
    Info << nl << "Creating fuel diffusivity models" << endl;

    // scalarField passed to diff model to return calculated diffusivity
    // --------------------------------
    scalarField fuelDiff(anodeFluidMesh.nCells());

    // pointerlist for all fuel diffusivity models
    // ------------------------------------------
    PtrList<autoPtr<diffusivityModels::diffusivityModel> >
        fuelDiffModels(1 + anodeZones.UPtrList<myPorosityModel>::size());


    // Diffusivity: whole anodeFluid zone
    // ----------------------------
    label anodeFluidZnId = anodeFluidMesh.cellZones().findZoneID("anodeFluid");
    Info<< "zone " << anodeFluidZnId << " fuel" << nl;
    const labelList& fuelCells = anodeFluidMesh.cellZones()[anodeFluidZnId];
    const dictionary& fuelDiffDict = anodeFluidProperties.subDict("diffusivity");

    fuelDiffModels.set
    (
        0,
        new autoPtr<diffusivityModels::diffusivityModel>
        (
            diffusivityModels::diffusivityModel::New
                (anodeFluidMesh, fuelDiff, fuelCells, fuelDiffDict)
        )
    );


    // anodeFluid porous zones
    // -----------------
    label nzFuel = anodeZones.UPtrList<myPorosityModel>::size();

    // initialize pointer lists
    PtrList<const labelList> fuelZnCells(nzFuel);
    PtrList<const dictionary> fuelZnDiffDict(nzFuel);
    PtrList<autoPtr<diffusivityModels::diffusivityModel> >
        fuelZnDiffusivity(nzFuel);

    for(label iz=0;iz<anodeZones.UPtrList<myPorosityModel>::size();iz++)
    {
        label znId =
            anodeFluidMesh.cellZones().findZoneID(anodeZones.UPtrList<myPorosityModel>::operator[](iz).name());

        Info<< "anodeZone " << iz << ", anodeFluidZone " << znId
            << ": " << anodeZones.UPtrList<myPorosityModel>::operator[](iz).name() << endl;
        Info<< "size = " << anodeFluidMesh.cellZones()[znId].size() << nl;

        fuelZnCells.set
        (
            iz,
            new labelList(anodeFluidMesh.cellZones()[znId])
        );

        fuelZnDiffDict.set
        (
            iz,
            new dictionary(anodeZones.UPtrList<myPorosityModel>::operator[](iz).dict().subDict("diffusivity"))
        );

        fuelDiffModels.set
        (
            iz + 1,
            new autoPtr<diffusivityModels::diffusivityModel>
            (
                diffusivityModels::diffusivityModel::New
                    (anodeFluidMesh, fuelDiff, fuelZnCells[iz], fuelZnDiffDict[iz])
            )
        );
    }


    // cathodeFluid side
    // --------
        Info << nl << "Creating air diffusivity models" << endl;

        // scalarField passed to diff model to return calculated diffusivity
        // --------------------------------
        scalarField airDiff(cathodeFluidMesh.nCells());

        // pointerlist for all air diffusivity models
        // ------------------------------------------
        PtrList<autoPtr<diffusivityModels::diffusivityModel> >
        airDiffModels(1 + cathodeZones.UPtrList<myPorosityModel>::size());


        // Diffusivity: whole cathodeFluid zone
        // ---------------------------
        label cathodeFluidZnId = cathodeFluidMesh.cellZones().findZoneID("cathodeFluid");
        Info<< "zone " << cathodeFluidZnId << " air" << nl;
        const labelList& airCells = cathodeFluidMesh.cellZones()[cathodeFluidZnId];
        const dictionary& airDiffDict = cathodeFluidProperties.subDict("diffusivity");

        airDiffModels.set
        (
            0,
            new autoPtr<diffusivityModels::diffusivityModel>
            (
                diffusivityModels::diffusivityModel::New
                    (cathodeFluidMesh, airDiff, airCells, airDiffDict)
            )
        );

        // cathodeFluid porous zones
        // ----------------
        label nzAir = cathodeZones.UPtrList<myPorosityModel>::size();

        // initialize pointer lists
        PtrList<const labelList> airZnCells(nzAir);
        PtrList<const dictionary> airZnDiffDict(nzAir);
        PtrList<autoPtr<diffusivityModels::diffusivityModel> >
            airZnDiffusivity(nzAir);

        for(label iz=0;iz<cathodeZones.UPtrList<myPorosityModel>::size();iz++)
        {
            label znId =
                cathodeFluidMesh.cellZones().findZoneID(cathodeZones.UPtrList<myPorosityModel>::operator[](iz).name());

            Info<< "cathodeZone " << iz << ", cathodeFluidZone " << znId
                << ": " << cathodeZones.UPtrList<myPorosityModel>::operator[](iz).name() << endl;
            Info<< "size = " << cathodeFluidMesh.cellZones()[znId].size() << nl;

            airZnCells.set
            (
                iz,
                new labelList(cathodeFluidMesh.cellZones()[znId])
            );

            airZnDiffDict.set
            (
                iz,
                new dictionary(cathodeZones.UPtrList<myPorosityModel>::operator[](iz).dict().subDict("diffusivity"))
            );

            airDiffModels.set
            (
                iz + 1,
                new autoPtr<diffusivityModels::diffusivityModel>
                (
                    diffusivityModels::diffusivityModel::New
                        (cathodeFluidMesh, airDiff, airZnCells[iz], airZnDiffDict[iz])
                )
            );
        }
