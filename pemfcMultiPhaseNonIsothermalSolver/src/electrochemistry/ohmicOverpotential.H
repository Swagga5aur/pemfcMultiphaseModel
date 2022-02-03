//    Calculating Ohmic overpotential (J.P.Kone, 08.11.2016)

scalarField etaOhm(cathodePatch.size());
{
    // Compute Ohmic overpotential
    etaOhm = 
    (
        I*R
    );

    Info<< "min,mean,max (etaOhmic) = " << gMin(etaOhm) << "  "
        << gAverage(etaOhm) << "  " << gMax(etaOhm) << nl;
}

//  Save data for output

scalarField& etaOhmicIn = etaOhmic.ref();
smearPatchToMesh smearetaOhmic(electrolyteMesh, electrolyteCathodeName);
etaOhmicIn = smearetaOhmic.smear(etaOhm);
etaOhmic.correctBoundaryConditions();
