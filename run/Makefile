################################################################################
All: mesh parprep run reconstruct view
mesh: bmesh patchBC regionSets regionInterface regionMesh control
###############
clean:
	(rm -rf *.log [1-9]*);
clear:
	(./Allclean)
###############
bmesh:
	(cp -rf ./system/controlDict.mesh ./system/controlDict);
	(blockMesh);
patchBC:
	(rm -rf ./constant/polyMesh/sets);
	(topoSet -dict ./config/faceSets.topoSetDict);
	(rm -rf ./constant/polyMesh/sets/*_old);
	(topoSet -dict ./config/faceSetsCathodeFluid.topoSetDict);
	(rm -rf ./constant/polyMesh/sets/*_old);
	(topoSet -dict ./config/faceSetsAnodeFluid.topoSetDict);
	(rm -rf ./constant/polyMesh/sets/*_old);
	(setsToZones -noFlipMap);
	(createPatch -overwrite);
regionSets:
	(rm -rf constant/polyMesh/sets constant/polyMesh/*Zones);
	(topoSet -dict ./config/topoSetDict);
	(rm -rf constant/polyMesh/sets/*_old);
	#(setsToZones -noFlipMap);
regionInterface:
	(topoSet -dict ./config/faceSetsIFaces.topoSetDict);
	(rm -rf constant/polyMesh/sets/*_old);
	(rm constant/polyMesh/sets/[abc]Faces);
	#(setsToZones -noFlipMap);
regionMesh:
	(splitMeshRegions -cellZonesOnly);
	(cp -r 0.001/* ./constant);
	(rm -rf 0.001);
	(topoSet -region cathodeFluid -dict config/cathodeFluid.topoSetDict);
	#(setsToZones -noFlipMap -region cathodeFluid);
	(topoSet -region electrolyte -dict config/electrolyte.topoSetDict);
	#(setsToZones -noFlipMap -region electrolyte);
	(topoSet -region anodeFluid -dict config/anodeFluid.topoSetDict);
	#(setsToZones -noFlipMap -region anodeFluid);		
	(rm -f constant/[aacce]*/[kpTU] constant/[aacce]*/rho);
	(rm -f constant/[aacce]*/Y* constant/[aacce]*/diff*);
control:
	(cp -rf ./system/controlDict.run ./system/controlDict);
#############
parprep:
	(./parprep.csh | tee log.parprep);
#############
run:
	( pemfcMultiphaseNonIsothermalSolver | tee log.run );
	
###(./prun | tee log.prun);
srun:
	(pemfcMultiphaseNonIsothermalSolver | tee log.srun);
#############
reconstruct:
	(reconstructPar);
	(reconstructPar -region cbp);
	(reconstructPar -region cathodeFluid);
	(reconstructPar -region electrolyte);
	(reconstructPar -region anodeFluid);
	(reconstructPar -region abp);
#############
view:
	(foamToVTK -ascii -latestTime);
	(foamToVTK -ascii -latestTime -region cbp);
	(foamToVTK -ascii -latestTime -region cathodeFluid);
	(foamToVTK -ascii -latestTime -region electrolyte);
	(foamToVTK -ascii -latestTime -region anodeFluid);
	(foamToVTK -ascii -latestTime -region abp);
viewAll:
	(foamToVTK -ascii);
	(foamToVTK -ascii -region cbp);
	(foamToVTK -ascii -region cathodeFluid);
	(foamToVTK -ascii -region electrolyte);
	(foamToVTK -ascii -region anodeFluid);
	(foamToVTK -ascii -region abp);
################################################################################
