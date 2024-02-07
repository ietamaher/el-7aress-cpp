#ifndef JOYDDSSUBSCRIBER_H
#define JOYDDSSUBSCRIBER_H

#include <QObject>

#include "JoystickDataPubSubTypes.h"
#include <chrono>
#include <thread>

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>

#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#undef Log 

using namespace eprosima::fastdds::dds;


class joyReaderListener : public DataReaderListener {
public:
    joyReaderListener(std::function<void(const JoystickData&)> callback);
    void on_data_available(DataReader* reader) override;
    void on_subscription_matched(DataReader*, const SubscriptionMatchedStatus& info) override;

private:
    std::function<void(const JoystickData&)> callback;
};


class JoyDDSSubscriber : public QObject {
    Q_OBJECT

public:
    explicit JoyDDSSubscriber(const QString& topicName, QObject* parent = nullptr);
    ~JoyDDSSubscriber();

signals:
    void joyDataReceived(const JoystickData& data);

private:
    DomainParticipant* participant_;
    Subscriber* subscriber_;
    Topic* topic_;
    DataReader* reader_;
    joyReaderListener* listener_;

};



#endif // JOYDDSSUBSCRIBER_H
