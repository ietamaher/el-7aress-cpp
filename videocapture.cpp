#include "videocapture.h"


VideoCapture::VideoCapture(QObject *parent) : QThread(parent), running(false) {}

void VideoCapture::run() {
    running = true;
    cv::VideoCapture cap("/dev/video1"); // Open the default camera

    if (!cap.isOpened()) {
        return; // Check if we succeeded in opening the camera
    }

    while (running) {
        cv::Mat frame;
        cap >> frame; // Capture a new frame

        if (!frame.empty()) {
            cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
            QImage imgIn = QImage((uchar*) frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
            emit frameReady(imgIn.copy()); // Emit the captured frame
        }
        QThread::msleep(10); // Slow down the loop to a reasonable rate
    }

    cap.release(); // Release the camera
}
 

void VideoCapture::stop() {
    running = false;
    wait(); // Wait for the run loop to finish
}
