#include "motorsubscriberthread.h"
#include "motorddssubscriber.h"  // Include your DDS Subscriber class header
#include "qdebug.h"

MotorSubscriberThread::MotorSubscriberThread(const QString& topicName, QObject* parent)
    : QThread(parent), topicName(topicName), stopped(false) {
    subscriber = new MotorDDSSubscriber(topicName);
    // Connect the DDS subscriber's signal to this class's slot
    connect(subscriber, &MotorDDSSubscriber::motorDataReceived, this, &MotorSubscriberThread::handleDataReceived, Qt::DirectConnection);
}

MotorSubscriberThread::~MotorSubscriberThread() {
    stop();  // Ensure the thread is stopped and cleaned up properly
    delete subscriber;
}

void MotorSubscriberThread::run() {
    qDebug() << "Running Motor Subscriber Thread";
    exec();  // Start the event loop
}

void MotorSubscriberThread::handleDataReceived(const Response_msg& data) {
    //qDebug() <<"received from publisher ...";
    emit motorDataReceived(data);  // Emit the signal with the received data
}

void MotorSubscriberThread::stop() {
    stopped = true;
    if (subscriber) {
        subscriber->deleteLater();  // Safely delete the subscriber object
    }
    quit();  // Quit the thread's event loop
    wait();  // Wait for the thread to finish
}

