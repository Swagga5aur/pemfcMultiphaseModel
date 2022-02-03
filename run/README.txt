RUNNING A SIMULATION
--------------------
Assuming OpenFOAM version 4.0 is installed and the "pemfcMultiphaseNonIsothermalSolver" application already compiled,
the command "make run" will run the executable from the terminal using the constructed case data.

The model can also be run by typing the executable name and the output that is normally directed to Standard Out can be redirected to a file:
"pemfcMultiphaseNonIsothermalSolver | tee log.run"

After the model has run to completion, VTK files for visualization, e.g. with ParaView, can be created using the Makefile file
by typing "make view" at the command line to generate VTK files for the last output step 
and "make viewAll" to get VTK files for all output directories. 
Otherwise, the "paraFoam" utility supplied with OpenFOAM can be invoked directly for visualization of results with ParaView.