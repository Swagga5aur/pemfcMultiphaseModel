// Calculating liquid water saturation in the cathode fluid (J.P.Kone, 17.05.2017)

{
    Info << nl << "Solving cathode liquid water saturation" << endl;

    fvScalarMatrix SEqn
    (
        fvm::laplacian(rhoCathodeLiquidWater*DiffCathodeLiquidWater, ScathodeLiquidWater, "laplacian(diff,S)")
      - fvm::div(phiAir, ScathodeLiquidWater)
      + cathodePhaseChangeRate
    );
    SEqn.solve();
    ScathodeLiquidWater.correctBoundaryConditions();

    Info << "ScathodeLiquidWater min mean max     = "
         << gMin(ScathodeLiquidWater.ref()) << "  "
         << gAverage(ScathodeLiquidWater.ref()) << "  "
         << gMax(ScathodeLiquidWater.ref()) << endl;
}
# include "cathodeSaturationFunction.H"

