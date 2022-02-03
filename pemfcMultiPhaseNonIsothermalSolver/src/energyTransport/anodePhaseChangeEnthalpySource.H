// Enthalpy source due to phase change in the anode (J.P.Kone, 29.05.2017)
// Spc (Watt/meter^3) = rw*hmfg
// rw = condensation rate
// hpc = specific latent heat of evaporation or condensation of water (J/kg)
//     = 45070 - 41.9*T + 3.44*10^-3*T^2 + 2.54*10^-6*T^3 - 8.98*10^-10*T^4 (J/mol)
// (ref: Nguyen and White, 1993)
 
{
    Info << nl << "Calculating anode phase change enthalpy" << endl;

    scalarField TanodeFluidIn = TanodeFluid.ref();

    // latent heat of phase change
    scalar T0 = 273.15;         // 273.15 Kelvin = 0 Celsius
    scalarField anodeWaterLatentHeat =
    (
        45070.0
      - (41.9*(TanodeFluidIn - T0))
      + (3.44e-3*sqr(TanodeFluidIn - T0))
      + (2.54e-6*pow((TanodeFluidIn - T0),3))
      - (8.98e-10*pow((TanodeFluidIn - T0),4))
    );
    anodeWaterLatentHeat /=0.018; // to convert J/mol to J/kg

    Info<< "anodeWaterLatentHeat min, max: "
        << gMin(anodeWaterLatentHeat) << " "
        << gMax(anodeWaterLatentHeat) << endl;

    scalarField& anodePhaseChangeEnthalpySourceIn = anodePhaseChangeEnthalpySource.ref();
    anodePhaseChangeEnthalpySourceIn =
    (
        phaseChangeEnthalpySourceCoefficient.value()
       *anodePhaseChangeRate.ref()
       *anodeWaterLatentHeat
    );
    anodePhaseChangeEnthalpySource.correctBoundaryConditions();

    Info<< "min,mean,max (anodePhaseChangeEnthalpySource): "
        << gMin(anodePhaseChangeEnthalpySource.ref()) << " "
        << gAverage(anodePhaseChangeEnthalpySource.ref()) << "  "
        << gMax(anodePhaseChangeEnthalpySource.ref()) << endl;
}
