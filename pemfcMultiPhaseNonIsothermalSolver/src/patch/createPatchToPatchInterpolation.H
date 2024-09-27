    // Code changed and adapted to pemfc multiphase flow model (J.P.Kone, 08.02.2017)

    // Electrochemistry will be solved on the cathode

    const polyPatch& anodePatch = anodeFluidMesh.boundaryMesh()[anodeID];
    const polyPatch& cathodePatch = cathodeFluidMesh.boundaryMesh()[cathodeID];
    const polyPatch& electrolyteAnodePatch = electrolyteMesh.boundaryMesh()[electrolyteAnodeID];
    const polyPatch& electrolyteCathodePatch = electrolyteMesh.boundaryMesh()[electrolyteCathodeID];

    mypatchToPatchInterpolation anodeToCathode
    (
        anodePatch,
        cathodePatch
    );

    mypatchToPatchInterpolation cathodeToAnode
    (
        cathodePatch,
        anodePatch
    );

    // Create a mask from global faces to the anode surface on the fuel mesh
    labelList anodeMask(anodePatch.size());

    forAll (anodeMask, i)
    {
        anodeMask[i] = anodeFluidFaceMap[anodePatch.start() + i];
    }

    // Create a mask from global faces to the cathode surface on the air mesh
    labelList cathodeMask(cathodePatch.size());

    forAll (cathodeMask, i)
    {
        cathodeMask[i] = cathodeFluidFaceMap[cathodePatch.start() + i];
    }

    // Create a mask from global faces to the anode surface on the electrolyte mesh
    labelList electrolyteAnodeMask(electrolyteAnodePatch.size());

    forAll (electrolyteAnodeMask, i)
    {
        electrolyteAnodeMask[i] = electrolyteFaceMap[electrolyteAnodePatch.start() + i];
    }

    // Create a mask from global faces to the cathode surface on the electrolyte mesh
    labelList electrolyteCathodeMask(electrolyteCathodePatch.size());

    forAll (electrolyteCathodeMask, i)
    {
        electrolyteCathodeMask[i] = electrolyteFaceMap[electrolyteCathodePatch.start() + i];
    }


    // Ohmic heating will occur in the electrolyte
    // Interpolation is no longer required (R.Nishida, 01.04.2014)

    mypatchToPatchInterpolation anodeFluidAnodeToElectrolyteAnode
    (
        anodeFluidMesh.boundaryMesh()[anodeID],
        electrolyteMesh.boundaryMesh()[electrolyteAnodeID]
    );

    mypatchToPatchInterpolation cathodeFluidCathodeToElectrolyteCathode
    (
        cathodeFluidMesh.boundaryMesh()[cathodeID],
        electrolyteMesh.boundaryMesh()[electrolyteCathodeID]
    );
