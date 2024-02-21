#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H

#include <QThread>
#include "camerawidget.h"

class CameraThread : public QThread {
    Q_OBJECT
public:
    explicit CameraThread(QObject *parent = nullptr);
    ~CameraThread();

protected:
    void run() override;

signals:
    void cameraWidgetReady(CameraWidget *cameraWidget);
    void updateCameraWidgetPosition(const QPoint& position);
    void updateSettingParameters(const QString& burstMode, bool trackingState, bool detectionState, bool stabState, double speed);


public slots:
    void handlePrimaryObjectPositionUpdate(const QPoint& position);
    void handleSettingParameters(const QString& burstMode, bool trackingState, bool detectionState, bool stabState, double speed);

              
private:
    CameraWidget *cameraWidget;
};

#endif // CAMERATHREAD_H
