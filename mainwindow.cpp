#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), incr_(0), steps(0),
    azimuth(0.0),
    elevation(0.0),
    lrf(0.0),
    lrf_rdy("OFF"),
    fov(45.0),
    speed_(10),
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
    CaptureVideoCuda *videoCapture = new CaptureVideoCuda();
    //connect(captureThread, &CaptureVideoCuda::ShutdownRequested, this, &YourClass::HandleCaptureThreadShutdown);
    connect(this, &MainWindow::detectionToggled, videoCapture, &CaptureVideoCuda::toggleDetection);
    connect(this, &MainWindow::trackingToggled, videoCapture, &CaptureVideoCuda::toggleTracking);

    connect(videoCapture, &CaptureVideoCuda::frameReady, ui->ui_widget, &VideoGLWidget_gl::updateFrame);
    videoCapture->start(); // Start the video capture thread
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
        updateSpeedLabel();  // Update the speed label on the UI
        qDebug() << this->speed_;  // Print the current speed for debugging
    }
    lastButtonSpdPlusValue = buttonSpdPlusValue;  // Update the last button state

    // Decrease speed
    if (buttonSpdMinusValue == 1 && lastButtonSpdMinusValue == 0) {
        this->speed_ -= 1000;  // Decrease speed
        updateSpeedLabel();  // Update the speed label on the UI
        qDebug() << this->speed_;  // Print the current speed for debugging
    }
    lastButtonSpdMinusValue = buttonSpdMinusValue;  // Update the last button state

    // Ensure speed is within bounds [0, 10000]
    speed_ = std::max(0, std::min(speed_, 10000));


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

}

void MainWindow::publishAction(const ActionData& action_data){
        //Assuming you have initialized and started a PublisherThread named self.dds_publisher_thread
        motorPublisherThread_->enqueueAction(action_data);
        qDebug() << "cmd to move" ;
}
 
void MainWindow::plc_PublishAction(const PlcActionData& action_data){
        //Assuming you have initialized and started a PublisherThread named self.dds_publisher_thread
        plcPublisherThread_->enqueueAction(action_data);
        qDebug() << "cmd to fire" ;
}


void MainWindow::onMotorDataReceived(const Response_msg& data) {
    // Extract azimuth from data - assuming data() returns a vector or array-like structure
    // Adjust the index [4] as needed to match the data layout in Response_msg
    this->steps = data.data()[4];

    incr_++;
    ui->display_detection->setText(QString::number(incr_));

    // Update the azimuth label with the new value
    updateAzimuthLabel();

}

void MainWindow::onPlcDataReceived(const Response_msg& data) {
    // Extract azimuth from data - assuming data() returns a vector or array-like structure
    // Adjust the index [4] as needed to match the data layout in Response_msg
    this->steps = data.data()[0];

    incr_++;
    ui->display_stab->setText(QString::number(incr_));

    // Update the azimuth label with the new value
    //updateAzimuthLabel();

}

void MainWindow::updateUI() {
    // Update UI elements based on latest data (e.g., DDS data, video frames)
}

void MainWindow::updateAzimuthLabel() {

    double azimuth = std::fmod(static_cast<double>(this->steps) * 0.009, 360.0);
    azimuth = std::round(azimuth * 10.0) / 10.0; // Round to 1 decimal place

    // Create the text for the label
    QString azimuthText = QString("Az: %1").arg(azimuth, 0, 'f', 1); // 'f' for fixed-point notation, 1 for 1 decimal place

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
    if (videoCapture->isRunning()) {
        //videoCapture->stop();
    }
    // Ensure the thread is properly stopped and cleaned up
    motorSubscriberThread_->stop();
    motorSubscriberThread_->wait();
    delete motorSubscriberThread_;

    delete ui;
}
