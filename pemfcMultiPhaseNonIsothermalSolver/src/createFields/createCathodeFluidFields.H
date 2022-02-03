// Code changed and adapted to pemfc multiphase flow model (J.P.Kone, 17.05.2017)
// for coversion to openfoam-4.0
// type 'porousZones' is changed to 'myIOporosityModelList'
// all functions of class porousZone are replaced with
// equivalent funcions from new class porosityModel
// 'forAll' loop is changed to 'for' to avoid any ambiguity

    // air fields

    // pressure
    Info<< "Creating air field p" << endl;
    volScalarField pAir
    (
        IOobject
        (
            "p",
            runTime.timeName(),
            cathodeFluidMesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        cathodeFluidMesh
    );

    // density
    Info<< "Creating air field rho" << endl;
    volScalarField rhoAir
    (
        IOobject
        (
            "rho",
            runTime.timeName(),
            cathodeFluidMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        cathodeFluidMesh,
        rhoA,
        zeroGradientFvPatchScalarField::typeName
    );
    rhoAir.correctBoundaryConditions();

    // dynamic viscosity
    Info<< "Creating air field mu" << endl;
    volScalarField muAir
    (
        IOobject
        (
            "mu",
            runTime.timeName(),
            cathodeFluidMesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        cathodeFluidMesh,
        muA,
        zeroGradientFvPatchScalarField::typeName
    );
    muAir.correctBoundaryConditions();

    // velocity
    Info<< "Creating air field U" << endl;
    volVectorField Uair
    (
        IOobject
        (
            "U",
            runTime.timeName(),
            cathodeFluidMesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        cathodeFluidMesh
    );

    // face flux
    Info<< "Creating air face flux field phi" << endl;
    surfaceScalarField phiAir
    (
        IOobject
        (
            "phiair",
            runTime.timeName(),
            cathodeFluidMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        linearInterpolate(rhoAir*Uair) & cathodeFluidMesh.Sf()
    );

    // inverse matrix diagonal field rUA
    volScalarField rUAair
    (
        IOobject
        (
            "(1|A(U))",
            runTime.timeName(),
            cathodeFluidMesh
        ),
        cathodeFluidMesh,
        dimensionedScalar("rUA", dimTime/dimDensity, 1),
        zeroGradientFvPatchScalarField::typeName
    );

    // Pressure reference point in air
    label pAirRefCell = 0;
    scalar pAirRefValue = 0.0;

    setRefCell
    (
        pAir,
        cathodeFluidMesh.solutionDict().subDict("PISO"),
        pAirRefCell,
        pAirRefValue
    );

    // specific heat capacity
    Info<< "Creating air field cp" << endl;
    volScalarField cpAir
    (
        IOobject
        (
            "cpair",
            runTime.timeName(),
            cathodeFluidMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        cathodeFluidMesh,
        CpA,
        zeroGradientFvPatchScalarField::typeName
    );
    cpAir.correctBoundaryConditions();

    // thermal conductivity
    Info<< "Creating air field k" << endl;
    volScalarField kAir
    (
        IOobject
        (
            "kair",
            runTime.timeName(),
            cathodeFluidMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        cathodeFluidMesh,
        kA,
        zeroGradientFvPatchScalarField::typeName
    );
    kAir.correctBoundaryConditions();

    /*----------------------------------------------------------------------- */

    // creating cathode liquid water fields

    // diffusivity
    Info<< "creating cathode liquid water field diffLiquiWater \n" << endl;
    volScalarField DiffCathodeLiquidWater
    (
        IOobject
        (
            "DiffcathodeLiquidWater",
            runTime.timeName(),
            cathodeFluidMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        cathodeFluidMesh,
        dimensionedScalar("d", dimensionSet(0, 2, -1, 0, 0, 0, 0),1),            
        zeroGradientFvPatchScalarField::typeName
    );

    // capillary pressure
    Info<< "creating cathode liquid water field pc \n" << endl;
    volScalarField cathodeCapillaryPressure
    (
        IOobject
        (
            "pccathodeLiquidWater",
            runTime.timeName(),
            cathodeFluidMesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        cathodeFluidMesh,
        0,
        zeroGradientFvPatchScalarField::typeName
    );

    // saturation
    Info<< "Creating cathode liquid water field S \n" << endl;
    volScalarField ScathodeLiquidWater
    (
        IOobject
        (
            "S",
            runTime.timeName(),
            cathodeFluidMesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        cathodeFluidMesh
    );

    // normalized saturation function
    volScalarField cathodeSaturationFunction
    (
        IOobject
        (
            "g(S)",
            runTime.timeName(),
            cathodeFluidMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        cathodeFluidMesh,
        1,
        zeroGradientFvPatchScalarField::typeName
    );    

    // condensation rate
    Info<< "Creating cathode liquid water field cathodePhaseChangeRate \n" << endl;
    volScalarField cathodePhaseChangeRate
    (
        IOobject
        (
            "cathodePhaseChangeRate",
            runTime.timeName(),
            cathodeFluidMesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        cathodeFluidMesh,
        dimensionedScalar("cC", dimensionSet(1, -3, -1, 0, 0, 0, 0),0),            
        zeroGradientFvPatchScalarField::typeName
    );

    // density
    Info<< "Creating cathode liquid water field rho" << endl;
    volScalarField rhoCathodeLiquidWater
    (
        IOobject
        (
            "rhocathodeLiquidWater",
            runTime.timeName(),
            cathodeFluidMesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        cathodeFluidMesh,
        rhoClw,
        zeroGradientFvPatchScalarField::typeName
    );
    rhoCathodeLiquidWater.correctBoundaryConditions();

    // dynamic viscosity
    Info<< "Creating cathode liquid water field mu" << endl;
    volScalarField muCathodeLiquidWater
    (
        IOobject
        (
            "mucathodeLiquidWater",
            runTime.timeName(),
            cathodeFluidMesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        cathodeFluidMesh,
        muClw,
        zeroGradientFvPatchScalarField::typeName
    );
    muCathodeLiquidWater.correctBoundaryConditions();

    // phase change enthalpy
    volScalarField cathodePhaseChangeEnthalpySource
    (
        IOobject
        (
            "cathodePhaseChangeEnthalpySource",
            runTime.timeName(),
            cathodeFluidMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        cathodeFluidMesh,
        0,
        zeroGradientFvPatchScalarField::typeName
    );

    // specific heat capacity
    Info<< "Creating cathode liquid water field cp" << endl;
    volScalarField cpCathodeLiquidWater
    (
        IOobject
        (
            "cpcathodeLiquidWater",
            runTime.timeName(),
            cathodeFluidMesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        cathodeFluidMesh,
        CpClw,
        zeroGradientFvPatchScalarField::typeName
    );
    cpCathodeLiquidWater.correctBoundaryConditions();
    
    // thermal conductivity
    Info<< "Creating cathode liquid water field k" << endl;
    volScalarField kCathodeLiquidWater
    (
        IOobject
        (
            "kcathodeLiquidWater",
            runTime.timeName(),
            cathodeFluidMesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        cathodeFluidMesh,
        kClw,
        zeroGradientFvPatchScalarField::typeName
    );
    kCathodeLiquidWater.correctBoundaryConditions();

    /*----------------------------------------------------------------------- */

    // temperature field

    // regional field obtained from Tcell
    Info<< "Creating cathodeFluid field TcathodeFluid" << endl;
    volScalarField TcathodeFluid
    (
        IOobject
        (
            "T",
            runTime.timeName(),
            cathodeFluidMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        cathodeFluidMesh,
        Tinit,
        zeroGradientFvPatchScalarField::typeName
    );

    /*----------------------------------------------------------------------- */

    // creating porous zones
    Info<< "Creating cathodeFluid porous zones" << endl;
    myIOporosityModelList cathodeZones(cathodeFluidMesh);

    Info<< "    number of porous zones = " << cathodeZones.UPtrList<myPorosityModel>::size() << nl;

    for(label i=0;i<cathodeZones.UPtrList<myPorosityModel>::size();i++)
    {
        label iz = cathodeFluidMesh.cellZones().findZoneID(cathodeZones.UPtrList<myPorosityModel>::operator[](i).name());
        Info<< "    zone " << cathodeZones.UPtrList<myPorosityModel>::operator[](i).cellZoneIDs()
            << " " << cathodeZones.UPtrList<myPorosityModel>::operator[](i).name()
            << " : size = " << cathodeFluidMesh.cellZones()[iz].size()
            << "; porosity = " << cathodeZones.UPtrList<myPorosityModel>::operator[](i).porosity() << nl;;
    }
    Info<< endl;

    /*----------------------------------------------------------------------- */

    // creating phase mixture fields

    // rhoCathodeFluid
    volScalarField rhoCathodeFluid
    (
        IOobject
        (
            "rhoCathodeFluid",
            runTime.timeName(),
            cathodeFluidMesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        cathodeFluidMesh,
        0,            
        zeroGradientFvPatchScalarField::typeName
    );
    rhoCathodeFluid.correctBoundaryConditions();

    // cpCathodeFluid
    volScalarField cpCathodeFluid
    (
        IOobject
        (
            "cpCathodeFluid",
            runTime.timeName(),
            cathodeFluidMesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        cathodeFluidMesh,
        0,            
        zeroGradientFvPatchScalarField::typeName
    );
    cpCathodeFluid.correctBoundaryConditions();

