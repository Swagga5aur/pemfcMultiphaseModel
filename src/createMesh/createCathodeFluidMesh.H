    // Code changed and adapted to pemfc multiphase flow model (J.P.Kone, 08.02.2017)

    fvMesh cathodeFluidMesh
    (
        IOobject
        (
            "cathodeFluid",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ
        )
    );

    // Read maps to global mesh

    // Note: faceRegionAddressing contains an offset by one and
    // sign management for a face flip.  airFaceMap and airFaceMask
    // are created to hold addressing and a (-1, 1) pre-multiplier
    // to manage the face flip.  HJ, 22/May/2011

    labelIOList cathodeFluidFaceRegionAddressing
    (
        IOobject
        (
            "faceRegionAddressing",
            cathodeFluidMesh.time().findInstance(cathodeFluidMesh.meshDir(), "faces"),
            polyMesh::meshSubDir,
            cathodeFluidMesh,
            IOobject::MUST_READ
        )
    );

    labelList cathodeFluidFaceMap(cathodeFluidFaceRegionAddressing.size());
    scalarField cathodeFluidFaceMask(cathodeFluidFaceRegionAddressing.size());

    forAll(cathodeFluidFaceMap, i)
    {
        cathodeFluidFaceMap[i] = mag(cathodeFluidFaceRegionAddressing[i]) - 1;
        cathodeFluidFaceMask[i] = sign(cathodeFluidFaceRegionAddressing[i]);
    }

    labelIOList cathodeFluidCellMap
    (
        IOobject
        (
            "cellRegionAddressing",
            cathodeFluidMesh.time().findInstance(cathodeFluidMesh.meshDir(), "faces"),
            polyMesh::meshSubDir,
            cathodeFluidMesh,
            IOobject::MUST_READ
        )
    );

    labelIOList cathodeFluidPatchesMap
    (
        IOobject
        (
            "boundaryRegionAddressing",
            cathodeFluidMesh.time().findInstance(cathodeFluidMesh.meshDir(), "faces"),
            polyMesh::meshSubDir,
            cathodeFluidMesh,
            IOobject::MUST_READ
        )
    );
