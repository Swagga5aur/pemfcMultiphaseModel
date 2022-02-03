//    Calculating cathode activation overpotential (J.P.Kone, 01.11.2016)

scalarField etaActivationCathode(cathodePatch.size());
{
    // Compute cathodic overpotential
    etaActivationCathode = 
    (
        ((Rgas*cathodeT)
        /(alphaCathodeElectrolyte.value()*F))
        *Foam::log(I/i0Cathode)
    );

    Info<< "min,mean,max (etaActCa) = " << gMin(etaActivationCathode) << "  "
        << gAverage(etaActivationCathode) << "  " << gMax(etaActivationCathode) << nl;
}

//  Save data for output

scalarField& etaAtcCIn = etaActC.ref();
smearPatchToMesh smearetaActC(electrolyteMesh, electrolyteCathodeName);
etaAtcCIn = smearetaActC.smear(etaActivationCathode);
etaActC.correctBoundaryConditions();
etaActC.boundaryFieldRef()[electrolyteCathodeID] == etaActivationCathode;
