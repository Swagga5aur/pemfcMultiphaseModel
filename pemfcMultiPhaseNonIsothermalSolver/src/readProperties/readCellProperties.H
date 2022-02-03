    // Code changed and adapted to pemfc multiphase flow (J.P.Kone, 06.07.2017)

    Info<< "Reading cell properties\n" << endl;

    IOdictionary cellProperties
    (
        IOobject
        (
            "cellProperties",
            runTime.constant(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    );

    word fuelCellType(cellProperties.lookup("fuelCellType"));		//Qing, 04.06.2014

    Switch galvanostatic(cellProperties.lookup("galvanostatic"));
    dimensionedScalar V(cellProperties.lookup("voltage"));
    dimensionedScalar Ibar0(cellProperties.lookup("Ibar0"));
    dimensionedScalar Tinit(cellProperties.lookup("Tinit"));
    dimensionedScalar numCells(cellProperties.lookup("numCells"));
    dimensionedScalar Rhat("Rhat", dimensionSet(1, 4, -3, 0, 0, -2, 0), 0);
    dimensionedScalar Rhat0("Rhat0", dimensionSet(1, 4, -3, 0, 0, -2, 0), 0);	//Qing, 22.06.2014
    dimensionedScalar kappaCl(cellProperties.lookup("kappaCl"));
    dimensionedScalar kappaGdl(cellProperties.lookup("kappaGdl"));
    dimensionedScalar rContact(cellProperties.lookup("rContact"));
    dimensionedScalar kappaBp(cellProperties.lookup("kappaBp"));
    dimensionedScalar tCl(cellProperties.lookup("tCl"));
    dimensionedScalar tGdl(cellProperties.lookup("tGdl"));
    dimensionedScalar tBp(cellProperties.lookup("tBp"));
    dimensionedScalar epsilonGdl(cellProperties.lookup("epsilonGdl"));
    dimensionedScalar epsilonCl(cellProperties.lookup("epsilonCl"));
    dimensionedScalar etaConCaConstant(cellProperties.lookup("etaConCaConstant"));

    // liquid water transport
    dimensionedScalar condensationRateConstant(cellProperties.lookup("condensationRateConstant"));
    dimensionedScalar phaseChangeEnthalpySourceCoefficient(cellProperties.lookup("phaseChangeEnthalpySourceCoefficient"));
    dimensionedScalar surfaceTension(cellProperties.lookup("surfaceTension"));
    dimensionedScalar contactAngle(cellProperties.lookup("contactAngle"));
    dimensionedScalar absolutePermeability(cellProperties.lookup("absolutePermeability"));       
    

    // echo inputs from dictionary to stdout
    // -------------------------------------
    if (galvanostatic)
    {
        Info<< "galvanostatic run" << nl;
        Info<< "    V     = " << V << nl;
        Info<< "    Ibar0 = " << Ibar0 << nl;

        Rhat = cellProperties.lookup("Rhat");
        Info<< "    Rhat  = " << Rhat << nl;

        Rhat0 = cellProperties.lookup("Rhat0");
	Info<< "    Rhat0  = " << Rhat0 << nl;
    }
    else
    {
        Info<< "potentiostatic run" << nl;
        Info<< "    Ibar0 = " << Ibar0 << nl;
        Info<< "    V     = " << V << nl;
    }

    Info<< "CL electronic conductivity(kappaCl) = " << kappaCl << nl
        << "GDL electronic conductivity(kappaGdl) = " << kappaGdl << nl
        << "GDL - BP Contact resistance (rContact)= " << rContact << nl
        << "BP electronic conductivity(kappaBp) = " << kappaBp << nl
        << "CL thickness (tCl) = " << tCl << nl
        << "GDL thickness (tGdl) = " << tGdl << nl
        << "BP thickness (tBp) = " << tBp << nl
        << "concentration overpotential constant = " << etaConCaConstant << nl
        << "condensation rate constant = " << condensationRateConstant << nl
        << "phase change enthalpy source coefficient = " << phaseChangeEnthalpySourceCoefficient << nl
        << "surface tension = " << surfaceTension << nl
        << "contact angle = " << contactAngle << nl
        << "absolute permeability = " << absolutePermeability << nl
        << endl;

    Info<< "    Tinit = " << Tinit << nl;
    Info<< endl;


    // special patch names
    // -------------------
    word anodeName(cellProperties.lookup("anodePatch"));
    word anodeFluidInletName(cellProperties.lookup("anodeFluidInletPatch"));
    word anodeFluidOutletName(cellProperties.lookup("anodeFluidOutletPatch"));
    word anodeFluidAbpName(cellProperties.lookup("anodeFluidAbpPatch")); // added (J.P. Kone, 13.06.2016)

    word cathodeName(cellProperties.lookup("cathodePatch"));
    word cathodeFluidInletName(cellProperties.lookup("cathodeFluidInletPatch"));
    word cathodeFluidOutletName(cellProperties.lookup("cathodeFluidOutletPatch"));
    word cathodeFluidCbpName(cellProperties.lookup("cathodeFluidCbpPatch")); // added (J.P. Kone, 13.06.2016)

    word electrolyteAnodeName
        (cellProperties.lookup("electrolyteAnodePatch"));

    word electrolyteCathodeName
        (cellProperties.lookup("electrolyteCathodePatch"));

    word abpAnodeFluidName
            (cellProperties.lookup("abpAnodeFluidPatch")); // added (J.P. Kone, 13.06.2016)

    word cbpCathodeFluidName
                (cellProperties.lookup("cbpCathodeFluidPatch")); // added (J.P. Kone, 13.06.2016)


    // echo inputs from dictionary to stdout
    Info
        << "    anodeName              = " << anodeName << nl
        << "    anodeFluidInletName          = " << anodeFluidInletName << nl
        << "    anodeFluidOutletName         = " << anodeFluidOutletName << nl
	<< "    anodeFluidAbpName            = " << anodeFluidAbpName << nl // added (J.P. Kone, 13.06.2016)
        << "    cathodeName            = " << cathodeName << nl
        << "    cathodeFluidInletName           = " << cathodeFluidInletName << nl
        << "    cathodeFluidOutletName          = " << cathodeFluidOutletName << nl
	<< "    cathodeFluidCbpName             = " << cathodeFluidCbpName << nl // added (J.P. Kone, 13.06.2016)
        << "    electrolyteAnodeName   = " << electrolyteAnodeName << nl
        << "    electrolyteCathodeName = " << electrolyteCathodeName << nl
	<< "    abpAnodeFluidName            = " << abpAnodeFluidName << nl // added (J.P. Kone, 13.06.2016)
	<< "    cbpCathodeFluidName             = " << cbpCathodeFluidName << nl // added (J.P. Kone, 13.06.2016)
        << endl;

