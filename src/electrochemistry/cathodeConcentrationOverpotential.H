//    Calculating cathode concentration overpotential (J.P.Kone, 01.11.2016)
//    Reference: O'Hayre (2006) p.148

scalarField etaConcentrationCathode(cathodePatch.size());
{
    // Compute cathodic concentration overpotential
    //forAll(airSpecies, s)
    //{
        //if (airSpecies[s].rSign() == -1)
        //{
            etaConcentrationCathode = 
            (
                //((Rgas*cathodeT)
                ///(airSpecies[s].ne()*F))
                //*(1.0+(1.0/(alphaCathodeElectrolyte.value())))
                etaConCaConstant.value()
                *Foam::log(iLCathode/(iLCathode - I))
            );
        //}
    //}

    Info<< "min,mean,max (etaConCa) = " << gMin(etaConcentrationCathode) << "  "
        << gAverage(etaConcentrationCathode) << "  " << gMax(etaConcentrationCathode) << nl;
}

//  Save data for output

scalarField& etaConCIn = etaConC.ref();
smearPatchToMesh smearetaConC(electrolyteMesh, electrolyteCathodeName);
etaConCIn = smearetaConC.smear(etaConcentrationCathode);
etaConC.correctBoundaryConditions();
etaConC.boundaryFieldRef()[electrolyteCathodeID] == etaConcentrationCathode;
