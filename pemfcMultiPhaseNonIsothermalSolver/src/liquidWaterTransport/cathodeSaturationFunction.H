// Calculating normalized saturation function in the cathode fluid (J.P.Kone, 13.05.2017)

// Source: Dawes et al (2009) Three-dimensional CFD modelling of PEM fuel cells:
// An investigation into the effects of water flooding.
// g(S) = [((1-S)-0.11)^0.9]/[(1-0.11)^0.9]
// where
// S = Saturation

{
    Info << nl << "Calculating cathode normalized saturation function" << endl;

    cathodeSaturationFunction =
    (
        Foam::pow(((1.0 - ScathodeLiquidWater) - 0.11),0.9)
      / Foam::pow((1.0 - 0.11),0.9)
    );
    cathodeSaturationFunction.correctBoundaryConditions();
    Info << "cathodeSaturationFunction min mean max     = "
         << gMin(cathodeSaturationFunction.ref()) << "  "
         << gAverage(cathodeSaturationFunction.ref()) << "  "
         << gMax(cathodeSaturationFunction.ref()) << endl;
}
