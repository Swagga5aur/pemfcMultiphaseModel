/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright held by original author
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Application
    pemfcMultiphaseNonIsothermalSolver

Description
    Solver that can simulate the multiphase non-isothermal operation of proton exchange membrane fuel cell (PEMFC)

\*---------------------------------------------------------------------------*/

#include <iostream> // standard file
#include <stdio.h> // standard file
#include <iomanip> // standard file

#include "fvCFD.H" // OpenFOAM src file
#include "atomicWeights.H" // OpenFOAM src file
#include "physicalConstants.H"
#include "specie.H" // OpenFOAM src file	
#include "speciesTable.H" // OpenFOAM src file
#include "pemfcSpecie.H"

#include "mypatchToPatchInterpolation.H" // new lib added (J.P.Kone, 20.07.2016) 
#include "myContinuityErrs.H"
#include "initContinuityErrs.H" // OpenFOAM src file
#include "fixedGradientFvPatchFields.H" // OpenFOAM src file
#include "smearPatchToMesh.H"

#include "diffusivityModels.H"
#include "myPorosityModel.H" // new lib added (J.P.Kone, 26.07.2016)
#include "myIOporosityModelList.H" // new lib added (J.P.Kone, 26.07.2016)
#include "phaseModel.H" // new lib added (J.P.Kone, 20.02.2017)
#include "polyToddYoung.H"
#include "RiddersRoot.H"//added SBB
#include "functionIntegration.H" //added (J.P.Kone, 26.10.2016)
#include "functionElectrolyteResistance.H" //added (J.P.Kone, 26.10.2016)
#include "functionTotalCurrent.H" //added (J.P.Kone, 01.11.2016)

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
#   include "setRootCase.H"	// OpenFOAM src file
#   include "createTime.H"	// OpenFOAM src file

    // Complete cell components
#   include "createMesh.H" // OpenFOAM src file
#   include "readCellProperties.H"
#   include "createCellFields.H"

    // BipolarPlate components
#   include "createAbpMesh.H"
#   include "createCbpMesh.H"
#   include "readAbpProperties.H"
#   include "readCbpProperties.H"
#   include "createAbpFields.H"
#   include "createCbpFields.H"

    // AnodeFluid-related components
#   include "createAnodeFluidMesh.H"
#   include "readAnodeFluidProperties.H"
#   include "createAnodeFluidFields.H"
#   include "createFuelSpecies.H"

    // Electrolyte components
#   include "createElectrolyteMesh.H"
#   include "readElectrolyteProperties.H"
#   include "readActivationParameters.H"//Added SBB
#   include "createElectrolyteFields.H"

    // CathodeFluid-related components
#   include "createCathodeFluidMesh.H"
#   include "readCathodeFluidProperties.H"
#   include "createCathodeFluidFields.H"
#   include "createAirSpecies.H"

#   include "readRxnProperties.H"

#   include "setGlobalPatchIds.H"

    // calculate electrolyte thickness, tMem and thickness of gdl, tGdl
#   include "geometricParameter.H"

    // Cathode & Anode interpolation
#   include "createPatchToPatchInterpolation.H"

    // Gas diffusivity models
#   include "createDiffusivityModels.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< "\nStarting time loop\n" << endl;

    bool firstTime = true;

    for (runTime++; !runTime.end(); runTime++)
    {
        Info<< "Time = " << runTime.timeName() << nl << endl;

    #   include "mapFromCell.H"    // map global T to fluid regions

    #   include "rhoFuel.H"
    #   include "rhoAir.H"

    #   include "muFuel.H"
    #   include "muAir.H"

    #   include "kFuel.H"
    #   include "kAir.H"

    #   include "solveAir.H" // air is solved before fuel
    #   include "solveFuel.H"

    #   include "ReynoldsNumber.H"

    #   include "anodeCapillaryPressure.H"
    #   include "cathodeCapillaryPressure.H"

    #   include "diffusivityAnodeLiquidWater.H"
    #   include "diffusivityCathodeLiquidWater.H"

    #   include "anodePhaseChangeRate.H"
    #   include "cathodePhaseChangeRate.H"

    #   include "solveAnodeLiquidWaterSaturation.H"
    #   include "solveCathodeLiquidWaterSaturation.H"

    #   include "anodePhaseChangeEnthalpySource.H"
    #   include "cathodePhaseChangeEnthalpySource.H"

    #   include "diffusivityFuel.H"
    #   include "diffusivityAir.H"

    #   include "YairEqn.H" // Yair is solved before fuel
    #   include "YfuelEqn.H"

    #   include "solveElectrochemistry.H"

    #   include "mapToCell.H"

    #   include "solveEnergy.H"

        runTime.write();

        if(firstTime)
        {
            firstTime = false;
        }

        Info<< "ExecutionTime = "
            << runTime.elapsedCpuTime()
            << " s\n\n" << endl;
    }

    Info<< "End\n" << endl;
    return(0);
}


// ************************************************************************* //
