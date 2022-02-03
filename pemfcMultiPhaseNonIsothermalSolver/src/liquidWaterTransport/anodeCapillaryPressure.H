// Calculating the capillary pressure in the anode (J.P.Kone, 16.02.2017)

// reference: Wang et al (2013) PEM fuel cells - thermal and water management, p.140
// p_c = sigma*cos(theta)*((epsilon/K)^1/2)*J(s)
// sigma = surface tension
// theta = contact angle
// epsilon = porosity
// K = absolute permeability
// J(s) = the Leverett J function
//      = 1.417(1-s)-2.12((1-s)^2)+1.263((1-s)^3) for theta < 90Deg
//        and
//        1.417s-2.12(s^2)+1.263(s^3) for theta > 90Deg
// s = anode liquid water saturation

{
    scalarField& anodeCapillaryPressureIn = anodeCapillaryPressure.ref();

    forAll(anodeFluidMesh.cellZones(), i)
    {
        const cellZone& zone = anodeFluidMesh.cellZones()[i];

        // gas diffusion layer
        if(zone.name()== "agdl")
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
                anodeCapillaryPressureIn[zn[celli]] =
                (
                    ((surfaceTension.value()*Foam::cos(contactAngle.value()))
                  / (Foam::sqrt(absolutePermeability.value()
                  / porosity)))
                  * (
                        contactAngle.value() < 90.0
                      ? ((1.417*(1-SanodeLiquidWater[zn[celli]]))
                        - (2.12*sqr(1.0 - SanodeLiquidWater[zn[celli]]))
                        + (1.263*pow((1.0 - SanodeLiquidWater[zn[celli]]),3)))
                      : ((1.417*SanodeLiquidWater[zn[celli]])
                        - (2.12*sqr(SanodeLiquidWater[zn[celli]]))
                        + (1.2*pow(SanodeLiquidWater[zn[celli]],3)))
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
                anodeCapillaryPressureIn[zn[celli]] =
                (
                    ((surfaceTension.value()*Foam::cos(contactAngle.value()))
                  / (Foam::sqrt(absolutePermeability.value()
                  / porosity)))
                  * (
                        contactAngle.value() < 90.0
                      ? ((1.417*(1-SanodeLiquidWater[zn[celli]]))
                        - (2.12*sqr(1.0 - SanodeLiquidWater[zn[celli]]))
                        + (1.263*pow((1.0 - SanodeLiquidWater[zn[celli]]),3)))
                      : ((1.417*SanodeLiquidWater[zn[celli]])
                        - (2.12*sqr(SanodeLiquidWater[zn[celli]]))
                        + (1.2*pow(SanodeLiquidWater[zn[celli]],3)))
                    )
                );
            }
        }
    }
    anodeCapillaryPressure.correctBoundaryConditions();

    Info << nl << "min,mean,max(anodeCapillaryPressure): "
         << gMin(anodeCapillaryPressure.ref()) << "  "
         << gAverage(anodeCapillaryPressure.ref()) << "  "
         << gMax(anodeCapillaryPressure.ref()) << endl;
}
