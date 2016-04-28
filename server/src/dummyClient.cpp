/**
    Purpose: DummyClient for sketch Recognition server 
*/
#include <QtCore/QCoreApplication>
#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <thread>
#include <chrono>

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>


#include "socketclient.h"

#include "helper.h"
#include "json/json.h"


using namespace std ;


string inFile = "";


int call_from_thread(SocketClient* server);

int infinityThread(SocketClient* server)
{
    while (1)
    {
        
        cout << "\n\nSending image \n\n" << endl;
        
        call_from_thread(server);
        
        
        cv::destroyAllWindows();
        // wait for device to connect
        std::this_thread::sleep_for(std::chrono::seconds(5));
        
        
    }
    
    return 0;
}

int call_from_thread(SocketClient* server)
{
    ifstream fin;
    
    fin.open(inFile, ios::binary);
    
    if(! fin.is_open() )
    {
        cerr << "error Opening file " << inFile << endl ;
        return -1 ;
    }
    //ifstream fin("printImage.jpeg", ios::binary);
    //ofstream fout("test.jpg", ios::binary);//for testing purpose, to see if the string is a right copy
    ostringstream ostrm;
    ostrm << fin.rdbuf();
    
    string data(ostrm.str());
    
    std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(data.c_str()), data.length());
    //std::string decoded = base64_decode(encoded);
    //std::cout << "encoded: " << encoded << std::endl;
    //std::cout << "decoded: " << decoded << std::endl;
    //out << count << endl;//ouput 60! Definitely not the right size
    //fout << data;//only 60 bytes
    
    
    //QString str = QString::fromUtf8(encoded.c_str());
    
    
    Json::Value sketchEvent;
    sketchEvent["sketchEvent"] = "image";
    sketchEvent["Value"] = encoded.c_str();
    
    std::string imageString = sketchEvent.toStyledString();
    QString imageStr = QString::fromUtf8(imageString.c_str());
    emit server->sendMessage(imageStr);
    
    return 0 ;
    
    
}



int main(int argc, char *argv[])
{
    //freopen("erroLog.txt", "w", stderr);
    
    if( argc == 2 ) {
        inFile = argv[1];
        
    }
    else {
        cerr << "Usage: " << argv[0] << " image.jpeg" << endl;
        return -1;
    }
    
    
    
    QCoreApplication app(argc, argv);
    
    
    SocketClient sketchAppDummy(QUrl(QStringLiteral("ws://localhost:9006")), ClientType::sketchApp);

    
    std::thread t1(infinityThread, &sketchAppDummy);
    
    app.exec();
    
    
    t1.join();
    return 0;
    //	fclose(fp);
    
    
}