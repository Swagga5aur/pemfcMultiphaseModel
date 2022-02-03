// Calculating water vapour condensation rate in the cathode fluid (J.P.Kone, 17.05.2017)

// r_w = c_r*(1-s)*epsilon*((p_wv-p_Sat)/RT)*M_H2O)
// r_w = condensation rate
// c_r = condensation rate constant = 100s^-1
// s = liquid water saturation rate
// p_wv = water vapour pressure
//      = x_H2O*p_air/p_Sat
// x = mole fraction of water vapour
// p_air = air pressure
// p_Sat = saturation pressure of water vapor (atm)
// R = gas constant
// T = temperature
// M_H2O = molecular weight of water

{
    scalarField& cathodePhaseChangeRateIn = cathodePhaseChangeRate.ref();

    const scalar Rgas = physicalConstant::Rgas.value();   // J/(mol K)
    
    scalarField TcathodeFluidIn = TcathodeFluid.ref();

    // saturation pressure of water vapor
    // value multiply by 101325 to convert to Pa
    scalar T0 = 273.15;         // 273.15 Kelvin = 0 Celsius
    scalar b = 10;

    scalarField pSatCathode =
    (
        pow(b, (-2.1794 + (0.02953*(TcathodeFluidIn-T0))
        - (9.1837e-5*sqr(TcathodeFluidIn-T0))
        + (1.4454e-7*pow((TcathodeFluidIn-T0),3))))
    );
    pSatCathode *= 101325.0;
    Info << nl << "min,max(pSatCathode): "
         << Foam::min(pSatCathode) <<" , "
         << Foam::max(pSatCathode) << endl;

    scalarField XH2O;
    scalarField pwvCathode;
    scalar MH2O = 0.0;

    forAll(airSpecies, s)
    {
        if (airSpecies[s].rSign() == 1)
        {
            MH2O = airSpecies[s].MW();
            XH2O = Xair[s];
            pwvCathode = XH2O*pAir.ref();

            forAll(cathodeFluidMesh.cellZones(), i)
            {
                const cellZone& zone = cathodeFluidMesh.cellZones()[i];

                // gas flow channels
                if(zone.name()=="cgfc")
                {
                    label znId = cathodeFluidMesh.cellZones().findZoneID("cgfc");
                    labelList zn(cathodeFluidMesh.cellZones()[znId]);

                    forAll(zn, celli)
                    {          
                        cathodePhaseChangeRateIn[zn[celli]] = 
                        (
                            pwvCathode[zn[celli]] < pSatCathode[zn[celli]]
                          ? (-condensationRateConstant.value()
                            *(((1.0 - ScathodeLiquidWater[zn[celli]])
                            *(pwvCathode[zn[celli]] - pSatCathode[zn[celli]]))
                            /(Rgas*TcathodeFluidIn[zn[celli]]))
                            *(1e-3*MH2O)) // convert kg/kmol to kg/mol
                          : (condensationRateConstant.value()
                            *(((1.0 - ScathodeLiquidWater[zn[celli]])
                            *(pwvCathode[zn[celli]] - pSatCathode[zn[celli]]))
                            /(Rgas*TcathodeFluidIn[zn[celli]]))
                            *(1e-3*MH2O)) // convert kg/kmol to kg/mol
                        );
                    }
                }
                // gas diffusion layer
                else if(zone.name()== "cgdl")
                {
                    label znId = cathodeFluidMesh.cellZones().findZoneID("cgdl");
                    labelList zn(cathodeFluidMesh.cellZones()[znId]);
                    scalar porosity = 0.0;

                    for(label ip=0;ip<cathodeZones.UPtrList<myPorosityModel>::size();ip++)
                    {
                        if(cathodeZones.UPtrList<myPorosityModel>::operator[](ip).name() == "cgdl")
                        {
                            porosity = cathodeZones.UPtrList<myPorosityModel>::operator[](ip).porosity();
                        }
                    }

                    forAll(zn, celli)
                    {          
                        cathodePhaseChangeRateIn[zn[celli]] =
                        (
                            pwvCathode[zn[celli]] < pSatCathode[zn[celli]]
                          ? (-condensationRateConstant.value()
                            *(((1.0 - ScathodeLiquidWater[zn[celli]])
                            *porosity
                            *(pwvCathode[zn[celli]] - pSatCathode[zn[celli]]))
                            /(Rgas*TcathodeFluidIn[zn[celli]]))
                            *(1e-3*MH2O)) // convert kg/kmol to kg/mol
                          : (condensationRateConstant.value()
                            *(((1.0 - ScathodeLiquidWater[zn[celli]])
                            *porosity
                            *(pwvCathode[zn[celli]] - pSatCathode[zn[celli]]))
                            /(Rgas*TcathodeFluidIn[zn[celli]]))
                            *(1e-3*MH2O)) // convert kg/kmol to kg/mol
                        );
                    }
                }
                // catalyst layer
                else if(zone.name()== "ccl")
                {
                    label znId = cathodeFluidMesh.cellZones().findZoneID("ccl");
                    labelList zn(cathodeFluidMesh.cellZones()[znId]);
                    scalar porosity = 0.0;

                    for(label ip=0;ip<cathodeZones.UPtrList<myPorosityModel>::size();ip++)
                    {
                        if(cathodeZones.UPtrList<myPorosityModel>::operator[](ip).name() == "ccl")
                        {
                            porosity = cathodeZones.UPtrList<myPorosityModel>::operator[](ip).porosity();
                        }
                    }

                    forAll(zn, celli)
                    {          
                        cathodePhaseChangeRateIn[zn[celli]] =
                        (
                            pwvCathode[zn[celli]] < pSatCathode[zn[celli]]
                          ? (-condensationRateConstant.value()
                            *(((1.0 - ScathodeLiquidWater[zn[celli]])
                            *porosity
                            *(pwvCathode[zn[celli]] - pSatCathode[zn[celli]]))
                            /(Rgas*TcathodeFluidIn[zn[celli]]))
                            *(1e-3*MH2O)) // convert kg/kmol to kg/mol
                          : (condensationRateConstant.value()
                            *(((1.0 - ScathodeLiquidWater[zn[celli]])
                            *porosity
                            *(pwvCathode[zn[celli]] - pSatCathode[zn[celli]]))
                            /(Rgas*TcathodeFluidIn[zn[celli]]))
                            *(1e-3*MH2O)) // convert kg/kmol to kg/mol
                        );
                    }
                }
            }
        }
    }
    cathodePhaseChangeRate.correctBoundaryConditions();

    Info << "min,mean,max(cathodePhaseChangeRate): "
         << gMin(cathodePhaseChangeRate.ref()) << "  "
         << gAverage(cathodePhaseChangeRate.ref()) << "  "
         << gMax(cathodePhaseChangeRate.ref()) << endl;
}
