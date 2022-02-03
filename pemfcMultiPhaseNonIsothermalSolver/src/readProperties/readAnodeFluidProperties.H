    // Code changed and adapted to pemfc multiphase flow model (J.P.Kone, 13.02.2017)

    Info<< "Reading anode fluid properties\n" << endl;

    IOdictionary anodeFluidProperties
    (
        IOobject
        (
            "anodeFluidProperties",
            runTime.constant(),
            anodeFluidMesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );

    // Creating the phase model
    
    // fuel
    autoPtr<phaseModel> fuel = phaseModel::New(anodeFluidMesh,anodeFluidProperties,"fuel");
    dimensionedScalar rhoF = fuel->rho();
    dimensionedScalar muF = fuel->mu();
    dimensionedScalar CpF = fuel->Cp();
    dimensionedScalar kF = fuel->k();

    // anodeLiquidWater
    autoPtr<phaseModel> anodeLiquidWater = phaseModel::New(anodeFluidMesh,anodeFluidProperties,"anodeLiquidWater");
    dimensionedScalar rhoAlw = anodeLiquidWater->rho();
    dimensionedScalar muAlw = anodeLiquidWater->mu();
    dimensionedScalar CpAlw = anodeLiquidWater->Cp();
    dimensionedScalar kAlw = anodeLiquidWater->k();

    // hydraulic diameter for Reynolds number calculation
    dimensionedScalar dHydFuel(anodeFluidProperties.lookup("dHyd"));

    // echo input to output:
    Info << "    rhoF      = " << rhoF     << nl
         << "    muF       = " << muF      << nl
         << "    CpF       = " << CpF      << nl
         << "    kF        = " << kF       << nl
         << "    dHydFuel  = " << dHydFuel << nl
         << "    rhoAlw    = " << rhoAlw   << nl
         << "    muAlw     = " << muAlw    << nl
         << "    CpAlw     = " << CpAlw    << nl
         << "    kAlw      = " << kAlw     << nl
         << endl;

    bool ReF(true);
    if(dHydFuel.value() == 0)
    {
        ReF = false;
        Info << "Hydraulic diameter = 0: not calculating ReFuel" << nl << endl;
    }
