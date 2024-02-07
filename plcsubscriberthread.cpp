#include "plcsubscriberthread.h"
#include "plcddssubscriber.h"  // Include your DDS Subscriber class header
#include "qdebug.h"

PlcSubscriberThread::PlcSubscriberThread(const QString& topicName, QObject* parent)
    : QThread(parent), topicName(topicName), stopped(false) {
    subscriber = new PlcDDSSubscriber(topicName);
    // Connect the DDS subscriber's signal to this class's slot
    connect(subscriber, &PlcDDSSubscriber::plcDataReceived, this, &PlcSubscriberThread::handleDataReceived, Qt::DirectConnection);
}

PlcSubscriberThread::~PlcSubscriberThread() {
    stop();  // Ensure the thread is stopped and cleaned up properly
    delete subscriber;
}

void PlcSubscriberThread::run() {
    qDebug() << "Running PLC Subscriber Thread";
    exec();  // Start the event loop
}

void PlcSubscriberThread::handleDataReceived(const Response_msg& data) {
    //qDebug() <<"received from publisher ...";
    emit plcDataReceived(data);  // Emit the signal with the received data
}

void PlcSubscriberThread::stop() {
    stopped = true;
    if (subscriber) {
        subscriber->deleteLater();  // Safely delete the subscriber object
    }
    quit();  // Quit the thread's event loop
    wait();  // Wait for the thread to finish
}

