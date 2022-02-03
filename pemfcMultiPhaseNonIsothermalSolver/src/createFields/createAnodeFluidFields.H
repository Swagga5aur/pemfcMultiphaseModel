// Code changed and adapted to pemfc multiphase flow model (J.P.Kone, 17.05.2017)
// for coversion to openfoam-4.0
// type 'porousZones' is changed to 'myIOporosityModelList'
// all functions of class porousZone are replaced with
// equivalent funcions from new class porosityModel
// 'forAll' loop is changed to 'for' to avoid any ambiguity

    // creating fuel fields

    // pressure
    Info<< "Creating fuel field p" << endl;
    volScalarField pFuel
    (
        IOobject
        (
            "p",
            runTime.timeName(),
            anodeFluidMesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        anodeFluidMesh
    );

    // density
    Info<< "Creating fuel field rho" << endl;
    volScalarField rhoFuel
    (
        IOobject
        (
            "rho",
            runTime.timeName(),
            anodeFluidMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        anodeFluidMesh,
        rhoF,
        zeroGradientFvPatchScalarField::typeName
    );
    rhoFuel.correctBoundaryConditions();

    // dynamic viscosity
    Info<< "Creating fuel field mu" << endl;
    volScalarField muFuel
    (
        IOobject
        (
            "mu",
            runTime.timeName(),
            anodeFluidMesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        anodeFluidMesh,
        muF,
        zeroGradientFvPatchScalarField::typeName
    );
    muFuel.correctBoundaryConditions();

    // velocity
    Info<< "Creating fuel field U" << endl;
    volVectorField Ufuel
    (
        IOobject
        (
            "U",
            runTime.timeName(),
            anodeFluidMesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        anodeFluidMesh
    );

    // face flux
    Info<< "Creating fuel face flux field phi" << endl;
    surfaceScalarField phiFuel
    (
        IOobject
        (
            "phifuel",
            runTime.timeName(),
            anodeFluidMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        linearInterpolate(rhoFuel*Ufuel) & anodeFluidMesh.Sf()
    );

    // inverse matrix diagonal field rUA
    volScalarField rUAfuel
    (
        IOobject
        (
            "(1|A(U))",
            runTime.timeName(),
            anodeFluidMesh
        ),
        anodeFluidMesh,
        dimensionedScalar("rUA", dimTime/dimDensity, 1),
        zeroGradientFvPatchScalarField::typeName
    );

    // Pressure reference point in fuel
    label pFuelRefCell = 0;
    scalar pFuelRefValue = 0.0;

    setRefCell
    (
        pFuel,
        anodeFluidMesh.solutionDict().subDict("PISO"),
        pFuelRefCell,
        pFuelRefValue
    );

    // specific heat capacity
    Info<< "Creating fuel field cp" << endl;
    volScalarField cpFuel
    (
        IOobject
        (
            "cpfuel",
            runTime.timeName(),
            anodeFluidMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        anodeFluidMesh,
        CpF,
        zeroGradientFvPatchScalarField::typeName
    );
    cpFuel.correctBoundaryConditions();
    
    // thermal conductivity
    Info<< "Creating fuel field k" << endl;
    volScalarField kFuel
    (
        IOobject
        (
            "kfuel",
            runTime.timeName(),
            anodeFluidMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        anodeFluidMesh,
        kF,
        zeroGradientFvPatchScalarField::typeName
    );
    kFuel.correctBoundaryConditions();

    /*----------------------------------------------------------------------- */

    // creating anode liquid water fields

    // diffusivity
    Info<< "creating anode liquid water field diffLiquiWater \n" << endl;
    volScalarField DiffAnodeLiquidWater
    (
        IOobject
        (
            "DiffanodeLiquidWater",
            runTime.timeName(),
            anodeFluidMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        anodeFluidMesh,
        dimensionedScalar("d", dimensionSet(0, 2, -1, 0, 0, 0, 0),1),            
        zeroGradientFvPatchScalarField::typeName
    );

    // capillary pressure
    Info<< "creating anode liquid water field pc \n" << endl;
    volScalarField anodeCapillaryPressure
    (
        IOobject
        (
            "pcanodeLiquidWater",
            runTime.timeName(),
            anodeFluidMesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        anodeFluidMesh,
        0,
        zeroGradientFvPatchScalarField::typeName
    );

    // saturation
    Info<< "Creating anode liquid water field S \n" << endl;
    volScalarField SanodeLiquidWater
    (
        IOobject
        (
            "S",
            runTime.timeName(),
            anodeFluidMesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        anodeFluidMesh
    );

    // normalized saturation function
    volScalarField anodeSaturationFunction
    (
        IOobject
        (
            "g(S)",
            runTime.timeName(),
            anodeFluidMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        anodeFluidMesh,
        1,
        zeroGradientFvPatchScalarField::typeName
    );    

    // condensation rate
    Info<< "Creating anode liquid water field anodePhaseChangeRate \n" << endl;
    volScalarField anodePhaseChangeRate
    (
        IOobject
        (
            "anodePhaseChangeRate",
            runTime.timeName(),
            anodeFluidMesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        anodeFluidMesh,
        dimensionedScalar("aC", dimensionSet(1, -3, -1, 0, 0, 0, 0),0),            
        zeroGradientFvPatchScalarField::typeName
    );

    // density
    Info<< "Creating anode liquid water field rho" << endl;
    volScalarField rhoAnodeLiquidWater
    (
        IOobject
        (
            "rhoanodeLiquidWater",
            runTime.timeName(),
            anodeFluidMesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        anodeFluidMesh,
        rhoAlw,
        zeroGradientFvPatchScalarField::typeName
    );
    rhoAnodeLiquidWater.correctBoundaryConditions();

    // dynamic viscosity
    Info<< "Creating anode liquid water field mu" << endl;
    volScalarField muAnodeLiquidWater
    (
        IOobject
        (
            "muanodeLiquidWater",
            runTime.timeName(),
            anodeFluidMesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        anodeFluidMesh,
        muAlw,
        zeroGradientFvPatchScalarField::typeName
    );
    muAnodeLiquidWater.correctBoundaryConditions();

    // phase change enthalpy
    volScalarField anodePhaseChangeEnthalpySource
    (
        IOobject
        (
            "anodePhaseChangeEnthalpySource",
            runTime.timeName(),
            anodeFluidMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        anodeFluidMesh,
        0,
        zeroGradientFvPatchScalarField::typeName
    );

    // specific heat capacity
    Info<< "Creating anode liquid water field cp" << endl;
    volScalarField cpAnodeLiquidWater
    (
        IOobject
        (
            "cpanodeLiquidWater",
            runTime.timeName(),
            anodeFluidMesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        anodeFluidMesh,
        CpAlw,
        zeroGradientFvPatchScalarField::typeName
    );
    cpAnodeLiquidWater.correctBoundaryConditions();
    
    // thermal conductivity
    Info<< "Creating anode liquid water field k" << endl;
    volScalarField kAnodeLiquidWater
    (
        IOobject
        (
            "kanodeLiquidWater",
            runTime.timeName(),
            anodeFluidMesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        anodeFluidMesh,
        kAlw,
        zeroGradientFvPatchScalarField::typeName
    );
    kAnodeLiquidWater.correctBoundaryConditions();

    /*----------------------------------------------------------------------- */

    // creating temperature field

    // regional field obtained from Tcell
    Info<< "Creating anodeFluid field TanodeFluid" << endl;
    volScalarField TanodeFluid
    (
        IOobject
        (
            "T",
            runTime.timeName(),
            anodeFluidMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        anodeFluidMesh,
        Tinit,
        zeroGradientFvPatchScalarField::typeName
    );

    /*----------------------------------------------------------------------- */

    // creating porous zones

    Info<< "Creating anodeFluid porous zones" << endl;
    myIOporosityModelList anodeZones(anodeFluidMesh);

    Info<< "    number of porous zones = " << anodeZones.UPtrList<myPorosityModel>::size() << nl;

    for(label i=0;i<anodeZones.UPtrList<myPorosityModel>::size();i++)
    {
        label iz = anodeFluidMesh.cellZones().findZoneID(anodeZones.UPtrList<myPorosityModel>::operator[](i).name());
        Info<< "    zone " << anodeZones.UPtrList<myPorosityModel>::operator[](i).cellZoneIDs()
            << " " << anodeZones.UPtrList<myPorosityModel>::operator[](i).name()
            << " : size = " << anodeFluidMesh.cellZones()[iz].size()
            << "; porosity = " << anodeZones.UPtrList<myPorosityModel>::operator[](i).porosity() << nl;
    }
    Info<< endl;

    /*----------------------------------------------------------------------- */

    // creating phase mixture fields

    // rhoAnodeFluid
    volScalarField rhoAnodeFluid
    (
        IOobject
        (
            "rhoAnodeFluid",
            runTime.timeName(),
            anodeFluidMesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        anodeFluidMesh,
        0,            
        zeroGradientFvPatchScalarField::typeName
    );
    rhoAnodeFluid.correctBoundaryConditions();

    // cpAnodeFluid
    volScalarField cpAnodeFluid
    (
        IOobject
        (
            "cpAnodeFluid",
            runTime.timeName(),
            anodeFluidMesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        anodeFluidMesh,
        0,            
        zeroGradientFvPatchScalarField::typeName
    );
    cpAnodeFluid.correctBoundaryConditions();

