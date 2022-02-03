// Calculating water vapour condensation rate in the anode fluid (J.P.Kone, 29.05.2017)

// r_w = c_r*(1-s)*epsilon*((p_wv-p_Sat)/RT)*M_H2O)
// r_w = condensation rate (kg/m^3 S)
// c_r = condensation rate constant = 100s^-1
// s = liquid water saturation rate
// p_wv = water vapour pressure
//      = x_H2O*p_fuel
// x = mole fraction of water vapour
// p_fuel = fuel pressure
// p_Sat = saturation pressure of water vapor (atm)
// R = gas constant
// T = temperature
// M_H2O = molecular weight of water

{
    scalarField& anodePhaseChangeRateIn = anodePhaseChangeRate.ref();

    const scalar Rgas = physicalConstant::Rgas.value();   // J/(mol K)

    scalarField TanodeFluidIn = TanodeFluid.ref();

    // saturation pressure of water vapor
    // value multiply by 101325 to convert atm to Pa
    scalar T0 = 273.15;         // 273.15 Kelvin = 0 Celsius
    scalar b = 10;

    scalarField pSatAnode =
    (
        pow(b, (-2.1794 + (0.02953*(TanodeFluidIn-T0))
        - (9.1837e-5*sqr(TanodeFluidIn-T0))
        + (1.4454e-7*pow((TanodeFluidIn-T0),3))))
    );
    pSatAnode *= 101325.0;

    Info << nl << "min,max(pSatAnode): "
         << Foam::min(pSatAnode) <<" , "
         << Foam::max(pSatAnode) << endl;

    scalarField XH2O;
    scalarField pwvAnode;
    scalar MH2O = 0.0;

    forAll(fuelSpecies, s)
    {
        if (fuelSpecies[s].rSign() == 0)
        {
            MH2O = fuelSpecies[s].MW();
            XH2O = Xfuel[s];
            pwvAnode = XH2O*pFuel.ref();

            forAll(anodeFluidMesh.cellZones(), i)
            {
                const cellZone& zone = anodeFluidMesh.cellZones()[i];

                // gas flow channels
                if(zone.name()=="agfc")
                {
                    label znId = anodeFluidMesh.cellZones().findZoneID("agfc");
                    labelList zn(anodeFluidMesh.cellZones()[znId]);

                    forAll(zn, celli)
                    {
                        anodePhaseChangeRateIn[zn[celli]] =
                        (
                            pwvAnode[zn[celli]] < pSatAnode[zn[celli]]
                          ? (-condensationRateConstant.value()
                            *(((1.0 - SanodeLiquidWater[zn[celli]])
                            *(pwvAnode[zn[celli]] - pSatAnode[zn[celli]]))
                            /(Rgas*TanodeFluidIn[zn[celli]]))
                            *(1e-3*MH2O)) // convert kg/kmol to kg/mol
                          : (condensationRateConstant.value()
                            *(((1.0 - SanodeLiquidWater[zn[celli]])
                            *(pwvAnode[zn[celli]] - pSatAnode[zn[celli]]))
                            /(Rgas*TanodeFluidIn[zn[celli]]))
                            *(1e-3*MH2O)) // convert kg/kmol to kg/mol
                        );
                    }
                }
                // gas diffusion layer
                else if(zone.name() == "agdl")
                {
                    label znId = anodeFluidMesh.cellZones().findZoneID("agdl");
                    labelList zn(anodeFluidMesh.cellZones()[znId]);
                    scalar porosity = 0.0;

                    for(label ip=0;ip<anodeZones.UPtrList<myPorosityModel>::size();ip++)
                    {
                        if(anodeZones.UPtrList<myPorosityModel>::operator[](ip).name() == "agdl")
                        {
                            porosity = anodeZones.UPtrList<myPorosityModel>::operator[](ip).porosity();
                        }
                    }

                    forAll(zn, celli)
                    {
                        anodePhaseChangeRateIn[zn[celli]] =
                        (
                            pwvAnode[zn[celli]] < pSatAnode[zn[celli]]
                          ? (-condensationRateConstant.value()
                            *(((1.0 - SanodeLiquidWater[zn[celli]])
                            *porosity
                            *(pwvAnode[zn[celli]] - pSatAnode[zn[celli]]))
                            /(Rgas*TanodeFluidIn[zn[celli]]))
                            *(1e-3*MH2O)) // convert kg/kmol to kg/mol
                          : (condensationRateConstant.value()
                            *(((1.0 - SanodeLiquidWater[zn[celli]])
                            *porosity
                            *(pwvAnode[zn[celli]] - pSatAnode[zn[celli]]))
                            /(Rgas*TanodeFluidIn[zn[celli]]))
                            *(1e-3*MH2O)) // convert kg/kmol to kg/mol
                        );
                    }
                }
                // catalyst layer
                else if (zone.name() == "acl")
                {
                    label znId = anodeFluidMesh.cellZones().findZoneID("acl");
                    labelList zn(anodeFluidMesh.cellZones()[znId]);
                    scalar porosity = 0.0;

                    for(label ip=0;ip<anodeZones.UPtrList<myPorosityModel>::size();ip++)
                    {
                        if (anodeZones.UPtrList<myPorosityModel>::operator[](ip).name() == "acl")
                        {
                            porosity = anodeZones.UPtrList<myPorosityModel>::operator[](ip).porosity();
                        }
                    }

                    forAll(zn, celli)
                    {
                        anodePhaseChangeRateIn[zn[celli]] =
                        (
                            pwvAnode[zn[celli]] < pSatAnode[zn[celli]]
                          ? (-condensationRateConstant.value()
                            *(((1.0 - SanodeLiquidWater[zn[celli]])
                            *porosity
                            *(pwvAnode[zn[celli]] - pSatAnode[zn[celli]]))
                            /(Rgas*TanodeFluidIn[zn[celli]]))
                            *(1e-3*MH2O)) // convert kg/kmol to kg/mol
                          : (condensationRateConstant.value()
                            *(((1.0 - SanodeLiquidWater[zn[celli]])
                            *porosity
                            *(pwvAnode[zn[celli]] - pSatAnode[zn[celli]]))
                            /(Rgas*TanodeFluidIn[zn[celli]]))
                            *(1e-3*MH2O)) // convert kg/kmol to kg/mol
                        );
                    }
                }
            }
        }
    }   
    anodePhaseChangeRate.correctBoundaryConditions();

    Info << "min,mean,max(anodePhaseChangeRate): "
         << gMin(anodePhaseChangeRate.ref()) << "  "
         << gAverage(anodePhaseChangeRate.ref()) << "  "
         << gMax(anodePhaseChangeRate.ref()) << endl;
}
