#ifndef JOYSUBSCRIBERTHREAD_H
#define JOYSUBSCRIBERTHREAD_H

#include <QThread>
#include <QObject>
#include "JoystickData.h"  // Assuming this is a C++ representation of your DDS message
#include "joyddssubscriber.h"

class JoySubscriberThread : public QThread {
    Q_OBJECT

public:
    explicit JoySubscriberThread(const QString& topicName, QObject* parent = nullptr);
    ~JoySubscriberThread();

    void stop();  // Method to stop the thread
    void handleDataReceived(const JoystickData& data);

signals:
    void joyDataReceived(const JoystickData& data);  // Signal to emit when new data is received

protected:
    void run() override;  // The thread's main function

private:
    QString topicName;
    JoyDDSSubscriber* subscriber;  // Pointer to the DDS subscriber
    bool stopped;  // Flag to control the thread's execution
};

#endif // JOYSUBSCRIBERTHREAD_H
