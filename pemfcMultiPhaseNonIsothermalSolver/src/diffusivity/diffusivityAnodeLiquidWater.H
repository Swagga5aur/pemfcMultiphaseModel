// Calculating the capillary diffusivity of liquid water in the anode (J.P.Kone, 17.05.2017)

// reference: Sunden and Faghri (2005) Transport phenomena in fuel cells, p.207 
// D = (rho*K*(s^3)/mu)*(dp_c/ds)
// rho = density of liquid water
// K = absolute permeability
// s = saturation of liquid water
// mu = dynamic viscosity of liquid water
// p_c = capillary pressure

{
    scalarField& DiffAnodeLiquidWaterIn = DiffAnodeLiquidWater.ref();

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
                DiffAnodeLiquidWaterIn[zn[celli]] = 2e-5;
            }
        }
        // gas diffusion layer
        else if (zone.name()== "agdl")
        {
            label znId = anodeFluidMesh.cellZones().findZoneID("agdl");
            labelList zn(anodeFluidMesh.cellZones()[znId]);
            scalar porosity = 0.0;

            for(label ip=0;ip<anodeZones.UPtrList<myPorosityModel>::size();ip++)
            {
                if (anodeZones.UPtrList<myPorosityModel>::operator[](ip).name() == "agdl")
                {
                    porosity = anodeZones.UPtrList<myPorosityModel>::operator[](ip).porosity();
                }
            }

            forAll(zn, celli)
            {
                DiffAnodeLiquidWaterIn[zn[celli]] =
                (
                    ((absolutePermeability.value()
                  * SanodeLiquidWater[zn[celli]])
                  / muAnodeLiquidWater[zn[celli]])
                  * ((surfaceTension.value()*Foam::cos(contactAngle.value()))
                  / (Foam::sqrt(absolutePermeability.value()
                  / porosity)))
                  * (
                        contactAngle.value() < 90.0
                      ? (-1.417 + (4.24*(1.0 - SanodeLiquidWater[zn[celli]]))
                        - (3.789*sqr(1.0 - SanodeLiquidWater[zn[celli]])))
                      : (1.417 - (4.24*SanodeLiquidWater[zn[celli]])
                        + (3.789*sqr(SanodeLiquidWater[zn[celli]])))
                    )
                );
            }
        }
        // catalyst layer
        else if(zone.name()== "acl")
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
                DiffAnodeLiquidWaterIn[zn[celli]] =
                (
                    ((absolutePermeability.value()
                  * SanodeLiquidWater[zn[celli]])
                  / muAnodeLiquidWater[zn[celli]])
                  * ((surfaceTension.value()*Foam::cos(contactAngle.value()))
                  / (Foam::sqrt(absolutePermeability.value()
                  / porosity)))
                  * (
                        contactAngle.value() < 90.0
                      ? (-1.417 + (4.24*(1.0 - SanodeLiquidWater[zn[celli]]))
                        - (3.789*sqr(1.0 - SanodeLiquidWater[zn[celli]])))
                      : (1.417 - (4.24*SanodeLiquidWater[zn[celli]])
                        + (3.789*sqr(SanodeLiquidWater[zn[celli]])))
                    )
                );
            }
        }
    }
    DiffAnodeLiquidWater.correctBoundaryConditions();
    Info << nl << "DiffAnodeLiquidWater internalField min mean max     = "
         << gMin(DiffAnodeLiquidWater.ref()) << "  "
         << gAverage(DiffAnodeLiquidWater.ref()) << "  "
         << gMax(DiffAnodeLiquidWater.ref()) << endl;
}
