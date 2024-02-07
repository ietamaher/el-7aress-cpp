#ifndef PLCDDSPUBLISHER_H
#define PLCDDSPUBLISHER_H

#include <QObject>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>

#include <fastdds/dds/topic/TypeSupport.hpp>
#include "Query_msgPubSubTypes.h"
#include <mutex>
#include <condition_variable>
#include "plcactiondata.h"
#include <chrono>
#include <thread>
#undef Log 
using namespace eprosima::fastdds::dds;

class PlcWriterListener : public DataWriterListener {
public:

    PlcWriterListener(): matched_(0), firstConnected_(false){}
    ~PlcWriterListener() override{}

    int matched_;
    bool firstConnected_;
    void on_publication_matched(DataWriter* writer, const PublicationMatchedStatus& info) override;
};


class PlcDDSPublisher : public QObject {
    Q_OBJECT

public:
    explicit PlcDDSPublisher(const QString& topicName, QObject* parent = nullptr);
    ~PlcDDSPublisher();

    void publishAction(const PlcActionData& actionData);
    void publishStatusUpdate();
    void run();
    // Q_INVOKABLE void enqueueAction(const ActionData& actionData);
    void convertToDdsMessage(const PlcActionData& actionData);

private:

    Query_msg Query_msg_;

    DomainParticipant* participant_;
    Publisher* publisher_;
    Topic* topic_;
    DataWriter* writer_;
    PlcWriterListener listener_;
    TypeSupport  type_;

    std::mutex discoveryMutex_;
    std::condition_variable discoveryCv_;
    int matchedReaderCount_;

    int increment;
    int send;
    //void waitDiscovery();
    

signals:
    void queryDataReceived(const PlcActionData& actionData);  // Signal for enqueuing actions

public slots: // or 'public:' if it's also a regular method
    void enqueueAction(const PlcActionData& actionData);
};

#endif // PLCDDSPUBLISHER_H
