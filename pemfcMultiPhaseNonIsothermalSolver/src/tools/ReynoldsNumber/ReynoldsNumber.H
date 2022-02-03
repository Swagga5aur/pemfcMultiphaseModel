// Code changed and adapted to pemfc (J.P.Kone, 23.09.2016)
// for coversion to openfoam-4.0
// call to 'boundaryField()' is changed to 'boundaryFieldRef()'

// Reynolds number @ fuelInlet

if(ReF)
{
    scalar ReFuel= dHydFuel.value()*Foam::mag
    (
        Foam::gAverage
        (
            rhoFuel.boundaryFieldRef()[anodeFluidInletID]
            *Ufuel.boundaryFieldRef()[anodeFluidInletID]
            /muFuel.boundaryFieldRef()[anodeFluidInletID]
        )
    );

    Info<< nl << "ReFuel = "<<ReFuel<<endl;
}


// Reynolds number @ airInlet

if(ReA)
{
    scalar ReAir= dHydAir.value()*Foam::mag
    (
        Foam::gAverage
        (
            rhoAir.boundaryFieldRef()[cathodeFluidInletID]
            *Uair.boundaryFieldRef()[cathodeFluidInletID]
            /muAir.boundaryFieldRef()[cathodeFluidInletID]
        )
    );

    Info<< "ReAir  = "<<ReAir<<endl;
}
