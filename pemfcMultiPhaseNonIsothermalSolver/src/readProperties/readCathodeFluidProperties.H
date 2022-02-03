    // Code changed and adapted to pemfc multiphase flow model (J.P.Kone, 13.02.2017)

    Info<< "Reading cathode fluid properties\n" << endl;

    IOdictionary cathodeFluidProperties
    (
        IOobject
        (
            "cathodeFluidProperties",
            runTime.constant(),
            cathodeFluidMesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );

    // phase model creation
    
    // air
    autoPtr<phaseModel> air = phaseModel::New(cathodeFluidMesh,cathodeFluidProperties,"air");
    dimensionedScalar rhoA = air->rho();
    dimensionedScalar muA = air->mu();
    dimensionedScalar CpA = air->Cp();
    dimensionedScalar kA = air->k();

    // cathodeLiquidWater
    autoPtr<phaseModel> cathodeLiquidWater = phaseModel::New(cathodeFluidMesh,cathodeFluidProperties,"cathodeLiquidWater");
    dimensionedScalar rhoClw = cathodeLiquidWater->rho();
    dimensionedScalar muClw = cathodeLiquidWater->mu();
    dimensionedScalar CpClw = cathodeLiquidWater->Cp();
    dimensionedScalar kClw = cathodeLiquidWater->k();

    // hydraulic diameter for Reynolds number calculation
    dimensionedScalar dHydAir(cathodeFluidProperties.lookup("dHyd"));

    // echo input to output:
    Info << "    muA      = " << muA     << nl
         << "    rhoA     = " << rhoA    << nl
         << "    CpA      = " << CpA     << nl
         << "    kA       = " << kA      << nl
         << "    dHydAir  = " << dHydAir << nl
         << "    rhoClw   = " << rhoClw  << nl
         << "    muClw    = " << muClw   << nl
         << "    CpClw    = " << CpClw   << nl
         << "    kClw     = " << kClw    << nl
         << endl;

    bool ReA(true);
    if(dHydAir.value() == 0)
    {
        ReA = false;
        Info << "Hydraulic diameter = 0: not calculating ReAir" << nl << endl;
    } 
