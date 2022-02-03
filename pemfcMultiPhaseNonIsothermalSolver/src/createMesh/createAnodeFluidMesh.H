    // Code changed and adapted to pemfc multiphase flow model (J.P.Kone, 08.02.2017)

    fvMesh anodeFluidMesh
    (
        IOobject
        (
            "anodeFluid",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ
        )
    );

    // Read maps to global mesh

    labelIOList anodeFluidFaceRegionAddressing
    (
        IOobject
        (
            "faceRegionAddressing",
            anodeFluidMesh.time().findInstance(anodeFluidMesh.meshDir(), "faces"),
            polyMesh::meshSubDir,
            anodeFluidMesh,
            IOobject::MUST_READ
        )
    );

    labelList anodeFluidFaceMap(anodeFluidFaceRegionAddressing.size());
    scalarField anodeFluidFaceMask(anodeFluidFaceRegionAddressing.size());

    forAll(anodeFluidFaceMap, i)
    {
        anodeFluidFaceMap[i] = mag(anodeFluidFaceRegionAddressing[i]) - 1;
        anodeFluidFaceMask[i] = sign(anodeFluidFaceRegionAddressing[i]);
    }

    labelIOList anodeFluidCellMap
    (
        IOobject
        (
            "cellRegionAddressing",
            anodeFluidMesh.time().findInstance(anodeFluidMesh.meshDir(), "faces"),
            polyMesh::meshSubDir,
            anodeFluidMesh,
            IOobject::MUST_READ
        )
    );

    labelIOList anodeFluidPatchesMap
    (
        IOobject
        (
            "boundaryRegionAddressing",
            anodeFluidMesh.time().findInstance(anodeFluidMesh.meshDir(), "faces"),
            polyMesh::meshSubDir,
            anodeFluidMesh,
            IOobject::MUST_READ
        )
    );
