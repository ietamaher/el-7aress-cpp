#ifndef PLCPUBLISHERTHREAD_H
#define PLCPUBLISHERTHREAD_H

#include <QThread>
#include "plcddspublisher.h"  // header for DDSPublisher class
#include "plcactiondata.h"    // header for ActionData structure

class PlcPublisherThread : public QThread {
    Q_OBJECT

public:
    explicit PlcPublisherThread(const QString& topicName, QObject* parent = nullptr);
    ~PlcPublisherThread();

    void enqueueAction(const PlcActionData& actionData);  // Public method to enqueue actions

protected:
    void run() override;  // The main function of the thread

private:
    QString topicName;
    PlcDDSPublisher* plcddsPublisher_;  // Pointer to the DDSPublisher
};


#endif // PLCPUBLISHERTHREAD_H
