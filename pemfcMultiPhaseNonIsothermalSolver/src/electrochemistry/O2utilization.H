// Code changed and adapted to pemfc electrochemistry (J.P.Kone, 20.02.2017)

// for coversion to openfoam-4.0
// call to 'boundaryField()' is changed to 'boundaryFieldRef()' for all volScalarFields

{
    // total air mass rate at inlet
    scalarField  rateInlet =
    (
        rhoAir.boundaryFieldRef()[cathodeFluidInletID]
        *(
             Uair.boundaryFieldRef()[cathodeFluidInletID]
             &
             cathodeFluidMesh.Sf().boundaryField()[cathodeFluidInletID]
         )
    );

    // total air mass rate at outlet
    scalarField  rateOutlet =
    (
        rhoAir.boundaryFieldRef()[cathodeFluidOutletID]
        *(
             Uair.boundaryFieldRef()[cathodeFluidOutletID]
             &
             cathodeFluidMesh.Sf().boundaryField()[cathodeFluidOutletID]
         )
    );

    // total air mass rate at cathode/electrolyte interface
    scalarField  rateCathode =
    (
        rhoAir.boundaryFieldRef()[cathodeID]
        *(
             Uair.boundaryFieldRef()[cathodeID]
             &
             cathodeFluidMesh.Sf().boundaryField()[cathodeID]
         )
    );

    Info<< "air mass rates: " << nl
        << "   inlet: " << Foam::mag(Foam::gSum(rateInlet))
        << "   outlet: " << Foam::mag(Foam::gSum(rateOutlet))
        << "   cathode: " << Foam::mag(Foam::gSum(rateCathode))
        << endl;

    // --------------------------------------------------------------------
    // Foam::mag added (J.P.Kone, 27.05.2016)

    forAll(airSpecies, I)
    {
        if(airSpecies[I].rSign() < 0)
        {
            scalar rateInletI = Foam::mag
            (
                gSum
                (
                    rateInlet*Yair[I].boundaryFieldRef()[cathodeFluidInletID]
                )
            );

            scalar rateOutletI = Foam::mag
            (
                gSum
                (
                    rateOutlet*Yair[I].boundaryFieldRef()[cathodeFluidOutletID]
                )
            );

            scalar rateElecI = Foam::mag
            (
                gSum
                (
                    mflux[I]*cathodeFluidMesh.magSf().boundaryField()[cathodeID]
                )
            );

            Info<< airSpecies[I].name() << " mass rates: " << nl;
            Info<< "   inlet = " << rateInletI
                << "   outlet = " << rateOutletI
                << "   chem = " << rateElecI
                << endl;

            Info<< "      utilization by chemRate = "
                << rateElecI/rateInletI
                << nl;
            Info<< "      utilization by outRate  = "
                << (rateInletI - rateOutletI)/rateInletI
                << endl;
        }
    }
}
