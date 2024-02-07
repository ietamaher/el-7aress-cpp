#ifndef VIDEOCAPTURE_H
#define VIDEOCAPTURE_H

#include <QThread>
#include <QImage>
#include <opencv2/opencv.hpp>

class VideoCapture : public QThread {
    Q_OBJECT

public:
    explicit VideoCapture(QObject *parent = nullptr);
    void run() override;
    void stop();

signals:
    void frameReady(QImage frame);

private:
    bool running;
};

#endif // VIDEOCAPTURE_H
