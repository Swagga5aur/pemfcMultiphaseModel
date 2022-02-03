// Calculating the capillary pressure in the cathode (J.P.Kone, 16.02.2017)

// reference: Wang et al (2013) PEM fuel cells - thermal and water management, p.140
// p_c = sigma*cos(theta)/((K/epsilon)^1/2)*J(s)
// sigma = surface tension
// theta = contact angle
// epsilon = porosity
// K = absolute permeability
// J(s) = the Leverett J function
//      = 1.417(1-s)-2.12((1-s)^2)+1.263((1-s)^3) for theta < 90Deg
//        and
//        1.417s-2.12(s^2)+1.263(s^3) for theta > 90Deg
// s = cathode liquid water saturation

{
    scalarField& cathodeCapillaryPressureIn = cathodeCapillaryPressure.ref();

    forAll(cathodeFluidMesh.cellZones(), i)
    {
        const cellZone& zone = cathodeFluidMesh.cellZones()[i];

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
                cathodeCapillaryPressureIn[zn[celli]] =
                (
                    ((surfaceTension.value()*Foam::cos(contactAngle.value()))
                  / (Foam::sqrt(absolutePermeability.value()
                  / porosity)))
                  * (
                        contactAngle.value() < 90.0
                      ? ((1.417*(1.0 - ScathodeLiquidWater[zn[celli]]))
                        - (2.12*sqr(1.0 - ScathodeLiquidWater[zn[celli]]))
                        + (1.263*pow((1.0 - ScathodeLiquidWater[zn[celli]]),3)))
                      : ((1.417*ScathodeLiquidWater[zn[celli]])
                        - (2.12*sqr(ScathodeLiquidWater[zn[celli]]))
                        + (1.2*pow(ScathodeLiquidWater[zn[celli]],3)))
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
                cathodeCapillaryPressureIn[zn[celli]] =
                (
                    ((surfaceTension.value()*Foam::cos(contactAngle.value()))
                  / (Foam::sqrt(absolutePermeability.value()
                  / porosity)))
                  * (
                        contactAngle.value() < 90.0
                      ? ((1.417*(1.0 - ScathodeLiquidWater[zn[celli]]))
                        - (2.12*sqr(1.0 - ScathodeLiquidWater[zn[celli]]))
                        + (1.263*pow((1.0 - ScathodeLiquidWater[zn[celli]]),3)))
                      : ((1.417*ScathodeLiquidWater[zn[celli]])
                        - (2.12*sqr(ScathodeLiquidWater[zn[celli]]))
                        + (1.2*pow(ScathodeLiquidWater[zn[celli]],3)))
                    )  
                );
            }
        }
    }
    cathodeCapillaryPressure.correctBoundaryConditions();

    Info << "min,mean,max(cathodeCapillaryPressure): "
         << gMin(cathodeCapillaryPressure.ref()) << "  "
         << gAverage(cathodeCapillaryPressure.ref()) << "  "
         << gMax(cathodeCapillaryPressure.ref()) << endl;
}
