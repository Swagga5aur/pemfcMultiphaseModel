// Code changed and adapted to pemfc electrochemistry (J.P.Kone, 20.02.2017)

// for coversion to openfoam-4.0
// call to 'boundaryField()' is changed to 'boundaryFieldRef()' for all volScalarFields

{
    // total mass rate at fuel inlet
    scalarField  rateInlet =
    (
        rhoFuel.boundaryFieldRef()[anodeFluidInletID]
        *(
             Ufuel.boundaryFieldRef()[anodeFluidInletID]
             &
             anodeFluidMesh.Sf().boundaryField()[anodeFluidInletID]
         )
    );
    
    // total mass rate at fuel outlet
    scalarField  rateOutlet =
    (
        rhoFuel.boundaryFieldRef()[anodeFluidOutletID]
        *(
             Ufuel.boundaryFieldRef()[anodeFluidOutletID]
             &
             anodeFluidMesh.Sf().boundaryField()[anodeFluidOutletID]
         )
    );
    
    // total mass rate at anode/electrolyte interface
    scalarField  rateAnode =
    (
        rhoFuel.boundaryFieldRef()[anodeID]
        *(
             Ufuel.boundaryFieldRef()[anodeID]
             &
             anodeFluidMesh.Sf().boundaryField()[anodeID]
         )
    );

    Info<< "fuel mass rates: " << nl
        << "   inlet: " << Foam::mag(Foam::gSum(rateInlet))
        << "   outlet: " << Foam::mag(Foam::gSum(rateOutlet))
        << "   anode: " << Foam::mag(Foam::gSum(rateAnode))
        << endl;
    
    // --------------------------------------------------------------------
    // cathodeToAnode interpolation of mass fluxes added (J.P.Kone, 27.05.2016)
    
    forAll(fuelSpecies, I)
    {
        if(fuelSpecies[I].rSign() < 0)
        {
            scalar rateInletI = Foam::gSum
            (
                rateInlet*Yfuel[I].boundaryFieldRef()[anodeFluidInletID]
            );

            scalar rateOutletI = Foam::gSum
            (
                rateOutlet*Yfuel[I].boundaryFieldRef()[anodeFluidOutletID]
            );

            scalar rateElecI = Foam::gSum
            (
            	cathodeToAnode.faceInterpolate(mflux[I])
                *anodeFluidMesh.magSf().boundaryField()[anodeID]
            );

            Info<< fuelSpecies[I].name() << " mass rates: " << nl;
            Info<< "   inlet = " << Foam::mag(rateInletI)
                << "   outlet = " << Foam::mag(rateOutletI)
                << "   chem = " << Foam::mag(rateElecI)
                << endl;

            Info<< "      utilization by chemRate = "
                << Foam::mag(rateElecI/rateInletI)
                << nl
                << "      utilization by outRate  = "
                << 1.0 - Foam::mag(rateOutletI/rateInletI)
                << endl;
        }
    }
}
