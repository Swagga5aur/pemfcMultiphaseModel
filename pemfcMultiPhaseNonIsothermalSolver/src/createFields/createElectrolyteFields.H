// Code changed and adapted to pemfc electrochemistry (J.P.Kone, 22.11.2016)

    //volumetric electrochemical Heating

    volScalarField electrochemicalHeating
    (
        IOobject
        (
            "electrochemicalHeating",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        electrolyteMesh,
        0,
        zeroGradientFvPatchScalarField::typeName
    );

    volScalarField totalEntropySource
    (
        IOobject
        (
            "totalEntropySource",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        electrolyteMesh,
        0,
        zeroGradientFvPatchScalarField::typeName
    );

    Info<< "creating current density field\n" << endl;

    volScalarField idensity
    (
        IOobject
        (
            "I",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        electrolyteMesh,
        dimensionedScalar
        (
            "I",
            dimensionSet(0, -2, 0, 0, 0, 1, 0),
            Ibar0.value()
        ),
        zeroGradientFvPatchScalarField::typeName
    );

    volScalarField i0C // (J.P.Kone, 05.07.2017)
    (
        IOobject
        (
            "i0C",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        electrolyteMesh,
        dimensionedScalar
        (
            "i0C",
            dimensionSet(0, -2, 0, 0, 0, 1, 0),
            0
        ),
        zeroGradientFvPatchScalarField::typeName
    );

    volScalarField iLC // (J.P.Kone, 05.07.2017)
    (
        IOobject
        (
            "iLC",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        electrolyteMesh,
        dimensionedScalar
        (
            "iLC",
            dimensionSet(0, -2, 0, 0, 0, 1, 0),
            0
        ),
        zeroGradientFvPatchScalarField::typeName
    );

    volScalarField etaOhmic
    (
        IOobject
        (
            "etaOhmic",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        electrolyteMesh,
        dimensionedScalar
        (
            "etaOhmic",
            dimensionSet(1, 2, -3, 0, 0, -1, 0),
            0
        ),
        zeroGradientFvPatchScalarField::typeName
    );

    volScalarField etaActC
    (
        IOobject
        (
            "etaActC",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        electrolyteMesh,
        dimensionedScalar
        (
            "etaAtcC",
            dimensionSet(1, 2, -3, 0, 0, -1, 0),
            0
        ),
        zeroGradientFvPatchScalarField::typeName
    );

    volScalarField etaConC
    (
        IOobject
        (
            "etaConC",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        electrolyteMesh,
        dimensionedScalar
        (
            "etaConC",
            dimensionSet(1, 2, -3, 0, 0, -1, 0),
            0
        ),
        zeroGradientFvPatchScalarField::typeName
    );

    Info<< "creating voltage field\n" << endl;
    volScalarField voltage
    (
        IOobject
        (
            "V",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        electrolyteMesh,
        V,
        zeroGradientFvPatchScalarField::typeName
    );

    Info<< "creating Nernst potential field\n" << endl;
    volScalarField NernstPot
    (
        IOobject
        (
            "NernstPot",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        electrolyteMesh,
        1,
        zeroGradientFvPatchScalarField::typeName
    );

    volScalarField sigmaMem
    (
        IOobject
        (
            "sigmaMem",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        electrolyteMesh,
        dimensionedScalar
        (
            "sigmaMem",
            dimensionSet(-1, -2, 3, 0, 0, 2, 0), // Siemens per meter (S/m)
            sigmaElectrolyte.value()
        ),
        zeroGradientFvPatchScalarField::typeName
    );

    // regional field obtained from Tcell
    Info<< "Creating electrolyte field Telectrolyte" << endl;
    volScalarField Telectrolyte
    (
        IOobject
        (
            "T",
            runTime.timeName(),
            electrolyteMesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        electrolyteMesh,
        Tinit,
        zeroGradientFvPatchScalarField::typeName
    );

