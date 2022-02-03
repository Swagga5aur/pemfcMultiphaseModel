// Calculating liquid water saturation in the anode fluid (J.P.Kone, 17.05.2017)

{
    Info << nl << "Solving anode liquid water saturation" << endl;
 
    fvScalarMatrix SEqn
    (
        fvm::laplacian(rhoAnodeLiquidWater*DiffAnodeLiquidWater, SanodeLiquidWater, "laplacian(diff,S)")
      - fvm::div(phiFuel, SanodeLiquidWater)
      + anodePhaseChangeRate
    );
    SEqn.solve();
    SanodeLiquidWater.correctBoundaryConditions();

    Info << "SanodeLiquidWater min mean max     = "
         << gMin(SanodeLiquidWater.ref()) << "  "
         << gAverage(SanodeLiquidWater.ref()) << "  "
         << gMax(SanodeLiquidWater.ref()) << endl;
}
# include "anodeSaturationFunction.H"

