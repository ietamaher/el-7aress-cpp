#ifndef MOTORSUBSCRIBERTHREAD_H
#define MOTORSUBSCRIBERTHREAD_H

#include <QThread>
#include <QObject>
#include "Response_msg.h"  // Assuming this is a C++ representation of your DDS message
#include "motorddssubscriber.h"
class DDSSubscriber;  // Assuming this represents your DDS Subscriber class

class MotorSubscriberThread : public QThread {
    Q_OBJECT

public:
    explicit MotorSubscriberThread(const QString& topicName, QObject* parent = nullptr);
    ~MotorSubscriberThread();

    void stop();  // Method to stop the thread
    void handleDataReceived(const Response_msg& data);

signals:
    void motorDataReceived(const Response_msg& data);  // Signal to emit when new data is received

protected:
    void run() override;  // The thread's main function

private:
    QString topicName;
    MotorDDSSubscriber* subscriber;  // Pointer to the DDS subscriber
    bool stopped;  // Flag to control the thread's execution
};

#endif // MOTORSUBSCRIBERTHREAD_H
