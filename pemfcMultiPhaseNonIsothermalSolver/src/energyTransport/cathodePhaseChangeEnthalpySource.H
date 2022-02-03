// Enthalpy source due to phase change in the cathode (J.P.Kone, 29.05.2017)
// Spc (Watt/meter^3) = rw*hmfg
// rw = condensation rate
// hpc = specific latent heat of evaporation or condensation of water (J/kg)
//     = 45070 - 41.9*T + 3.44*10^-3*T^2 + 2.54*10^-6*T^3 - 8.98*10^-10*T^4 (J/mol)
// (ref: Nguyen and White, 1993)

{ 
    Info << nl << "Calculating cathode phase change enthalpy" << endl;

    scalarField TcathodeFluidIn = TcathodeFluid.ref();

    // latent heat of phase change
    scalar T0 = 273.15;         // 273.15 Kelvin = 0 Celsius
    scalarField cathodeWaterLatentHeat =
    (
        45070.0
      - (41.9*(TcathodeFluidIn - T0))
      + (3.44e-3*sqr(TcathodeFluidIn - T0))
      + (2.54e-6*pow((TcathodeFluidIn - T0),3))
      - (8.98e-10*pow((TcathodeFluidIn - T0),4))
    );
    cathodeWaterLatentHeat /=0.018; // to convert J/mol to J/kg

    Info<< "cathodeWaterLatentHeat min, max: "
        << gMin(cathodeWaterLatentHeat) << " "
        << gMax(cathodeWaterLatentHeat) << endl;

    scalarField& cathodePhaseChangeEnthalpySourceIn = cathodePhaseChangeEnthalpySource.ref();
    cathodePhaseChangeEnthalpySourceIn =
    (
        phaseChangeEnthalpySourceCoefficient.value()
       *cathodePhaseChangeRate.ref()
       *cathodeWaterLatentHeat
    );
    cathodePhaseChangeEnthalpySource.correctBoundaryConditions();

    Info<< "min,mean,max (cathodePhaseChangeEnthalpySource): "
        << gMin(cathodePhaseChangeEnthalpySource.ref()) << " "
        << gAverage(cathodePhaseChangeEnthalpySource.ref()) << " "
        << gMax(cathodePhaseChangeEnthalpySource.ref()) << endl;
}
