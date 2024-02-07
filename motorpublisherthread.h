#ifndef MOTORPUBLISHERTHREAD_H
#define MOTORPUBLISHERTHREAD_H

#include <QThread>
#include "motorddspublisher.h"  // Include the header for your DDSPublisher class
#include "actiondata.h"    // Include the header for your ActionData structure

class MotorPublisherThread : public QThread {
    Q_OBJECT

public:
    explicit MotorPublisherThread(const QString& topicName, QObject* parent = nullptr);
    ~MotorPublisherThread();

    void enqueueAction(const ActionData& actionData);  // Public method to enqueue actions

protected:
    void run() override;  // The main function of the thread

private:
    QString topicName;
    MotorDDSPublisher* motorddsPublisher_;  // Pointer to the DDSPublisher
};


#endif // MOTORPUBLISHERTHREAD_H
