#ifndef MOTORDDSSUBSCRIBER_H
#define MOTORDDSSUBSCRIBER_H

#include <QObject>

#include "Response_msgPubSubTypes.h"
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

class ReaderListener : public DataReaderListener {
public:
    ReaderListener(std::function<void(const Response_msg&)> callback);
    void on_data_available(DataReader* reader) override;
    void on_subscription_matched(DataReader*, const SubscriptionMatchedStatus& info) override;

private:
    std::function<void(const Response_msg&)> callback;
};


class MotorDDSSubscriber : public QObject {
    Q_OBJECT

public:
    explicit MotorDDSSubscriber(const QString& topicName, QObject* parent = nullptr);
    ~MotorDDSSubscriber();

signals:
    void motorDataReceived(const Response_msg& data);

private:
    DomainParticipant* participant_;
    Subscriber* subscriber_;
    Topic* topic_;
    DataReader* reader_;
    ReaderListener* listener_;

};



#endif // MOTORDDSSUBSCRIBER_H
