#include "joyddssubscriber.h"


joyReaderListener::joyReaderListener(std::function<void(const JoystickData&)> callback)
    : callback(callback) {}


void joyReaderListener::on_data_available(DataReader* reader) {
    JoystickData data;
    SampleInfo info;
    if (reader->take_next_sample(&data, &info) == ReturnCode_t::RETCODE_OK) {
        if (info.valid_data) {
            callback(data);
            //std::cout << "rx response msg ...";
        }
    }
}

void joyReaderListener::on_subscription_matched(DataReader*, const SubscriptionMatchedStatus& info){
    if (info.current_count_change == 1){
        std::cout << "Subscriber matched." << std::endl;
    } else if (info.current_count_change == -1){
        std::cout << "Subscriber unmatched." << std::endl;
    }else{
        std::cout << info.current_count_change << " is not a valid value for SubscriptionMatchedStatus current count change" << std::endl;
    }
}

JoyDDSSubscriber::JoyDDSSubscriber(const QString& topicName, QObject* parent)
    : QObject(parent), participant_(nullptr), subscriber_(nullptr), topic_(nullptr), reader_(nullptr), listener_(nullptr) {
    participant_ = DomainParticipantFactory::get_instance()->create_participant(0, PARTICIPANT_QOS_DEFAULT);
    // Check participant creation

    // Register the TypeSupport
    TypeSupport typeSupport(new JoystickDataPubSubType());
    typeSupport.register_type(participant_);

    // Create Topic
    topic_ = participant_->create_topic(topicName.toStdString(), typeSupport.get_type_name(), TOPIC_QOS_DEFAULT);
    // Check topic creation

    // Create Subscriber
    subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT, nullptr);
    // Check subscriber creation

    // Create Listener with lambda to emit signal
    listener_ = new joyReaderListener([this](const JoystickData& data) {
        emit joyDataReceived(data);
    });

    // Create DataReader
    reader_ = subscriber_->create_datareader(topic_, DATAREADER_QOS_DEFAULT, listener_);
    // Check reader creation
}

JoyDDSSubscriber::~JoyDDSSubscriber() {
    // Clean up DDS entities
    if (participant_) {
        participant_->delete_contained_entities();
        DomainParticipantFactory::get_instance()->delete_participant(participant_);
    }
    delete listener_;
}
