// Calculating normalized saturation function in the anode fluid (J.P.Kone, 13.05.2017)

// Source: Dawes et al (2009) Three-dimensional CFD modelling of PEM fuel cells:
// An investigation into the effects of water flooding.
// g(S) = [((1-S)-0.11)^0.9]/[(1-0.11)^0.9]
// where
// S = Saturation

{
    Info << nl << "Calculating anode normalized saturation function" << endl;

    anodeSaturationFunction =
    (
        Foam::pow(((1.0 - SanodeLiquidWater) - 0.11),0.9)
      / Foam::pow((1.0 - 0.11),0.9)
    );
    anodeSaturationFunction.correctBoundaryConditions();
    Info << "anodeSaturationFunction min mean max     = "
         << gMin(anodeSaturationFunction.ref()) << "  "
         << gAverage(anodeSaturationFunction.ref()) << "  "
         << gMax(anodeSaturationFunction.ref()) << endl;
}
