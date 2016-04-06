#Raw_Data_Rename.py
#Written in Python2 for a Linux bash environment 
#Made for UT Austin's Brain Development Lab 
#Most recent change: 03/09/2016

#This file will rename the early UTdata to the open FMRI standard
#It will automatically copy all files. This will probably take a long time 
#It will eventually copy files like T1_rej 
#TO DO: copy other files, analysis, deal with fieldmap, not copy DTI 

import shutil 
import os
import distutils.core

verifier = 0
no_copy = ["1Phonrun1", "2Phonrun2", "3Gramrun1", "4Gramrun2", "5Semrun1", "6Semrun2", "7Plausrun1", "8Plausrun2"]

#This function finds the folders for each subject we get 
#It then activates the functions to obtain its data 
#It will eventually call a function to start the copying
def findFolders(subject, folder):
	do = 0
	if(len(subject) == 4):
		do = 1
		print "\033[1;34mCopying data for Subject: %s\033[1;37m" %subject
	#We check all the folders in inside our main folder 
	for name in os.listdir(folder):
		if os.path.isdir(os.path.join(folder, name)):
			test = getTest(name)
			if (test!='E'): #This means there are tests, so we should copy 
				run = getRun(name)
				print "Copying Test: %s, Run: %d for Subject: %s " %(test, run, subject)
				copyFiles(subject, run, test)
	#The folder we are checking is a subject, so we need to copy MPRAGE and fieldamp
	if(do):
		copyMPRAGE(subject)
		for name in os.listdir(folder):
			if os.path.isdir(os.path.join(folder, name)):
				copyFolder(folder, name, subject)
				copyRootFiles(folder, subject)
		do = 0
        print "\n"
		

#This function obtains the test we are renaming 
#If it is not a test, it returns an E for Error
def getTest(folder):
	number = folder[:1]
	if(number=='1' or number=='2'):
			return "Phon"
	elif(number=='4' or number=='3'):
			return "Gram"
	elif(number=='5' or number=='6'):
			return "Sem"
	elif(number=='7' or number=='8'):
			return "Plaus"
	else:
			return "E"

#This function gets which run we are dealing with. Very simple 
def getRun(folder):
	number = folder[:1]
	int_num = int(number)
	if (int_num % 2 == 0):
		return 2
	else:
		return 1

#This function gets the number of the folder under Maggie's standardization 
def getNum(test, run):
	if(test == "Phon"):
		number = 1
	elif(test == "Gram"):
		number = 3
	elif(test == "Sem"):
		number = 5
	elif(test == "Plaus"):
		number = 7
	if(run==2):
		number = number +1
	return number

#We implement the same kind of copying we use on the renaming script
#It assumes all the 4d.nii's are collapsed and there's only one version for each piece of data
def copyFiles(subject, run, test):
	global root
	
	number = getNum(test, run)
	folder_name = str(number)+test+"run"+str(run)
	source = root+subject+"/"+folder_name+"/"  
	folder_name = "T1_"+folder_name 
	file_name = "task-"+test+"_ses-T1"+"_run-0"+str(run)+"_bold"
	destination = root+"renamed/"+subject+"/func/"+folder_name+"/"
	#Create the destination directory 
	if not os.path.exists(destination):
		os.makedirs(destination)

	for root, dirs, files in os.walk(source):
		files.sort()	
		for filename in files: 
			if(filename == "4D.nii"):
				source_file = source + filename 
				string_trunc = ".nii" 		
				dest_file = destination+file_name+string_trunc
				shutil.copyfile(source_file, dest_file)

	#We count the total number of copies 
	for root, dirs, files in os.walk(destination):
		global verifier
		verifier = 0
		for filename in files:
			verifier = verifier + 1
	
	#Print final message  
	print "\033[1;32mSuccesfuly copied %d file(s) for Subject:%s Test:%s Run:%d.\033[1;37m" %(verifier, subject, test, run)
	root = "/scratch/projects/BoothLab/early/UTdata/raw/"

#This part of the script copies the MPRAGE files of a subject
#It counts the total number of files, though it should be one 
def copyMPRAGE(subject):
	global root
	
	print "Copying MPRAGE files."
	#We first create our strings for source and destination  
	folder_name = "MPRAGE/"
	source = root + subject + "/" + folder_name
	destination = root+"renamed/"+subject+"/anat/"+"T1_MPRAGE/"
	file_name = "ses-T1_T1w.nii"
	final = destination + file_name 

	#We make the folder if it doesn't exist 
	if not os.path.exists(destination):
		os.makedirs(destination)
	
	#We copy! 
	for root, dirs, files in os.walk(source):
		for filename in files:
			if(filename == "MPRAGE.nii"):	
				source_file= source + filename 
				shutil.copyfile(source_file, final) 
	
	#Count the total number of files copied 
	for root, dirs, files in os.walk(destination):
		verifier = 0
		for filename in files:
			verifier = verifier + 1
	
	#We mark it finished 
	print "\033[1;32mFinished copying %d MPRAGE files for subject %s.\033[1;37m" %(verifier, subject)  
	root = "/scratch/projects/BoothLab/early/UTdata/raw/"
	
#A function to copy stuff that isn't our normal data
#It copies folders such as T1rej, and analysis folders
#It does not copy the DTI files, and puts the fieldmap in the anat folder 
def copyFolder(folder, name, subject):
	global root
	
	#Don't copy anything from the DTI folder 
	if (name == 'DTI'):
		return
	
	#Don't copy our functional tests (we already have them!)
	elif (name in no_copy):
		return
	
	elif (name[:6]=='MPRAGE'):
		return
	
	#Copy the fieldmap 
	elif(name == 'fieldmap'):
		print "Copying fieldmap for Subject %s" %(subject)
		source = os.path.join(folder, name)
		destination = root+"renamed/"+subject+"/anat/T1_fieldmap/"
		name = "ses-T1_fieldmap.nii"
		if not os.path.exists(destination):
			os.makedirs(destination)
		for root, dirs, files in os.walk(source):
			for filename in files:
				source_file = source + "/" + filename
				dest_file = destination + name 
                shutil.copyfile(source_file, dest_file)
		root = "/scratch/projects/BoothLab/early/UTdata/raw/"
		return

	elif(name == 'T1rej'):
		print "Copying T1rej"
		destination = root+"renamed/"+subject+"/T1rej/"
		source = os.path.join(folder, name)
		shutil.copytree(source, destination)
		root = "/scratch/projects/BoothLab/early/UTdata/raw/"			
		return

	else:
		return
		
#This function copies the rest of the files in the subject's root
#It is useless, as we shouldn't copy root files 
def copyRootFiles(folder, subject):
	global root
	source = folder
	destination = root+"renamed/"+subject+"/"
	for root, dirs, files in os.walk(source):
		for filename in files: 
			if(filename == subject):
				source_file= source + filename
				dest_file = destination + filename + "_txt_log"
				if os.path.isfile(source_file):
					shutil.copyfile(source_file, dest_file)
			if(filename == subject+"_txt_log"):
				source_file= source + filename
                                dest_file = destination + filename
                                if os.path.isfile(source_file):
                                        shutil.copyfile(source_file, dest_file)
	root = "/scratch/projects/BoothLab/early/UTdata/raw/"
	
#Our main function finds all of our subject folders 
def main():
	for name in os.listdir(root):
		if (os.path.isdir(os.path.join(root, name)) and (name !="renamed")):	
			subject_folder = root+"/"+name+"/"
			findFolders(name, subject_folder)

#This is our main root for all operations. It should end in the raw folder
root = "/scratch/projects/BoothLab/early/UTdata/raw/"

main()


#Credits: Eduardo Zueck Garces - March 2016




