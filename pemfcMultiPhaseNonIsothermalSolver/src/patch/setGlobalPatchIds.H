    // Code changed and adapted to pemfc multiphase flow model (J.P.Kone, 08.02.2017)

    Info<< "Setting special patch IDs\n" << endl;

    // IDs of the named patches
    // ------------------------

    label anodeID = anodeFluidMesh.boundaryMesh().findPatchID(anodeName);
    if (anodeID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << anodeName << " on the anodeFluid mesh."
            << abort(FatalError);
    }

    label anodeFluidInletID = anodeFluidMesh.boundaryMesh().findPatchID(anodeFluidInletName);
    if (anodeFluidInletID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << anodeFluidInletName << " on the anodeFluid mesh."
            << abort(FatalError);
    }

    label anodeFluidOutletID = anodeFluidMesh.boundaryMesh().findPatchID(anodeFluidOutletName);
    if (anodeFluidOutletID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << anodeFluidOutletName << " on the anodeFluid mesh."
            << abort(FatalError);
    }

    // added (J.P. Kone, 13.06.2016)
    label anodeFluidAbpID = anodeFluidMesh.boundaryMesh().findPatchID(anodeFluidAbpName);
    if (anodeFluidAbpID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << anodeFluidAbpName << " on the anodeFluid mesh."
            << abort(FatalError);
    }


    label cathodeID = cathodeFluidMesh.boundaryMesh().findPatchID(cathodeName);
    if (cathodeID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << cathodeName << " on the cathodeFluid mesh."
            << abort(FatalError);
    }

    label cathodeFluidInletID = cathodeFluidMesh.boundaryMesh().findPatchID(cathodeFluidInletName);
    if (cathodeFluidInletID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << cathodeFluidInletName << " on the cathodeFluid mesh."
            << abort(FatalError);
    }

    label cathodeFluidOutletID = cathodeFluidMesh.boundaryMesh().findPatchID(cathodeFluidOutletName);
    if (cathodeFluidOutletID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << cathodeFluidOutletName << " on the cathodeFluid mesh."
            << abort(FatalError);
    }

    // added (J.P. Kone, 13.06.2016)
    label cathodeFluidCbpID = cathodeFluidMesh.boundaryMesh().findPatchID(cathodeFluidCbpName);
    if (cathodeFluidCbpID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << cathodeFluidCbpName << " on the cathodeFluid mesh."
            << abort(FatalError);
    }


    label electrolyteAnodeID =
        electrolyteMesh.boundaryMesh().findPatchID(electrolyteAnodeName);
    if (electrolyteAnodeID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << electrolyteAnodeName
            << " on the electrolyte mesh."
            << abort(FatalError);
    }

    label electrolyteCathodeID =
        electrolyteMesh.boundaryMesh().findPatchID(electrolyteCathodeName);
    if (electrolyteCathodeID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << electrolyteAnodeName
            << " on the electrolyte mesh."
            << abort(FatalError);
    }

    // added (J.P. Kone, 13.06.2016)
    label abpAnodeFluidID = abpMesh.boundaryMesh().findPatchID(abpAnodeFluidName);
    if (abpAnodeFluidID == -1)
    {
        FatalErrorIn(args.executable())
            << "Cannot find patch " << abpAnodeFluidName
	    << " on the anode bipolarPlate mesh."
            << abort(FatalError);
    }

    // added (J.P. Kone, 13.06.2016)
    label cbpCathodeFluidID = cbpMesh.boundaryMesh().findPatchID(cbpCathodeFluidName);
    if (cbpCathodeFluidID == -1)
    {
        FatalErrorIn(args.executable())
             << "Cannot find patch " << cbpCathodeFluidName
	     << " on the cathode bipolarPlate mesh."
             << abort(FatalError);
        }
