COMPILING THE SOURCE CODE
-------------------------

Assuming OpenFOAM version 4.0 is installed. To compile the library and application source code, go to:
"pemfcModels-4.0/multiPhaseNonIsothermal" directory and run the "Allwmake" script.

To do this, type "./Allwmake" at the command terminal. 
This should generate the shared object library "libMultiphasePEMFC.so" in the "$FOAM_USER_LIBBIN" directory 
and application executable "pemfcMultiphaseNonIsothermalSolver" in the "$FOAM_USER_APPBIN" directory. 

A "lnInclude/" directory, containing links to all the lib class files, will appear in the "lib/" directory.