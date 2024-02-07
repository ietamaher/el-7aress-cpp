cmake_minimum_required(VERSION 3.20)

project(el-7ress-cpp)

# Enable AUTOMOC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


if(NOT fastcdr_FOUND)
    find_package(fastcdr 2 REQUIRED)
endif()

if(NOT fastrtps_FOUND)
    find_package(fastrtps 2.12 REQUIRED)
endif()

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(Qt5 COMPONENTS Widgets OpenGL REQUIRED)
find_package(OpenGL REQUIRED)  # Add this line

find_package(jetson-utils)

find_package(jetson-inference)

# CUDA is required
find_package(CUDA)

# add directory for libnvbuf-utils to program
link_directories(/usr/lib/aarch64-linux-gnu/tegra)
include_directories(/usr/local/cuda/include)

# Set C++11
include(CheckCXXCompilerFlag)
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG OR
        CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    check_cxx_compiler_flag(-std=c++11 SUPPORTS_CXX11)
    if(SUPPORTS_CXX11)
        add_compile_options(-std=c++11)
    else()
        message(FATAL_ERROR "Compiler doesn't support C++11")
    endif()
endif()


set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

add_executable(el-7ress-cpp
     
    ${PROJECT_SOURCES}
    videowidget.h videowidget.cpp
    motorsubscriberthread.h motorsubscriberthread.cpp
    plcsubscriberthread.h plcsubscriberthread.cpp    
    Response_msg.h
    motorddssubscriber.h motorddssubscriber.cpp
    plcddssubscriber.h plcddssubscriber.cpp    
    JoystickDataPubSubTypes.h
    JoystickData.h
    joyddssubscriber.h joyddssubscriber.cpp
    Response_msgPubSubTypes.h
    generated/Query_msg.cxx generated/Query_msg.i generated/Query_msgCdrAux.hpp generated/Query_msgCdrAux.ipp generated/Query_msgPubSubTypes.cxx generated/Query_msgPubSubTypes.i generated/Response_msg.cxx generated/Response_msg.i generated/Response_msgCdrAux.hpp generated/Response_msgCdrAux.ipp generated/Response_msgPubSubTypes.cxx generated/Response_msgPubSubTypes.i   generated/State_msgCdrAux.hpp generated/State_msgCdrAux.ipp
    generated/JoystickData.cxx generated/JoystickData.i generated/JoystickDataCdrAux.hpp generated/JoystickDataCdrAux.ipp generated/JoystickDataPubSubTypes.cxx generated/JoystickDataPubSubTypes.i 
    motorddspublisher.h motorddspublisher.cpp
    plcddspublisher.h plcddspublisher.cpp    
    Query_msgPubSubTypes.h
    actiondata.h
    plcactiondata.h    
    joysubscriberthread.h joysubscriberthread.cpp
    plcpublisherthread.h plcpublisherthread.cpp
    motorpublisherthread.h motorpublisherthread.cpp
    videocapture.h videocapture.cpp
    videocapture_cuda.h videocapture_cuda.cpp
    videoglwidget.h videoglwidget.cpp
    videoglwidget_gl.h videoglwidget_gl.cpp

)


target_link_libraries(el-7ress-cpp ${OpenCV_LIBS} Qt5::Widgets Qt5::OpenGL OpenGL::GL fastrtps fastcdr jetson-inference jetson-utils cudart)

 
