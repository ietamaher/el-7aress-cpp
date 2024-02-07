#include "motorpublisherthread.h"


MotorPublisherThread::MotorPublisherThread(const QString& topicName, QObject* parent)
    : QThread(parent), topicName(topicName), motorddsPublisher_(nullptr) {
}

MotorPublisherThread::~MotorPublisherThread() {
    if (motorddsPublisher_) {
        delete motorddsPublisher_;
        motorddsPublisher_ = nullptr;
    }
}

void MotorPublisherThread::run() {
    motorddsPublisher_ = new MotorDDSPublisher(topicName);
    std::cout << "pub thread started ...";
    motorddsPublisher_->run();
    exec();  // Start the event loop if needed
}

void MotorPublisherThread::enqueueAction(const ActionData& actionData) {
     std::cout << "cmd to from thream to publisher" ;
    if (motorddsPublisher_) {
        // Pass the action data to the publisher
        motorddsPublisher_->enqueueAction(actionData);
        //QMetaObject::invokeMethod(motorddsPublisher_, "enqueueAction", Qt::QueuedConnection, Q_ARG(ActionData, actionData));
    }
}
