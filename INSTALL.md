# Neural Creativity Server 
Hand drawn sketch recognition server using Deep Neural Networks


INSTALLATION NEURAL CREATIVITY SKETCH RECOGNITION SERVER

1) Install CMake

2) Install OpenCV


    a) clone or download zip of OpenCV and OpenCV_contrib
        cd ~/<my_working_directory>
        git clone https://github.com/Itseez/opencv.git
        git clone https://github.com/Itseez/opencv_contrib.git
    
    b) Make a directory where you want to build opencv 
        cd ~/opencv
        mkdir build
        cd build
    c) Run cmake configure 
        Set OPENCV_EXTRA_MODULES_PATH=opencv_root/opencv_contrib/modules
        Again run configure and generate 
        select appropriate generator (Tested on visual studio in windows and MacOSX)
    d) Compile project ( make all , make install for linux and mac)
        (run install project in OpenCV.sln project in windows)
    e) Set OpenCV_DIR envirment variable to path to opencv install folder 
    f) Set Path variable where opencv is installed 
    
3) Install QT 


    Download and install Install QT_SDK from https://www.qt.io/download/
    or in MacOSX brew install qt5 
    a) Set QT_DIR to /usr/local/opt/qt5 for MacOSX 
    Note QT is needed for using Websocket libarary for socket communication 
        
4) Build Neural Creativity server


    a) git clone https://github.com/keplerlab/buildYourStoryServer.git
    or in MacOSX brew install qt5 
    b) cd buildYourStoryServer/server
    c) mkdir build ; cd build
    d) cmake -D CMAKE_BUILD_TYPE=Release ..
    e) make all or open buildYourStory.sln file in visual studio and compile server application
    f) Set config.txt folder path to app caffeModel folder 
   

5) Run server 

    ./neuralCreativityServer 
    for checking output use browser to send base64 encoded image in json format or use 
    Invoke dummyClient as 
    ./dummyClient yourImage.png 
    
   
   
SKETCH RECOGNITION TRAINING   

1) Download training data from 
    wget http://cybertron.cg.tu-berlin.de/eitz/projects/classifysketch/sketches_png.zip 
Initial training data obtained from TU Berlin sketch dataset 
<i>"How do humans sketch objects?." ACM Trans. Graph. 31.4 (2012): 44-1.Eitz, Mathias, James Hays, and Marc Alexa. 
(available under Creative Commons Attribution 4.0 International License.) </i>







