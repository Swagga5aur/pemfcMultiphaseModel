    // Mapping anode bipolarPlate subdomain.  Memory management

    // Code changed and adapted to pemfc (J.P.Kone, 13.06.2016)
    // abpMesh: anode side bipolarPlate mesh
    // BP: bipolarplate

    {

        scalarField kF
        (
            abpMesh.nCells(),
            kAnodeBP.value()
        );

        // Perform reverse mapping
        kCellIn.rmap(kF, abpCellMap);
    }
