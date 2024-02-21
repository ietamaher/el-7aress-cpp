#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <QObject>
#include "objectinfo.h"

class CameraController : public QObject {
    Q_OBJECT

public:
    CameraController(QObject *parent = nullptr);

public slots:
    void adjustCamera(const ObjectInfo& targetObject);

    // Add methods for camera control commands
};

#endif // CAMERACONTROLLER_H