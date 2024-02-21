#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
//#include "videowidget.h"


#include <QLabel>
#include <QTimer>
#include "QPushButton"

#include "motorsubscriberthread.h"
#include "joysubscriberthread.h"
#include "plcsubscriberthread.h"

#include "motorpublisherthread.h"
#include "plcpublisherthread.h"
//#include "videocapture.h"
//#include "videocapture_cuda.h"
#include "camerathread.h"
#include "camerawidget.h"


#include "videoglwidget_gl.h"
#include "actiondata.h"
#include "plcactiondata.h"

#include "trackingmanager.h"
#include "cameracontroller.h"
#include "overlaywidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void addCameraWidgetToLayout(CameraWidget *cameraWidget);

private:
    Ui::MainWindow *ui;
    void initUI();
    void initDDS();
    void initVideoStream();
    void initLabels();
    void updateAzimuthLabel();
    void updateTrackLabel();
    void updateDetectLabel();
    void updateStabLabel();
    void updateSpeedLabel();
    void updateBurstModeLabel();
    void publishAction(const ActionData& action_data);
    void plc_PublishAction(const PlcActionData& action_data);
    void focusCameraOnTrack(const ObjectInfo& trackInfo);


    QVBoxLayout *mainLayout;
    QPushButton* findButtonForTrack(unsigned long uniqueId);
    QMap<unsigned long, QPushButton*> trackButtons;

    //VideoWidget *videoWidget;  // OpenGL widget for video streaming
    // Add DDS-related members, QTimer, etc.
    //CaptureVideoCuda *videoCapture; // Declare videoCapture as a member variable
    VideoGLWidget_gl *videoWidget;
    CameraThread *cameraThread;
    CameraWidget *cameraWidget;
    TrackingManager *trackingManager;
    CameraController *cameraController;


    MotorSubscriberThread* motorSubscriberThread_;  // Pointer to the MotorSubscriberThread
    MotorPublisherThread* motorPublisherThread_;
    JoySubscriberThread* joySubscriberThread_; 
    PlcSubscriberThread* plcSubscriberThread_;  // Pointer to the MotorSubscriberThread
    PlcPublisherThread* plcPublisherThread_;   

    int incr_;
    int steps;                  // Total steps taken by the motor
    double azimuth;             // Initial azimuth value
    double elevation;           // Elevation value
    double lrf;                 // Laser Range Finder distance
    QString lrf_rdy;            // LRF ready state

    double fov;                 // Field of View
    int speed_;                 // Speed
    int latestSpeed;
    int lastButtonSpdPlusValue;
    int lastButtonSpdMinusValue;
    int mot1Speed;
    int direction;

    QString gun_ready;          // Gun ready state
    QString gun_charged;        // Gun charged state
    QString gun_armed;          // Gun armed state
    QString ammunition_low;     // Ammunition low state
    QString ammunition_ready;   // Ammunition ready state

    // Initial states and modes
    int last_button_burst_mode_value;
    int last_button_track_value;
    int last_button_detect_value;
    int last_button_stab_value;
    int lastButtonBurstModeValue;
    int last_buttonFireValue;
    
    
    QStringList burstModes;    // Burst modes
    int currentBurstModeIndex;  // Current burst mode index
    QString track_state;        // Tracking state
    QString detect_state;       // Detection state
    QString stab_state;         // Stabilization state


public slots:
    void updateUI();  // Slot to update UI elements
    void onMotorDataReceived(const Response_msg& data);  // Slot to handle motor data
    void onJoyDataReceived(const JoystickData& data);  // Slot to handle motor data
    void onPlcDataReceived(const Response_msg& data);  // Slot to handle motor data
    void onPrimaryObjectUpdated(const ObjectInfo& primaryObject);
    void updateTrackButton(const ObjectInfo& trackInfo);
    void triggerTrackingManagerTest();

signals:
    void detectionToggled(bool enabled);
    void settingParameters(const QString& burstMode, bool trackingState, bool detectionState, bool stabState, double speed);
    void trackingToggled(bool enabled);  // Add slots for DDS data updates, video frame updates, etc.
    void primaryObjectPositionUpdated(const QPoint& position);


};

#endif // MAINWINDOW_H
