cmake_minimum_required(VERSION 2.2)
find_package(OpenCV REQUIRED)
SET(PROJECT_SOURCE_DIR ".")

set(CMAKE_CXX_STANDARD 11)

#set (CMAKE_CXX_FLAGS "-std=c++11")

project( bysServer )


set(CMAKE_PREFIX_PATH "$ENV{QT_DIR}/lib/cmake")

SET(PROJECT_INC_DIR "${PROJECT_SOURCE_DIR}/src")
SET(PROJECT_SRC_DIR "${PROJECT_SOURCE_DIR}/src")



# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
#core websockets
find_package(Qt5Core)
find_package(Qt5WebSockets)


SET(Headers
"${PROJECT_INC_DIR}/helper.h"
)

SET(Dependencies
"${PROJECT_SRC_DIR}/helper.cpp"
"${PROJECT_SRC_DIR}/jsoncpp.cpp"

)
  
ADD_EXECUTABLE(neuralCreativityServer ${PROJECT_SRC_DIR}/neuralCreativityMain.cpp ${PROJECT_INC_DIR}/socketServer.h ${PROJECT_SRC_DIR}/socketServer.cpp ${Headers} ${Dependencies} )
TARGET_LINK_LIBRARIES(neuralCreativityServer ${OpenCV_LIBS} Qt5::Core Qt5::WebSockets )



ADD_EXECUTABLE(dummyClient ${PROJECT_SRC_DIR}/socketClient.h ${PROJECT_SRC_DIR}/socketClient.cpp ${PROJECT_SRC_DIR}/dummyClient.cpp ${Headers} ${Dependencies})
TARGET_LINK_LIBRARIES(dummyClient ${OpenCV_LIBS}  Qt5::Core Qt5::WebSockets )


