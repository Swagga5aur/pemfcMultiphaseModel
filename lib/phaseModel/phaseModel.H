/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 1991-2010 OpenCFD Ltd.
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

Class
    Foam::phaseModel

Description
    Incompressible phase model for the pemfcMultiphaseNonIsothermalSolver.

SourceFiles
    phaseModel.C

\*---------------------------------------------------------------------------*/

    #ifndef phaseModel_H
    #define phaseModel_H

    #include "dictionary.H"
    #include "dimensionedScalar.H"
    #include "volFields.H"
    #include "surfaceFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

    /*---------------------------------------------------------------------------*\
                   Class phaseModel Declaration
    \*---------------------------------------------------------------------------*/

    class phaseModel
    {
        //- Name of phase
        word name_;

        // Private data
        dictionary dict_;

        //- density
        dimensionedScalar rho_;

        //- dynamic viscosity
        dimensionedScalar mu_;

        //- density
        dimensionedScalar Cp_;

        //- density
        dimensionedScalar k_;

    public:

        // Constructors

        phaseModel
        (
            const fvMesh& mesh,
            const dictionary& dict,
            const word& phaseName
        );


        // Selectors

        //- Return a reference to the selected turbulence model
        static autoPtr<phaseModel> New
        (
            const fvMesh& mesh,
            const dictionary& dict,
            const word& phaseName
        );

        //- Destructor
        virtual ~phaseModel();

        // Member Functions
        const word& name() const
            {
                return name_;
            }

        const dimensionedScalar& rho() const
            {
                return rho_;
            }

        const dimensionedScalar& mu() const
            {
                return mu_;
            }

        const dimensionedScalar& Cp() const
            {
                return Cp_;
            }

        const dimensionedScalar& k() const
            {
                return k_;
            }
    };

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
