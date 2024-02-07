#include "joysubscriberthread.h"
#include "joyddssubscriber.h"  // Include your DDS Subscriber class header
#include "qdebug.h"

JoySubscriberThread::JoySubscriberThread(const QString& topicName, QObject* parent)
    : QThread(parent), topicName(topicName), stopped(false) {
    subscriber = new JoyDDSSubscriber(topicName);
    // Connect the DDS subscriber's signal to this class's slot
    connect(subscriber, &JoyDDSSubscriber::joyDataReceived, this, &JoySubscriberThread::handleDataReceived, Qt::DirectConnection);
}

JoySubscriberThread::~JoySubscriberThread() {
    stop();  // Ensure the thread is stopped and cleaned up properly
    delete subscriber;
}

void JoySubscriberThread::run() {
    qDebug() << "Running Motor Subscriber Thread";
    exec();  // Start the event loop
}

void JoySubscriberThread::handleDataReceived(const JoystickData& data) {
    //qDebug() <<"received from joy publisher ...";
    emit joyDataReceived(data);  // Emit the signal with the received data
}

void JoySubscriberThread::stop() {
    stopped = true;
    if (subscriber) {
        subscriber->deleteLater();  // Safely delete the subscriber object
    }
    quit();  // Quit the thread's event loop
    wait();  // Wait for the thread to finish
}

