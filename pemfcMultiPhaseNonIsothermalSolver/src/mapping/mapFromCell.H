// Code changed and adapted to pemfc multiphase flow model (J.P.Kone, 08.02.2017)
// for coversion to openfoam-4.0
// call to 'internalField()' is changed to 'ref()'

{
    // anode bipolar plate
    {
        forAll (TanodeBP, cellI)
        {
            TanodeBP[cellI] = Tcell[abpCellMap[cellI]];
        }
        TanodeBP.correctBoundaryConditions();
    }

    // anode fluid
    {
        forAll (TanodeFluid, cellI)
        {
            TanodeFluid[cellI] = Tcell[anodeFluidCellMap[cellI]];
        }
        TanodeFluid.correctBoundaryConditions();
    }

    // electrolyte
    {
        forAll (Telectrolyte, cellI)
        {
            Telectrolyte[cellI] = Tcell[electrolyteCellMap[cellI]];
        }
        Telectrolyte.correctBoundaryConditions();
    }

    // cathode fluid
    {
        forAll (TcathodeFluid, cellI)
        {
            TcathodeFluid[cellI] = Tcell[cathodeFluidCellMap[cellI]];
        }
        TcathodeFluid.correctBoundaryConditions();
    }


    // cathode bipolar plate
    {
        forAll (TcathodeBP, cellI)
        {
            TcathodeBP[cellI] = Tcell[cbpCellMap[cellI]];
        }
        TcathodeBP.correctBoundaryConditions();
    }


    Info << "TanodeFluid min mean max = "
         << Foam::gMin(TanodeFluid.ref())     << "    "
         << Foam::gAverage(TanodeFluid.ref()) << "    "
         << Foam::gMax(TanodeFluid.ref())
         << endl;

    Info << "TcathodeFluid min mean max  = "
         << Foam::gMin(TcathodeFluid.ref())     << "    "
         << Foam::gAverage(TcathodeFluid.ref()) << "    "
         << Foam::gMax(TcathodeFluid.ref())
         << endl;
}

