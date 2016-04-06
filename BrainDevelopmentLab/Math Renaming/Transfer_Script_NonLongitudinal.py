#Transfer_Script.py 
#I'll try and make life easier by copying all the folders for each kind to my own folder 
#This will work with a file that has all of the longitudinal kids 
#It will probably take a long long long long LONG time
#I will leave it running on a screen, probably 

import os 
import shutil

#Our root directories 
srcRoot = "/scratch/projects/BoothLab/math/raw/"
destRoot = "/scratch/projects/BoothLab/math/Eduardo/"
nonlongitudinal = "/scratch/projects/BoothLab/math/Eduardo/scripts/Nonlongitudinal.txt"

#Get all the nonlongitudinal kids we need 
with open(nonlongitudinal) as f:
    lines = f.read().splitlines()

#Copy all elements!
for element in lines:
	dest = destRoot + element + "/"
	for name in os.listdir(srcRoot):
		if os.path.isdir(os.path.join(srcRoot, name)):
			srcFolder = os.path.join(srcRoot, name)
			if element in srcFolder:
				dstFolder = os.path.join(dest, name) 
				print dstFolder
				shutil.copytree(srcFolder, dstFolder) 
				with open("outputl", "w") as text_file:
    					text_file.write("Copied: %s" % name)

