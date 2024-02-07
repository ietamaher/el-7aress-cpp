#ifndef PLCSUBSCRIBERTHREAD_H
#define PLCSUBSCRIBERTHREAD_H

#include <QThread>
#include <QObject>
#include "Response_msg.h"  // Assuming this is a C++ representation of your DDS message
#include "plcddssubscriber.h"
class DDSSubscriber;  // Assuming this represents your DDS Subscriber class

class PlcSubscriberThread : public QThread {
    Q_OBJECT

public:
    explicit PlcSubscriberThread(const QString& topicName, QObject* parent = nullptr);
    ~PlcSubscriberThread();

    void stop();  // Method to stop the thread
    void handleDataReceived(const Response_msg& data);

signals:
    void plcDataReceived(const Response_msg& data);  // Signal to emit when new data is received

protected:
    void run() override;  // The thread's main function

private:
    QString topicName;
    PlcDDSSubscriber* subscriber;  // Pointer to the DDS subscriber
    bool stopped;  // Flag to control the thread's execution
};

#endif // PLCSUBSCRIBERTHREAD_H
