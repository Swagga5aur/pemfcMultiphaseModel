// Code changed and adapted to pemfc electrochemistry (J.P.Kone, 26.05.2016)

// for coversion to openfoam-4.0
// call to 'internalField()' is changed to 'ref()'

// Current density

    scalarField& idensityIn = idensity.ref();
    smearPatchToMesh smearIdensity(electrolyteMesh, electrolyteCathodeName);
    idensityIn = smearIdensity.smear(I);
    idensity.correctBoundaryConditions();

// Nernst Potential

    scalarField& NernstPotIn = NernstPot.ref();
    smearPatchToMesh smearNernstPot(electrolyteMesh, electrolyteCathodeName);
    NernstPotIn = smearNernstPot.smear(Nernst);
    NernstPot.correctBoundaryConditions();


