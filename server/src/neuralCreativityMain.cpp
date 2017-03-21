/**
Purpose: Sketch Recongnition server 

*/     
#include <QtCore/QCoreApplication>
#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <thread>
#include <chrono>

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;
using namespace cv::dnn;

#include <fstream>
#include <iostream>
#include <cstdlib>
using namespace std;

#include "json/json.h"


#include "socketServer.h"

#include "helper.h"


volatile bool sketchRecivedState;

std::string modelFolderName ;

std::string modelBinName ;
std::string saveFolderName;

int currentSaveNum ;

QString base64_encode(QString string){
    QByteArray ba;
    ba.append(string);
    return ba.toBase64();
}


/* Threading control */
volatile bool g_running = false;
volatile bool g_stop = true;

int call_from_thread(SocketServer* server);

int call_from_thread2(SocketServer* server)
{
    String imageFile = "11392.png";
    Mat img = imread(imageFile,-1);
    
    if(! img.data || img.empty())                              // Check for invalid input
    {
        std::cerr << "Can't read image from the file: " << imageFile << std::endl;
        
        sketchRecivedState = false ;
        return -1 ;
    }
    cerr << "Displaying image before Calculating" << endl ;
    imwrite("myimg.png",img);
   // cv::waitKey(5000) ;
    
    
}

int infinityThread(SocketServer* server)
{
    
    while (1)
    {
        //sketchRecivedState = true ;
        
        if(sketchRecivedState)
        {
            cout << "Calculating Sketech association" << endl ;
            call_from_thread(server);
        
            cv::destroyAllWindows();
        
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        
        cout << "Waiting for image ... " << endl;
        
        
    }
    
    return 0;
}



/* Find best class for the blob (i. e. class with maximal probability) */
void getMaxClass(dnn::Blob &probBlob, int *classId, double *classProb)
{
    Mat probMat = probBlob.matRefConst().reshape(1, 1); //reshape the blob to 1x1000 matrix
    Point classNumber;

    minMaxLoc(probMat, NULL, classProb, NULL, &classNumber);
    *classId = classNumber.x;
}

std::vector<String> readClassNames()
{
    std::string filename = modelFolderName + "map.txt" ;
    cout << filename ;
    std::vector<String> classNames;

    std::ifstream fp(filename);
    if (!fp.is_open())
    {
        std::cerr << "File with classes labels not found: " << filename << std::endl;
        exit(-1);
    }

    std::string name;
    while (!fp.eof())
    {
        std::getline(fp, name);
        if (name.length())
            classNames.push_back( name.substr(name.find(' ')+1) );
    }

    fp.close();
    return classNames;
}


void preProcessImage128(cv::Mat& src,cv::Mat& dst)
{
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    auto milliseconds_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();


    ostringstream ostrm , ostrm2,xyFileName ;
    ostrm << saveFolderName << milliseconds_since_epoch << ".png" ; 

    cv::imwrite(ostrm.str() ,src) ;
    
    if(src.channels() == 1)
    {
        dst = src.clone() ;
    }
    else if(src.channels() == 3)
    { 
        cv::cvtColor(src, dst, CV_BGR2GRAY);
    }
    else if(src.channels() == 4)
    {
        std::vector<cv::Mat> layers;
        cv::split(src, layers);
        dst = layers[3];
        dst = 255 - dst ;
        //kernel = np.ones((3,3),np.uint8);
        cv::erode(dst, dst, Mat(), Point(-1, -1),1);
       // cv::imwrite("dst.png",dst) ;
        
    }
    
    if(dst.rows != 128 || dst.cols != 128)
    {
                        
        cout << "converting into 128 image" << endl ;
        cv::erode(dst, dst, Mat(), Point(-1, -1),3);
        cv::dilate(dst, dst, Mat(), Point(-1, -1),1);
        
        cv::Size newsize(128,128);
        cv::Mat resized_img;
        cv::resize(dst, resized_img, newsize );

        double thresh = 250;
        double maxValue = 255; 
        cv::threshold(resized_img,resized_img, thresh, maxValue, THRESH_BINARY);
        
		resized_img = 255 - resized_img;

		// Calculating Min bounding Box of object drawn
        Mat Points;
        findNonZero(resized_img,Points);
        Rect Min_Rect=boundingRect(Points);
        
		// Get the roi in the input image according to the mask
		Mat roi(resized_img(Min_Rect));

		//Vec3b backgroundColor(0); // black
		Mat segmentedItem(roi.rows, roi.cols,CV_8UC1);
		roi.copyTo(segmentedItem);


		// copy min bounding box image to center 
		Point center(resized_img.cols / 2, resized_img.rows / 2);
		
		Mat centeredImg(resized_img.rows, resized_img.cols,CV_8UC1);
		centeredImg = 0;
		Rect centerBox(center.x - Min_Rect.width / 2, center.y - Min_Rect.height / 2, Min_Rect.width, Min_Rect.height);

		//segmentedItem.copyTo(centeredImg(centerBox));

		
		//cv::rectangle(centeredImg, centerBox, Scalar(255), 2);
		//cv::imshow("oldCenteredImg", centeredImg);
		//cv::waitKey(2000);
		// check 
		float smallCheckRatio = 3; 
		if ( (centerBox.width <= (int)(centeredImg.cols / smallCheckRatio)) && (centerBox.height <= (int)(centeredImg.rows / smallCheckRatio)))
		{
			cout << "too small image resize needed " << endl;
			// New size 
			double correctionRatioW = centeredImg.cols / (double)(centerBox.width) ;
			double correctionRatioH = centeredImg.rows /(double) (centerBox.height);
			// take smaller of two correction ratio
			double correctionRatio = (correctionRatioW < correctionRatioH) ? correctionRatioW : correctionRatioH;
			correctionRatio = correctionRatio / 2.0; // only need 2x correction
			cout << "scaling image by W " << correctionRatioW << " H "  << correctionRatioH << " F " <<correctionRatio << endl;
			cv::Rect newCenterBox;
			newCenterBox.width = centerBox.width * correctionRatio;
			newCenterBox.height = centerBox.height * correctionRatio;

			newCenterBox.x = center.x - newCenterBox.width / 2;
			newCenterBox.y = center.y - newCenterBox.height / 2;
			
			Mat dstSegmentedItem;
			cv::resize(segmentedItem, dstSegmentedItem, cv::Size(newCenterBox.width, newCenterBox.height));
			//segmentedItem;
			dstSegmentedItem.copyTo(centeredImg(newCenterBox));
		}
		else
		{
			segmentedItem.copyTo(centeredImg(centerBox));
			blur(centeredImg, centeredImg, Size(3, 3));
		}
        //cv::imshow("resizedImg",resized_img) ;
		//cv::imshow("centeredImg", centeredImg);
        //cv::waitKey(2000) ;
		
		dst = centeredImg;
        
    }
    
    
     ostrm2 << saveFolderName << "t_" << milliseconds_since_epoch << ".png" ; 
     cv::imwrite(ostrm2.str() ,dst) ;


	 xyFileName << saveFolderName << "xyinfo_"  << milliseconds_since_epoch << ".txt";

	 std::ifstream  xyTestFile("xytest.txt", std::ios::binary);
	 std::ofstream  dstXYFile(xyFileName.str(), std::ios::binary);

	 dstXYFile << xyTestFile.rdbuf();

    
    
}
int call_from_thread(SocketServer* server)
{

    

    String modelTxt = modelFolderName + "deploy.prototxt";
    String modelBin = modelFolderName + modelBinName ;
    
    cout << "\nmodelTxt file: " << modelTxt << endl ; 
    cout << "\nmodelBin file: " << modelBin << endl ;
    
 //   String modelTxt = "../oldBVLC/bvlc_googlenet.prototxt";
 //   String modelBin = "../oldBVLC/bvlc_googlenet.caffemodel" ;
    
    //String modelBin = "../bvlc_googlenet.caffemodel";
    String imageFile = "test.png";

    //! [Create the importer of Caffe model]
    Ptr<dnn::Importer> importer;
    try                                     //Try to import Caffe GoogleNet model
    {
        importer = dnn::createCaffeImporter(modelTxt, modelBin);
    }
    catch (const cv::Exception &err)        //Importer can throw errors, we will catch them
    {
        std::cerr << "Error in importing " << err.msg << std::endl;
        
    }
    //! [Create the importer of Caffe model]

    if (!importer)
    {
        std::cerr << "Can't load network by using the following files: " << std::endl;
        std::cerr << "prototxt:   " << modelTxt << std::endl;
        std::cerr << "caffemodel: " << modelBin << std::endl;
        exit(-1);
    }

     //! [Prepare blob]
    Mat img = imread(imageFile,-1);
    
    
    if(! img.data || img.empty())                              // Check for invalid input
    {
        std::cerr << "Can't read image from the file: " << imageFile << std::endl;
        
        sketchRecivedState = false ;
        return -1 ;
    }
    
    Mat img2 ;
    preProcessImage128(img,img2);
    
   // cerr << "Displaying image before Calculating" << endl ;
    //imshow("testImage",img);
    //cv::waitKey(5000) ;
    
    //! [Initialize network]
    dnn::Net net;
    importer->populateNet(net);
    importer.release();                     //We don't need importer anymore
    //! [Initialize network]

      
    //Convert Mat to dnn::Blob image batch
    dnn::Blob inputBlob = dnn::Blob::fromImages(img2);
    //dnn::Blob inputBlob = dnn::Blob(img2);  // Old method in opencv 3.0 
    //! [Prepare blob]

    //! [Set input blob]
    net.setBlob(".data", inputBlob);        //set the network input
    //! [Set input blob]

    //! [Make forward pass]
    net.forward();                          //compute output
    //! [Make forward pass]

    //! [Gather output]
    dnn::Blob prob = net.getBlob("prob");   //gather output of "prob" layer

    int classId;
    double classProb;
    getMaxClass(prob, &classId, &classProb);//find the best class
    //! [Gather output]

    //! [Print results]
    std::vector<String> classNames = readClassNames();
    std::cout << "Best class: #" << classId << " '" << classNames.at(classId) << "'" << std::endl;
    std::cout << "Probability: " << classProb * 100 << "%" << std::endl;
    //! [Print results]

    
    Json::Value sketchEvent;
    sketchEvent["sketchEvent"] = "bestClass";
    sketchEvent["Value"] = classId ;
    sketchEvent["Confidence"] = classProb*100 ;
    sketchEvent["Name"] = classNames.at(classId).c_str() ;
    
    
    std::string sketchEventStr = sketchEvent.toStyledString();
    QString sketchStr = QString::fromUtf8(sketchEventStr.c_str());
    emit server->sendMessage(sketchStr);

    
    sketchRecivedState = false ;
    
    
    return 0;
} //main



int main(int argc, char *argv[])
{
    //freopen("erroLog.txt", "w", stderr);
    
    std::string configFileName="../config.txt"; 
	std::ifstream infile(configFileName); 
	
	if (infile.is_open())
	{
		std::string tmp1 ,tmp2,tmp3   ; 
		infile >> tmp1 >> modelFolderName ; 
        infile >> tmp2 >> modelBinName ;
        infile >> tmp3 >> saveFolderName;
		infile.close() ;
	}
    else
    {
        std::cerr << "\n\nCannot open config file " << configFileName << endl ;
		return -1;
    }

	cout << "Model Folder Name : " << modelFolderName << endl  ; 
    cout << "Model Bin Name: " << modelBinName << endl  ; 
    
    infile.close();
    
    
    
    
    QCoreApplication app(argc, argv);
    
    SocketServer *server = new SocketServer(9006);
    
    std::thread t1(infinityThread, server);
   // t1.detach();

    app.exec();
    
    t1.join();

    return 0;
    //	fclose(fp);
    
    
}

