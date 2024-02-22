#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), incr_(0), incr_1(0), in1(0), steps(0),
    azimuth(0.0),
    elevation(0.0),
    lrf(0.0),
    lrf_rdy("OFF"),
    fov(45.0),
    speed_(0),
    latestSpeed(0),
    lastButtonSpdPlusValue(0),
    lastButtonSpdMinusValue(0),
    mot1Speed(0),
    direction(0),
    gun_ready("READY"),
    gun_charged("CHARGED"),
    gun_armed("ARMED"),
    ammunition_low("AMMUNITION LOW"),
    ammunition_ready("AMMUNITION READY"),
    last_button_burst_mode_value(0),
    last_button_track_value(0),
    last_button_detect_value(0),
    last_button_stab_value(0),
    lastButtonBurstModeValue(0),
    last_buttonFireValue(0),
    currentBurstModeIndex(0),
    burstModes({"SINGLE SHOT", "BURST SHORT", "BURST FULL"}),
    track_state("OFF"),
    detect_state("OFF"),
    stab_state("OFF")
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initUI();
    initVideoStream();
    initDDS();
    initLabels();

}



void MainWindow::initUI() {
    //QWidget *centralWidget = new QWidget(this);
    //setCentralWidget(centralWidget);

    //mainLayout = new QVBoxLayout(centralWidget);
    //videoWidget = new VideoWidget(this);
    //mainLayout->addWidget(videoWidget);

    // Setup other UI components like control panels, labels, etc.

    QTimer *updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &MainWindow::updateUI);
    updateTimer->start(100);  // Update UI every 100 ms
    
}

void MainWindow::initVideoStream() {
    // Initialize and start video streaming thread
    // Connect signals from the video streaming thread to slots in this class for frame updates
    //VideoCapture *videoCapture = new VideoCapture(this);
    //CaptureVideoCuda *videoCapture = new CaptureVideoCuda();




    //connect(captureThread, &CaptureVideoCuda::ShutdownRequested, this, &YourClass::HandleCaptureThreadShutdown);
    //connect(this, &MainWindow::detectionToggled, videoCapture, &CaptureVideoCuda::toggleDetection);
    //connect(this, &MainWindow::trackingToggled, videoCapture, &CaptureVideoCuda::toggleTracking);
    //trackingManager = new TrackingManager(this); // 'this' ensures it's parented to the MainWindow for memory management
    //cameraController = new CameraController(this);
    //connect(this, &MainWindow::trackingToggled, trackingManager, &TrackingManager::toggleTracking);
    // Connecting the TrackingManager to the MainWindow and CameraController
    //connect(trackingManager, &TrackingManager::primaryObjectUpdated, this, &MainWindow::onPrimaryObjectUpdated);
    //connect(trackingManager, &TrackingManager::primaryObjectUpdated, cameraController, &CameraController::adjustCamera);
    //connect(trackingManager, &TrackingManager::trackUpdated, this, &MainWindow::updateTrackButton, Qt::QueuedConnection);
    //bool connected = connect(trackingManager, &TrackingManager::trackUpdated, this, &MainWindow::updateTrackButton, Qt::QueuedConnection);
    //Q_ASSERT(connected);
    //qDebug() << "Connection status is set to :" << connected;
    //connect(ui->testButton, &QPushButton::clicked, this, &MainWindow::triggerTrackingManagerTest);

    //connect(videoCapture, &CaptureVideoCuda::frameReady, ui->ui_widget, &VideoGLWidget_gl::updateFrame);
    //videoCapture->start(); // Start the video capture thread


    cameraThread = new CameraThread(this);
    connect(cameraThread, &CameraThread::cameraWidgetReady, this, &MainWindow::addCameraWidgetToLayout);
    //connect(this, &MainWindow::primaryObjectPositionUpdated, cameraThread, &CameraThread::handlePrimaryObjectPositionUpdate);
    //connect(this, SIGNAL(primaryObjectPositionUpdated(QPoint)), cameraThread, SLOT(handlePrimaryObjectPositionUpdate(QPoint)));
    connect(this, &MainWindow::primaryObjectPositionUpdated, cameraThread, &CameraThread::handlePrimaryObjectPositionUpdate);
    connect(this, &MainWindow::settingParameters, cameraThread, &CameraThread::handleSettingParameters);
    connect(this, &MainWindow::displayingMotorParameters, cameraThread, &CameraThread::handleMotorParameters);
    connect(this, &MainWindow::displayingPlcParameters, cameraThread, &CameraThread::handlePlcParameters);

    // Inside MainWindow constructor or initialization function
    //connect(this, &MainWindow::detectionToggled, cameraWidget, &CameraWidget::toggleDetection);
    //connect(this, &MainWindow::trackingToggled, cameraWidget, &CameraWidget::toggleTracking);
    cameraThread->start(); // Start the thread    

}

void MainWindow::addCameraWidgetToLayout(CameraWidget *cameraWidget) {
    QVBoxLayout *layout = new QVBoxLayout(ui->widget2); // Use the central widget's layout
    layout->addWidget(cameraWidget);
}

void MainWindow::onPrimaryObjectUpdated(const ObjectInfo& primaryObject) {

    QString statusText = QString("Primary Object - ID: %1, Position: (%2, %3), Size: (%4x%5)")
                         .arg(primaryObject.uniqueId)
                         .arg(primaryObject.bboxLeft).arg(primaryObject.bboxTop)
                         .arg(primaryObject.bboxRight).arg(primaryObject.bboxBottom);
    qDebug() << statusText ;

    QPoint targetPosition((primaryObject.bboxLeft + primaryObject.bboxRight)/2, (primaryObject.bboxTop + primaryObject.bboxBottom)/2);
    emit primaryObjectPositionUpdated(targetPosition);
}

void MainWindow::updateTrackButton(const ObjectInfo& trackInfo) {
    // Check if a button for this track already exists, update it if so, or create a new one if not.
    // You might use the trackInfo.uniqueId as a key to identify buttons for each track.
    qDebug() << "QPushButton set to:";
    qDebug() << "QPushButton set to:" << trackInfo.uniqueId;
    QPushButton* button = findButtonForTrack(trackInfo.uniqueId);
    if (!button) {
        button = new QPushButton(QString::number(trackInfo.uniqueId), this);
        button->setStyleSheet("QPushButton { color: green; font-size: 30px; }"); // Set text color to green and font size to 30px

        ui->trackButtonLayout->addWidget(button);
        trackButtons[trackInfo.uniqueId] = button; // Add the button to the map

        connect(button, &QPushButton::clicked, [this, trackInfo]() {
            focusCameraOnTrack(trackInfo);
        });
    } else {
        // If the button already exists, you can update its properties as needed
        button->setText(QString::number(trackInfo.uniqueId)); // Update button text to uniqueId
        button->setStyleSheet("QPushButton { color: green; font-size: 30px; }"); // Update text color to green and font size to 30px
        qDebug() << "Button already exist " << trackInfo.uniqueId;
    }
}
 


QPushButton* MainWindow::findButtonForTrack(unsigned long uniqueId) {
    return trackButtons.value(uniqueId, nullptr); // Returns nullptr if uniqueId is not found
}


void MainWindow::focusCameraOnTrack(const ObjectInfo& trackInfo) {
    QString statusText = QString(" focusCameraOnTrack  Primary Object - ID: %1, Position: (%2, %3), Size: (%4x%5)")
                         .arg(trackInfo.uniqueId)
                         .arg(trackInfo.bboxLeft).arg(trackInfo.bboxTop)
                         .arg(trackInfo.bboxRight).arg(trackInfo.bboxBottom);
    qDebug() << statusText ;
}

void MainWindow::triggerTrackingManagerTest() {
    trackingManager->testProcessObjectMetadata();
}
void MainWindow::initDDS() {
    // Initialize the MotorSubscriberThread
    motorSubscriberThread_ = new MotorSubscriberThread("ResponseTopic", this);
    //connect(motorSubscriberThread_, &MotorSubscriberThread::motorDataReceived, this, &MainWindow::onMotorDataReceived);
    connect(motorSubscriberThread_, &MotorSubscriberThread::motorDataReceived,
            this, &MainWindow::onMotorDataReceived,
            Qt::DirectConnection);
    // Start the thread
    motorSubscriberThread_->start();
    // Initialize the MotorSubscriberThread
    plcSubscriberThread_ = new PlcSubscriberThread("PLC_ResponseTopic", this);
    //connect(motorSubscriberThread_, &MotorSubscriberThread::motorDataReceived, this, &MainWindow::onMotorDataReceived);
    connect(plcSubscriberThread_, &PlcSubscriberThread::plcDataReceived,
            this, &MainWindow::onPlcDataReceived,
           Qt::DirectConnection);
    // Start the thread
    plcSubscriberThread_->start();


    joySubscriberThread_ = new JoySubscriberThread("JoystickTopic", this);
    //connect(motorSubscriberThread_, &MotorSubscriberThread::motorDataReceived, this, &MainWindow::onMotorDataReceived);
    connect(joySubscriberThread_, &JoySubscriberThread::joyDataReceived,
            this, &MainWindow::onJoyDataReceived,
            Qt::DirectConnection);
    // Start the thread
    joySubscriberThread_->start();


    // Initialize the MotorPublisherThread
    motorPublisherThread_ = new MotorPublisherThread("QueryTopic", this);
    motorPublisherThread_->start();
    
    plcPublisherThread_ = new PlcPublisherThread("PLC_QueryTopic", this);
    plcPublisherThread_->start();    
}

/**
 * @brief Handles joystick input data and updates UI components and internal states accordingly.
 * 
 * This method processes the joystick data received, including button states, axis states, and hat states.
 * It updates the UI and internal states based on the joystick input, such as speed adjustments, mode toggling,
 * and tracking or detection state changes. It also publishes actions to PLC or motor controllers based on the input.
 * 
 * @param data A const reference to JoystickData, which contains the current state of the joystick,
 * including button states, axis positions, and hat directions.
 */
void MainWindow::onJoyDataReceived(const JoystickData& data) {
    //qDebug() << "onJoyDataReceived received #";
    // Retrieve joystick states (assuming these methods return vectors or arrays)
    std::vector<int> buttonStates = data.buttonStates();
    std::vector<double> axisStates = data.axisStates();
    std::vector<int> hatStates = data.hatStates();

    // Assign button state values
    int buttonSpdPlusValue = buttonStates[1];
    int buttonSpdMinusValue = buttonStates[0];
    int buttonBurstModeValue = buttonStates[6];
    int buttonTrackValue = buttonStates[3];
    int buttonDetectValue = buttonStates[2];
    int buttonStabValue = buttonStates[7];
    int buttonFireValue = buttonStates[4];

    // Burst mode button
    if (buttonBurstModeValue == 1 && lastButtonBurstModeValue == 0) {
        // Cycle through burst modes
        this->currentBurstModeIndex = (this->currentBurstModeIndex + 1) % this->burstModes.size();
        updateBurstModeLabel();
        //qDebug() << "Burst mode set to:" << QString::fromStdString(this->burstModes[this->currentBurstModeIndex]);
        qDebug() << "Burst mode set to:" << this->burstModes[this->currentBurstModeIndex]; 
        QMap<QString, QVariant> params;
        params["val"] = this->currentBurstModeIndex;

        PlcActionData plcActionData("mode", "plc1", params);       

        plc_PublishAction(plcActionData) ;               
        //ActionData actionData("mode", "plc1", {{"val", currentBurstModeIndex}});
        //publishPlcAction(actionData);
    }
    this->lastButtonBurstModeValue = buttonBurstModeValue;

    if (buttonTrackValue == 1 && this->last_button_track_value ==0){
        //Toggle track state
        this->track_state = (this->track_state == "OFF") ? "ON" : "OFF";
        updateTrackLabel();
        qDebug() << "Track set to: " << this->track_state;
        emit trackingToggled(this->track_state == "ON");
        
    };   
    this->last_button_track_value = buttonTrackValue;
 
    if (buttonDetectValue == 1 && this->last_button_detect_value ==0){
        //Toggle track state
        this->detect_state = (this->detect_state == "OFF") ? "ON" : "OFF";
        updateDetectLabel();
        qDebug() << "Detection set to: " << this->detect_state ; 
        emit detectionToggled(this->detect_state == "ON");        
    };
    this->last_button_detect_value = buttonDetectValue;

    if (buttonStabValue == 1 && this->last_button_stab_value ==0){
        //Toggle track state
        this->stab_state = (this->stab_state == "OFF") ? "ON" : "OFF";
        updateStabLabel();
        qDebug() << "Stab set to: " << this->stab_state;
    }; 
    this->last_button_stab_value = buttonStabValue;

    // Increase speed
    if (buttonSpdPlusValue == 1 && lastButtonSpdPlusValue == 0) {
        this->speed_ += 1000;  // Increase speed
        this->speed_ = std::max(0, std::min(this->speed_, 10000));
        updateSpeedLabel();  // Update the speed label on the UI
        qDebug() << this->speed_;  // Print the current speed for debugging
    }
    lastButtonSpdPlusValue = buttonSpdPlusValue;  // Update the last button state

    // Decrease speed
    if (buttonSpdMinusValue == 1 && lastButtonSpdMinusValue == 0) {
        this->speed_ -= 1000;  // Decrease speed
        this->speed_ = std::max(0, std::min(this->speed_, 10000));
        updateSpeedLabel();  // Update the speed label on the UI
        qDebug() << this->speed_;  // Print the current speed for debugging
    }
    lastButtonSpdMinusValue = buttonSpdMinusValue;  // Update the last button state

    // Ensure speed is within bounds [0, 10000]
    


    // Process axis state for motor speed control
    double axis4Value = axisStates[4]; // Assuming this is a double value
    int mot1Speed = 0;
    if (axis4Value >= 0.3) {
        mot1Speed = this->speed_;
        direction = 0x4000;
    } else if (axis4Value <= -0.3) {
        mot1Speed = this->speed_;
        direction = 0x8000;
    } else {
        mot1Speed = 0;
        direction = 0;
    }
    QThread::msleep(10); // Use QThread::msleep in Qt instead of time.sleep

    if (mot1Speed != latestSpeed) {
        qDebug() << direction << mot1Speed;
        QMap<QString, QVariant> params;
        params["direction"] = direction;
        params["speed"] = mot1Speed;
        ActionData actionData("move_forward", "motor1", params);        
        //ActionData actionData("move_forward", "motor1", {{"direction", direction}, {"speed", mot1Speed}});
        publishAction(actionData);
        latestSpeed = mot1Speed;
    }

    this->speed_ = std::max(0, std::min(this->speed_, 10000));   
    
    if (buttonFireValue != last_buttonFireValue){
        QMap<QString, QVariant> params;
        params["val"] = buttonFireValue;

        PlcActionData plcActionData("fire", "plc1", params);       

        plc_PublishAction(plcActionData) ;
        last_buttonFireValue = buttonFireValue;  
        }    

    emit settingParameters(this->burstModes[currentBurstModeIndex],
                     this->track_state == "ON",
                     this->detect_state == "ON",
                     this->stab_state == "ON",
                     this->speed_);

}

/**
 * @brief Publishes an action to a motor controller based on the provided action data.
 * 
 * This method enqueues the specified action into the motor publisher thread, which is responsible for
 * sending commands to the motor controller. The action data includes details about the action to be performed,
 * such as movement direction or speed adjustments. This method is typically called in response to user inputs
 * or other events that require motor action.
 * 
 * @param action_data A const reference to ActionData, encapsulating the details of the action to be performed,
 * including the action type and necessary parameters.
 */
void MainWindow::publishAction(const ActionData& action_data){
        //Assuming you have initialized and started a PublisherThread named self.dds_publisher_thread
        motorPublisherThread_->enqueueAction(action_data);
        qDebug() << "cmd to move" ;
}

/**
 * @brief Publishes an action to a PLC (Programmable Logic Controller) based on the provided PLC action data.
 * 
 * Similar to `publishAction`, this method enqueues the specified action into the PLC publisher thread,
 * which handles commands intended for PLC-controlled devices. The PLC action data contains information
 * specific to PLC operations, such as device control commands or configuration changes. This method
 * facilitates interaction with PLC devices, enabling control over their operation based on the current
 * application state or user inputs.
 * 
 * @param action_data A const reference to PlcActionData, containing the specifics of the PLC action,
 * including the action type and associated parameters.
 */ 
void MainWindow::plc_PublishAction(const PlcActionData& action_data){
        //Assuming you have initialized and started a PublisherThread named self.dds_publisher_thread
        plcPublisherThread_->enqueueAction(action_data);
        qDebug() << "cmd to fire" ;
}

/**
 * @brief Processes motor response data and updates the UI accordingly.
 * 
 * This method extracts relevant information from the motor response, such as azimuth,
 * and updates the UI components to reflect the current state or position. It increments
 * an internal counter to track the number of responses processed.
 * 
 * @param data A const reference to Response_msg, which contains the response data from the motor,
 * including status, position, or error codes as applicable.
 */
void MainWindow::onMotorDataReceived(const Response_msg& data) {
    // Extract azimuth from data - assuming data() returns a vector or array-like structure
    // Adjust the index [4] as needed to match the data layout in Response_msg
    this->steps = data.data()[4];

    incr_++;
    ui->display_detection->setText(QString::number(incr_));
    // Update the azimuth label with the new value
    updateAzimuthLabel();

}

/**
 * @brief Processes PLC response data and updates the UI accordingly.
 * 
 * Similar to onMotorDataReceived, this method processes response data from PLC-controlled devices.
 * It updates specific UI components related to PLC operations and increments an internal counter
 * to keep track of the number of PLC responses processed.
 * 
 * @param data A const reference to Response_msg, which contains the response data from the PLC,
 * including status, position, or error codes as applicable.
 */
void MainWindow::onPlcDataReceived(const Response_msg& data) {
    // Extract azimuth from data - assuming data() returns a vector or array-like structure
    // Adjust the index [4] as needed to match the data layout in Response_msg
    //this->steps = data.data()[0];

    incr_1++;
    emit displayingPlcParameters(incr_1, this->in1);

    ui->display_stab->setText(QString::number(incr_1));

    // Update the azimuth label with the new value
    //updateAzimuthLabel();

}

void MainWindow::updateUI() {
    // Update UI elements based on latest data (e.g., DDS data, video frames)
}

/**
 * @brief Updates the label displaying the current azimuth value.
 * 
 * This method calculates the azimuth based on internal step count, formats it for display,
 * and updates the corresponding UI label. It also emits a signal with the updated parameters
 * for any connected slots to use.
 */
void MainWindow::updateAzimuthLabel() {

    double azimuth = std::fmod(static_cast<double>(this->steps) * 0.009, 360.0);
    azimuth = std::round(azimuth * 10.0) / 10.0; // Round to 1 decimal place

    // Create the text for the label
    QString azimuthText = QString("Az: %1").arg(azimuth, 0, 'f', 1); // 'f' for fixed-point notation, 1 for 1 decimal place
    emit displayingMotorParameters(this->incr_, azimuth);


    // Set the text on the label
    ui->display_azimuth->setText(azimuthText);
}

void MainWindow::updateTrackLabel(){        
    ui->display_track->setText(QString("TRACK:%1").arg(this->track_state));
}

void MainWindow::updateDetectLabel(){         
    ui->display_detection->setText(QString("DETECTION:%1").arg(this->detect_state));
}

void MainWindow::updateStabLabel() {         
    ui->display_stab->setText(QString("STAB:%1").arg(this->stab_state));
}

void MainWindow::updateSpeedLabel(){

        ui->display_speed->setText(QString("SPEED:%1").arg(this->speed_/100,0))  ;
}
void MainWindow::updateBurstModeLabel() {

    // Create the text for the label
    QString burstMode = QString("MODE:%1").arg(this->burstModes[this->currentBurstModeIndex]); // 'f' for fixed-point notation, 1 for 1 decimal place

    // Set the text on the label
    ui->display_burst_mode->setText(burstMode);  
}

void MainWindow::initLabels() {
    /*steps = 0;
    azimuth = 0.0;
    elevation = 0.0;
    lrf = 0.0;
    lrf_rdy = "OFF";

    fov = 45.0;
    speed_ = 10;

    gun_ready = "READY";
    gun_charged = "CHARGED";
    gun_armed = "ARMED";
    ammunition_low = "AMMUNITION LOW";
    ammunition_ready = "AMMUNITION READY";

    last_button_burst_mode_value = 0;
    last_button_track_value = 0;
    last_button_detect_value = 0;
    last_button_stab_value = 0;

    burstModes = {"SINGLE SHOT", "BURST SHORT", "BURST FULL"};
    currentBurstModeIndex = 0;
    track_state = "OFF";
    detect_state = "OFF";
    stab_state = "OFF";*/
    ui->display_lrf->setText(QString("LRF: %1 M").arg(this->lrf, 0, 'f', 1));
    ui->display_track->setText(QString("TRACK:%1").arg(this->track_state));
    ui->display_stab->setText(QString("STAB:%1").arg(this->stab_state));
    ui->display_detection->setText(QString("DETECTION:%1").arg(this->detect_state));

    ui->display_azimuth->setText(QString("Az: %1").arg(this->azimuth, 0, 'f', 1));
    ui->display_burst_mode->setText(QString("MODE:%1").arg(this->burstModes[this->currentBurstModeIndex]));


    // Here you might also update UI labels, buttons, etc., based on the above values
    // For example:
    // ui->azimuthLabel->setText(QString("Az: %1").arg(azimuth, 0, 'f', 1));
}


MainWindow::~MainWindow()
{
    //if (videoCapture->isRunning()) {
        //videoCapture->stop();
    //}
    // Ensure the thread is properly stopped and cleaned up
    motorSubscriberThread_->stop();
    motorSubscriberThread_->wait();
    delete motorSubscriberThread_;

    delete ui;
}
