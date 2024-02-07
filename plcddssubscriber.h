#ifndef PLCDDSSUBSCRIBER_H
#define PLCDDSSUBSCRIBER_H

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

class PlcReaderListener : public DataReaderListener {
public:
    PlcReaderListener(std::function<void(const Response_msg&)> callback);
    void on_data_available(DataReader* reader) override;
    void on_subscription_matched(DataReader*, const SubscriptionMatchedStatus& info) override;

private:
    std::function<void(const Response_msg&)> callback;
};


class PlcDDSSubscriber : public QObject {
    Q_OBJECT

public:
    explicit PlcDDSSubscriber(const QString& topicName, QObject* parent = nullptr);
    ~PlcDDSSubscriber();

signals:
    void plcDataReceived(const Response_msg& data);

private:
    DomainParticipant* participant_;
    Subscriber* subscriber_;
    Topic* topic_;
    DataReader* reader_;
    PlcReaderListener* listener_;

};



#endif // PLCDDSSUBSCRIBER_H
