#ifndef VIDEOCAPTURE_CUDA_H
#define VIDEOCAPTURE_CUDA_H

#include <QThread>
#include <QImage>

#include <jetson-utils/videoSource.h>
#include <jetson-utils/videoOutput.h>
#include <jetson-utils/cudaUtility.h>
#include <jetson-utils/cudaMappedMemory.h>
#include <jetson-utils/cudaNormalize.h>
#include <jetson-utils/cudaResize.h>
#include <jetson-utils/cudaCrop.h>
#include <jetson-utils/cudaDraw.h>
#include <jetson-utils/imageIO.h>
#include <jetson-utils/cudaFont.h> // For drawing text if needed
#include <jetson-inference/detectNet.h>
#include <jetson-inference/objectTrackerIOU.h>

#include <signal.h>
#include <stdio.h>

#undef Log 

class CaptureVideoCuda : public QThread {
    Q_OBJECT

    //static int signalFd[2]; // Pipe file descriptors
    
public:
    explicit CaptureVideoCuda(QObject *parent = nullptr); // Constructor with optional parent
    ~CaptureVideoCuda(); // Destructor

protected:
    void run() override; // Override the QThread's run() method

private:
    //static void SignalHandler(int signo);
    //void SetupSignalHandler();
    bool running;
    bool enableDetection;
    bool enableTracking;
 
    //void SetupSignalHandler();
    bool InitializeVideoSource();
    //bool InitializeVideoOutput(const commandLine& cmdLine);
    bool InitializeDetectNet();
    QImage ConvertToQImage(uchar3* image, int width, int height);
    videoSource* input = nullptr;
    videoOutput* output = nullptr;
    detectNet* net = nullptr;

    void DetectAndRender(uchar3* image);

    void stop();

signals:
    void frameReady(QImage frame); // Signal to emit when a new frame is ready
    void ErrorOccurred(QString error); // Signal to emit in case of error
    //void ShutdownRequested(); // Signal to indicate a shutdown has been requested

public slots:
    //void HandleShutdown(); // Slot to handle cleanup and shutdown    
    void toggleDetection(bool enabled);
    void toggleTracking(bool enabled);    
};


#endif // VIDEOCAPTURE_CUDA_H
