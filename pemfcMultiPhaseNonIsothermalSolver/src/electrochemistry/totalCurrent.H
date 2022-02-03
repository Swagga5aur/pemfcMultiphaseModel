// calculating local current densities (J.P.Kone, 17.10.2016)

// from
// E_cell = E_Nernst - I*ASR - (R*T/alpha*F*ln(I/I0)) - (B*ln(IL/(IL-I)))
// where
// E_cell = cell potential
// E_Nernst = Nernst potential
// I = local current density
// ASR = area specific resistance
// R = gas constant
// T = temperature
// alpha = transfer coefficient
// F = Faraday's constant
// I0 = exchange current density
// IL = limiting current density

scalarField I(cathodePatch.size());
{
    scalar A;
    scalar B = etaConCaConstant.value();
    scalar r0 = 1.0;
    scalar r1 = 0.0;

    forAll (I, facei)
    {
        A = (Rgas*cathodeT[facei])/(alphaCathodeElectrolyte.value()*F);

        r1 = iLCathode[facei] - 1.0; // (J.P.Kone, 06.07.2017)
               
        functionTotalCurrent ftc
        (
            Nernst[facei], 
            voltage.boundaryFieldRef()[electrolyteCathodeID][facei],
            R[facei],
            A,
            i0Cathode[facei],
            B,
            iLCathode[facei]
        );

        I[facei] = RiddersRoot<functionTotalCurrent>(ftc, 1.0e-5).root(r0, r1); // upper boundary for root bracketing must be less than the limiting current density

    }
}
