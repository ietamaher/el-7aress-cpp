#include "camerathread.h"

CameraThread::CameraThread(QObject *parent) : QThread(parent) {
    cameraWidget = new CameraWidget();
    connect(this, &CameraThread::updateCameraWidgetPosition, cameraWidget, &CameraWidget::setTargetPosition, Qt::QueuedConnection);
    connect(this, &CameraThread::updateSettingParameters, cameraWidget, &CameraWidget::setSettingParameters);
}

CameraThread::~CameraThread() {
    delete cameraWidget;
}

void CameraThread::run() {
    // Start camera processing or any other initialization
    cameraWidget->startCamera();

    // Emit signal to pass the CameraWidget instance to the main thread
    emit cameraWidgetReady(cameraWidget);
    exec();
}

void CameraThread::handlePrimaryObjectPositionUpdate(const QPoint& position) {
    emit updateCameraWidgetPosition(position);
}

void CameraThread::handleSettingParameters(const QString& burstMode, bool trackingState, bool detectionState, bool stabState, double speed){
    emit updateSettingParameters(burstMode, trackingState, detectionState, stabState, speed);
}