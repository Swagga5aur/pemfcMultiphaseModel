// Code changed and adapted to pemfc multiphase flow model (J.P.Kone, 08.02.2017)
{
    Info << nl << "Solving air scalars" << endl;

    tmp<fv::convectionScheme<scalar> > yConvection
    (
        fv::convectionScheme<scalar>::New
        (
            cathodeFluidMesh,
            airYs,
            phiAir,
            cathodeFluidMesh.divScheme("div(phiair,y)")
        )
    );

    label inertIndex = -1;
    volScalarField Yt("Yt", 0.0*Yair[0]);

    // code structure changed (J. P. Kone, 18.09.2016)
    for (label i = 0; i < Yair.size(); i++)
    {
        if (airSpecies[i].name() != airInertSpecie)
        {
            Info << " Yair["<< i << "].name() = " << Yair[i].name() << nl;

            volScalarField& Yi = Yair[i];
            volScalarField& diffAirS = diffSpAir[i];

            solve
            (
                fvm::ddt(rhoAir, Yi)
              + yConvection->fvmDiv(phiAir, Yi)
              - fvm::laplacian(rhoAir*diffAirS, Yi, "laplacian(diff,y)"),
                cathodeFluidMesh.solver("Yi")
            );

            //Yi.max(0.0);
            Yi.max(Foam::doubleScalarSmall);
            Yt += Yi;
        }
        else
        {
            Info << " Yair["<< i << "].name() = " << Yair[i].name() << nl;
            inertIndex = i;
        }
    }
    
    Info<< "inertIndex = " << inertIndex << nl;
    Yair[inertIndex] = scalar(1) - Yt;
    //Yair[inertIndex].max(0.0);
    Yair[inertIndex].max(Foam::doubleScalarSmall);

    forAll(Yair, i)
    {
        Info << "min,mean,max(" << Yair[i].name() << "): "
             << gMin(Yair[i]) <<" , "
             << gAverage(Yair[i]) <<" , "
             << gMax(Yair[i]) << endl;
    }
}

// calculate mole fractions
#include "getXair.H"

