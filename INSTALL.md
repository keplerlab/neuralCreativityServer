# Neural Creativity Server 
Hand drawn sketch recognition server using Deep Neural Networks


INSTALLATION Neural Creativity SKETCH RECOGNITION SERVER

1) Install CMake
2) Install OpenCV
    Checkout latest 3.0 OpenCV with OpenCV_Contrib and build from source 
    a) Start cmake-gui
    b) clone or download zip of OpenCV and OpenCV_contrib
        cd ~/<my_working_directory>
        git clone https://github.com/Itseez/opencv.git
        git clone https://github.com/Itseez/opencv_contrib.git
    
    c) Make a directory where you want to build opencv 
        cd ~/opencv
        mkdir build
        cd build
    d) Run cmake configure 
        Set OPENCV_EXTRA_MODULES_PATH=opencv_root/opencv_contrib/modules
        Again run configure and generate 
        select appropriate generator (Tested on visual studio in windows and MacOSX)
    c) Compile project ( make all , make install for linux and mac)
        (run install project in OpenCV.sln project in windows)
    d) Set OpenCV_DIR envirment variable to path to opencv install folder 
    e) Set Path variable where opencv is installed 
    
3) Install QT 
    Download and install Install QT_SDK from https://www.qt.io/download/
    or in MacOSX brew install qt5 
    a) Set QT_DIR to /usr/local/opt/qt5 for MacOSX 
    Note QT is needed for using Websocket libarary for socket communication 
        
3) Build BuildYourStory server application
   a)  git clone https://github.com/keplerlab/buildYourStoryServer.git
   b)  cd buildYourStoryServer/server
   c)  mkdir build ; cd build
   d)  cmake -D CMAKE_BUILD_TYPE=Release ..
   e)  make all or open buildYourStory.sln file in visual studio and compile server application
   f)  Set config.txt folder path to app caffeModel folder 
   
4) Run server ./neuralCreativityServer 
    for checking output use browser to send base64 encoded image in json format or use 
    Invoke dummyClient as 
    ./dummyClient yourImage.png 
    
   
   
   
SKETCH RECOGNITION TRAINING   

1) Download training data from 
Initial training data can be obtained using TU Berlin sketch dataset 
<i>"How do humans sketch objects?." ACM Trans. Graph. 31.4 (2012): 44-1.Eitz, Mathias, James Hays, and Marc Alexa. 
(available under Creative Commons Attribution 4.0 International License.) 
Link http://cybertron.cg.tu-berlin.de/eitz/projects/classifysketch/sketches_png.zip </i>






