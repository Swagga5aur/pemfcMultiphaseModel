    // Mapping cathode bipolarPlate subdomain.  Memory management

    // Code changed and adapted to pemfc (J.P.Kone, 13.06.2016)
    // cbpMesh: cathode side bipolarPlate mesh
    // BP: bipolarplate

    {
        scalarField kF
        (
            cbpMesh.nCells(),
            kCathodeBP.value()
        );

        // Perform reverse mapping
        kCellIn.rmap(kF, cbpCellMap);
    }
