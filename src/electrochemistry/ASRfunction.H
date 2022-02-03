// Area Specific Resistance function (J.P.Kone, 17.10.2016)

scalarField R(cathodePatch.size()); //ohm*m^2

if (fuelCellType == "PEMFC")
{
    // membrane ionic conductivity
    #include "sigmaElectrolyte.H"

    // ionic resistance (in the membrane)
    scalarField RiMem(electrolyteCathodePatch.size());
    forAll (RiMem, facei)
    {
        functionElectrolyteResistance sigmaElectrolyteIntegral(sigmaElectrolyte[facei]);
        RiMem[facei] = functionIntegration<functionElectrolyteResistance>(sigmaElectrolyteIntegral).integral(0., tMem.value(), 20);
    }
    
    Info<< "min,mean,max (RiMem) = " 
        << gMin(RiMem) << "  "
        << gAverage(RiMem) << "  " 
        << gMax(RiMem) << nl;
        //<< sumRiMem << nl;

    // electronic resistance (in the gas diffusion layer)
    scalar ReGdl = 2.0*tGdl.value()*(1.0/(kappaGdl.value()*epsilonGdl.value()));

    // electronic resistance (in the catalyst layer)
    scalar ReCl = 2.0*tCl.value()*(1.0/(kappaCl.value()*epsilonCl.value()));

    // contact resistance (between gas diffusion layer and bipolar plate) 
    scalar Rc = 2*rContact.value();

    // electronic resistance (in the bipolar plate)
    scalar ReBp = 2.0*tBp.value()*(1.0/(kappaBp.value()));

    // total cell internal resistance
    R = RiMem + ReGdl + ReCl + Rc +ReBp;
}
else 
{
    FatalError
	<< "not available fuel cell type! (available==>PEMFC)" <<nl
	<< exit (FatalError);
}

Info << "min,mean,max (ASR)= "
     << gMin(R) <<" "
     << gAverage(R) << " "
     << gMax(R) << nl;
