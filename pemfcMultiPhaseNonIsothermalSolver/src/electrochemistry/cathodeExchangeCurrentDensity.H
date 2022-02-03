// calculating cathode exchange current density (J.P.Kone, 01.11.2016)

//    For cathode (air)
//    i0Cathode = i0,ref(353.15)*exp[-EAct/(R)*(1/T-1/353.15)]
//    where
//    io(353.15) = reference exchange current density at 353.15K
//    EAct = Activation energy
//    R = gas constant
//    T = cathode temperature
//

scalarField i0Cathode(cathodePatch.size());
{
    // Compute local cathode exchange current density
    i0Cathode = 
    (
        i0CathodeElectrolyteReference.value()
        *Foam::exp(((-1.*ECathodeElectrolyte.value())/(Rgas))
        *((1.0/cathodeT)-(1.0/353.15)))
    );

    Info<< "min,mean,max (cathodeT) = " << gMin(cathodeT) << "  "
        << gAverage(cathodeT) << "  " << gMax(cathodeT) << nl;
    Info<< "min,mean,max (i0Ca) = " << gMin(i0Cathode) << "  "
        << gAverage(i0Cathode) << "  " << gMax(i0Cathode) << nl;
}

//  Save data for output (J.P.Kone, 05.07.2017)
scalarField& i0CIn = i0C.ref();
smearPatchToMesh smeari0C(electrolyteMesh, electrolyteCathodeName);
i0CIn = smeari0C.smear(i0Cathode);
i0C.correctBoundaryConditions();
i0C.boundaryFieldRef()[electrolyteCathodeID] == i0Cathode;
