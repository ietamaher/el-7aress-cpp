#include "plcpublisherthread.h"


PlcPublisherThread::PlcPublisherThread(const QString& topicName, QObject* parent)
    : QThread(parent), topicName(topicName), plcddsPublisher_(nullptr) {
}

PlcPublisherThread::~PlcPublisherThread() {
    if (plcddsPublisher_) {
        delete plcddsPublisher_;
        plcddsPublisher_ = nullptr;
    }
}

void PlcPublisherThread::run() {
    plcddsPublisher_ = new PlcDDSPublisher(topicName);
    std::cout << "PLC publisher thread started ...";
    plcddsPublisher_->run();
    exec();  // Start the event loop if needed
}

void PlcPublisherThread::enqueueAction(const PlcActionData& actionData) {
     std::cout << "cmd to from thream to publisher" ;
    if (plcddsPublisher_) {
        // Pass the action data to the publisher
        plcddsPublisher_->enqueueAction(actionData);

    }
}
