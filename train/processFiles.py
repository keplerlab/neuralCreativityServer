from os import listdir
from os.path import isfile, join
import operator
import time
from shutil import copyfile
import os
import cv2
import numpy as np
from subprocess import call
import random

transformImages = bool(1)
filterclasses = bool(0)
doFlip = bool(1)
doTranslate = bool(1)
doRandomRotate = bool(1)

allDirs = []
pngDir = "png"
pngTargetDir = "png2"


print "getting all selected categories .. \n\n"

for idx,iDir in enumerate(listdir(pngDir)):
    joinedEntry = os.path.join(pngDir,iDir)
    if os.path.isdir(joinedEntry):
        #print(joinedEntry)
        
        if not filterclasses or iDir in selectedClasses:
            allDirs.append(iDir)

            

indexFileName = 'map.txt'
indexFile = open(indexFileName,'w')
indexOfClasses = dict()

allImagesListFileName = 'filelist.txt'        
allImagesListFile = open(allImagesListFileName, 'w')

avgOfImgPixels = 0.0 
noOfPixels = 0.0 

print "converting and transform images in compatible format ... \n\n"
for idx,iDir in enumerate(allDirs):
    indexFile.write( str(idx) + " " + iDir + "\n")
    indexOfClasses[iDir] = idx 
    #fileCountInIDir = 0 ;
    for file in listdir(os.path.join(pngDir,iDir)):
        imageFile = os.path.join(pngDir,iDir,file)
        if os.path.isfile(imageFile) and imageFile.endswith(".png"):
            #fileCountInIDir += 1
            if transformImages:
                img = cv2.imread(imageFile,-1)
                kernel = np.ones((3,3),np.uint8)    
                if len(img.shape) > 2:
                    b,g,r,a = cv2.split(img)
                    #cv2.imshow('a',a)
                    img = 255 - a
                    #kernel = np.ones((3,3),np.uint8)
                    img = cv2.erode(img,kernel,iterations = 1)
                    img = cv2.dilate(img,kernel,iterations = 1)
                else:
                    img = cv2.erode(img,kernel,iterations = 3)
                    img = cv2.dilate(img,kernel,iterations = 1)
                #print  str(img.shape) + "\n";

                if not os.path.exists(os.path.join(png2Dir,iDir)):
                    os.makedirs(os.path.join(png2Dir,iDir))
                saveFileName = os.path.join(png2Dir,iDir,file)
                
                #print saveFileName 
                resized_img = cv2.resize(img, (128, 128))
                #print "mean of img " + str( int(round(np.mean(resized_img))) )
                #convert into binary image 
                thresh = 250
                resized_img = cv2.threshold(resized_img, thresh, 255, cv2.THRESH_BINARY)[1]
                
                resized_img = 255-resized_img
                
                
                
                
                # get images in middle
                
                result = cv2.findNonZero(resized_img)
                if result is not None:
                    x,y,w,h = cv2.boundingRect(result);
                    #cv2.rectangle(resized_img,(x,y),(x+w,y+h),(255,255,0),2)
                    
                    #// Calculating Min bounding Box of object drawn
                    roi = resized_img[y:y+h,x:x+w]
                    height, width = resized_img.shape
                    
                    
                    segmentedItem = roi.copy()
                    #cv2.imshow("segmentedItem",segmentedItem)
                    #cv2.imshow("resized_img",resized_img)
#                    Mat segmentedItem(roi.rows, roi.cols,CV_8UC1);
#                    roi.copyTo(segmentedItem);
                
                
                    centeredImg = resized_img.copy()
                    centerX = width/2 
                    centerY = height/2 
                    centeredImg[:] = (0) 
                    #cv2.imshow("centeryempty",centeredImg)
                    #print str(w)
                    #print str(h)
                    #print str(centerX)
                    #print str(centerY)
                    
                    centeredImg[centerY - h/2 :centerY - h/2 + h,centerX - w/2 : centerX - w/2 + w] = segmentedItem[0:h,0:w]
                    #cv2.imshow("centeredImg",centeredImg)
                                   
                
                #print "mean of img " + str( int(round(np.mean(resized_img))) )
                #resized_img -= int(round(np.mean(resized_img)))
                resized_img = cv2.blur(resized_img,(3,3))
                avgOfImgPixels += np.mean(resized_img) 
                noOfPixels += 1.0 
                
                #print "mean of img so far " + str( avgOfImgPixels/ noOfPixels ) 
                #resized_img -= 130
                
                # check contour bounding box 
                if doFlip:
                    flipName = "flipped_" + file
                    saveFileNameFlipped = os.path.join(png2Dir,iDir,flipName)
                    #print saveFileNameFlipped
                    flippedImage=cv2.flip(resized_img,1)
                    cv2.imwrite(saveFileNameFlipped,flippedImage)
                    allImagesListFile.write( saveFileNameFlipped + " " + str(idx) + "\n")
                    
                cv2.imwrite(saveFileName,resized_img) 
                
                #cv2.imshow("flipeedImage",flippedImage)
                #cv2.waitKey(10000)
                
                allImagesListFile.write( os.path.join(png2Dir,iDir,file) + " " + str(idx) + "\n")
                    
            else:
                allImagesListFile.write( os.path.join(pngDir,iDir,file) + " " + str(idx) + "\n")
    #print str(fileCountInIDir)
    #categories[iDir] = fileCountInIDir

allImagesListFile.write( "\nMean of all images so far " + str( avgOfImgPixels/ noOfPixels ))
    
allImagesListFile.close()
indexFile.close()

print "mean of img so far " + str( avgOfImgPixels/ noOfPixels )

#print categories



# list train , val and test set 
trainingSet = []
validationSet = []
testSet = []

# bisect between traininng , valdiation and test set // total should be 100 

# writing train and val files ratio 
trainingPercent = 85
validationPercent = 10 
testPercent = 5
print "dividing between train , validation and test set in Ratio of 85 10 5 ....\n\n"

#shuffle classes for better performance in trainnig 
random.shuffle(allDirs)

allFileNo = 0 
for iDir in allDirs:
    # get all files list in directory
    if transformImages:
        filesListInDir = listdir(os.path.join(png2Dir,iDir))
    else:
        filesListInDir = listdir(os.path.join(pngDir,iDir))
    numOfFiles = len(filesListInDir)    
    allFileNo = numOfFiles + allFileNo
    #shuffle file list for better set
    random.shuffle(filesListInDir)
    #iterate over files and divide between training val and test set
    for jdx,file in enumerate(filesListInDir):
        if transformImages:
            imageFile = os.path.join(png2Dir,iDir,file)
        else:
            imageFile = os.path.join(pngDir,iDir,file)
        if os.path.isfile(imageFile) and imageFile.endswith(".png"):
            if(jdx < (trainingPercent * numOfFiles)/100): 
                if transformImages:
                    trainingSet.append( os.path.join(png2Dir,iDir,file) + " " + str(indexOfClasses[iDir]) + "\n")
                else:
                    trainingSet.append( os.path.join(pngDir,iDir,file) + " " + str(indexOfClasses[iDir]) + "\n")
                #print "first: " + str(jdx) 
            elif(jdx < ((trainingPercent+validationPercent) * numOfFiles)/100 ):
                if transformImages:
                    validationSet.append( os.path.join(png2Dir,iDir,file) + " " + str(indexOfClasses[iDir]) + "\n")
                else:
                    validationSet.append( os.path.join(pngDir,iDir,file) + " " + str(indexOfClasses[iDir]) + "\n")
                #print "second : " + str(jdx) 
            else:
                if transformImages:
                    testSet.append( os.path.join(png2Dir,iDir,file) + " " + str(indexOfClasses[iDir]) + "\n")
                else:
                    testSet.append( os.path.join(pngDir,iDir,file) + " " + str(indexOfClasses[iDir]) + "\n")
                #print "last: " + str(jdx)
            

random.shuffle(trainingSet)
random.shuffle(validationSet)
random.shuffle(testSet)

#print trainingSet
#print validationSet
#print testSet
print "\n\ntotal no of files = " + str(allFileNo)


# seperate files into training validation and testing set 
imageTrainFileName = 'fileListTrain.txt'
imageValidationFileName= 'fileListVal.txt'
imageFilesTestName = 'fileListTest.txt'
imageFilesTrain = open(imageTrainFileName,'w')
imageFilesValidation = open(imageValidationFileName,'w')
imageFilesTest = open(imageFilesTestName,'w')

#write into files 
for entry in trainingSet:
    imageFilesTrain.write( entry )

for entry in validationSet:
    imageFilesValidation.write( entry )
    
for entry in testSet:
    imageFilesTest.write( entry )    


imageFilesTrain.close()
imageFilesValidation.close()  
imageFilesTest.close()

trainSetDir = "./set18apr/"
archiveFileName = "training_set_archive128_20apr.zip"
call(["rm","-rf", trainSetDir])
call(["mkdir",trainSetDir])
# Make lmdb file from training validation and test set 
print "deleting old train test lmdb folder"
#call(["rm","-rf", trainSetDir+"train_data_lmdb"])
#call(["rm","-rf" ,trainSetDir+"validation_data_lmdb"])
#convert_imageset --resize_height=224 --resize_width=224 --shuffle ./keyData/ fileListTrain.txt  ./train_data_lmdb
#convert_imageset --resize_height=224 --resize_width=224 --shuffle ./keyData/ fileListVal1.txt  ./test_data_lmdb
print "Make lmdb file from training validation and test set ....\n\n"

if transformImages:
    call(["convert_imageset" ,"--gray=true", "../train/", imageTrainFileName , trainSetDir+"train_data_lmdb"])
    call(["convert_imageset", "--gray=true","../train/", imageValidationFileName  ,trainSetDir + "validation_data_lmdb"])
else:
    call(["convert_imageset","--gray=true", "--resize_height=128", "--resize_width=128",  "../train/", imageTrainFileName ,  trainSetDir+"train_data_lmdb"])
    call(["convert_imageset", "--gray=true","--resize_height=128", "--resize_width=128",  "../train/", imageValidationFileName  ,trainSetDir + "validation_data_lmdb"])

print "moving all misc files in train set dir "
call(["mv",imageTrainFileName,trainSetDir])
call(["mv",imageValidationFileName,trainSetDir])
call(["mv",imageFilesTestName,trainSetDir])
call(["mv",indexFileName,trainSetDir])
call(["mv",allImagesListFileName,trainSetDir])


print "copying caffe trainng files"
caffeTrainFileName = 'buildYourStoryNet128/train_val.prototxt'
deployFileName= 'buildYourStoryNet128/deploy.prototxt'
solverName = 'buildYourStoryNet128/solver.prototxt'
quickSolverName = 'buildYourStoryNet128/quick_solver.prototxt'
resumeTrainFile = 'buildYourStoryNet128/resumetrain.sh'
caffeRunFile = 'buildYourStoryNet128/train.sh'

call(["cp",caffeTrainFileName,trainSetDir])
call(["cp",deployFileName,trainSetDir])
call(["cp",solverName,trainSetDir])
call(["cp",quickSolverName,trainSetDir])
call(["cp",caffeRunFile,trainSetDir])
call(["cp",resumeTrainFile,trainSetDir])



print "\ncompress training set folder into: " + archiveFileName
call(["zip" , "-r", "-X",  archiveFileName, trainSetDir])


