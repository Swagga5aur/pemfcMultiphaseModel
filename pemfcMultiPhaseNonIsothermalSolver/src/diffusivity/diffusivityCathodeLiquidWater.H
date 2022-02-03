// Calculating the capillary diffusivity of liquid water in the cathode (J.P.Kone, 17.05.2017)

// reference: Sunden and Faghri (2005) Transport phenomena in fuel cells, p.207 
// D = (rho*K*(s^3)/mu)*(dp_c/ds)
// rho = density of liquid water
// K = absolute permeability
// s = saturation of liquid water
// mu = dynamic viscosity of liquid water
// p_c = capillary pressure

{
    scalarField& DiffCathodeLiquidWaterIn = DiffCathodeLiquidWater.ref();

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
                DiffCathodeLiquidWaterIn[zn[celli]] = 2e-5;
            }
        }
        // gas diffusion layer
        if(zone.name()== "cgdl")
        {
            label znId = cathodeFluidMesh.cellZones().findZoneID("cgdl");
            labelList zn(cathodeFluidMesh.cellZones()[znId]);
            scalar porosity = 0.0;

            for(label ip=0;ip<cathodeZones.UPtrList<myPorosityModel>::size();ip++)
            {
                if (cathodeZones.UPtrList<myPorosityModel>::operator[](ip).name() == "cgdl")
                {
                    porosity = cathodeZones.UPtrList<myPorosityModel>::operator[](ip).porosity();
                }
            }

            forAll(zn, celli)
            {
                DiffCathodeLiquidWaterIn[zn[celli]] =
                (
                    ((absolutePermeability.value()
                  * ScathodeLiquidWater[zn[celli]])
                  / muCathodeLiquidWater[zn[celli]])
                  * ((surfaceTension.value()*Foam::cos(contactAngle.value()))
                  / (Foam::sqrt(absolutePermeability.value()
                  / porosity)))
                  * (
                        contactAngle.value() < 90.0
                      ? (-1.417 + (4.24*(1.0 - ScathodeLiquidWater[zn[celli]]))
                        - (3.789*sqr(1.0 - ScathodeLiquidWater[zn[celli]])))
                      : (1.417 - (4.24*ScathodeLiquidWater[zn[celli]])
                        + (3.789*sqr(ScathodeLiquidWater[zn[celli]])))
                    )
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
                if (cathodeZones.UPtrList<myPorosityModel>::operator[](ip).name() == "ccl")
                {
                    porosity = cathodeZones.UPtrList<myPorosityModel>::operator[](ip).porosity();
                }
            }

            forAll(zn, celli)
            {
                DiffCathodeLiquidWaterIn[zn[celli]] =
                (
                    ((absolutePermeability.value()
                  * ScathodeLiquidWater[zn[celli]])
                  / muCathodeLiquidWater[zn[celli]])
                  * ((surfaceTension.value()*Foam::cos(contactAngle.value()))
                  / (Foam::sqrt(absolutePermeability.value()
                  / porosity)))
                  * (
                        contactAngle.value() < 90.0
                      ? (-1.417 + (4.24*(1.0 - ScathodeLiquidWater[zn[celli]]))
                        - (3.789*sqr(1.0 - ScathodeLiquidWater[zn[celli]])))
                      : (1.417 - (4.24*ScathodeLiquidWater[zn[celli]])
                        + (3.789*sqr(ScathodeLiquidWater[zn[celli]])))
                    )
                );
            }
        }
    }
    Info << "DiffCathodeLiquidWater internalField min mean max     = "
         << gMin(DiffCathodeLiquidWater.ref()) << "  "
         << gAverage(DiffCathodeLiquidWater.ref()) << "  "
         << gMax(DiffCathodeLiquidWater.ref()) << endl;
}
