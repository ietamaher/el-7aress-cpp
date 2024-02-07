#include "plcddspublisher.h"


void PlcWriterListener::on_publication_matched(DataWriter* writer, const PublicationMatchedStatus& info) {
    if (info.current_count_change > 0) {
        std::cout << "Publisher matched " << info.last_subscription_handle << std::endl;
    } else {
        std::cout << "Publisher unmatched " << info.last_subscription_handle << std::endl;
    }
}

PlcDDSPublisher::PlcDDSPublisher(const QString& topicName, QObject* parent)
    : QObject(parent), participant_(nullptr), publisher_(nullptr), topic_(nullptr), writer_(nullptr), type_(new Query_msgPubSubType()), matchedReaderCount_(0) {

    DomainParticipantQos pqos = PARTICIPANT_QOS_DEFAULT;
    pqos.name("Plc_Query_pub");

    participant_ = DomainParticipantFactory::get_instance()->create_participant(0, PARTICIPANT_QOS_DEFAULT);
    //listener_ = new PlcWriterListener();
    //writer_->set_listener(listener_);

    type_.register_type(participant_);
    publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT, nullptr);

    topic_ = participant_->create_topic(topicName.toStdString(), "Query_msg", TOPIC_QOS_DEFAULT);


    DataWriterQos dw_qos;
    publisher_->get_default_datawriter_qos(dw_qos);

    // Enable asynchronous publishing
    dw_qos.publish_mode().kind = ASYNCHRONOUS_PUBLISH_MODE;
    writer_ = publisher_->create_datawriter(topic_, dw_qos, &listener_);
    int increment = 0;
    int send = 0;

     // Initialize DDS entities (Publisher, Topic, DataWriter) here similar to Python code
}

PlcDDSPublisher::~PlcDDSPublisher() {
    // Clean up DDS entities
    if (participant_) {
        participant_->delete_contained_entities();
        DomainParticipantFactory::get_instance()->delete_participant(participant_);
    }
}

void PlcDDSPublisher::run() {
    //waitDiscovery();  // Ensure there's at least one subscriber matched

    while (true) {
        if (send) {
            // Assuming 'send' is a flag indicating whether there's action data to publish
            // If you have an action queue similar to the Python version, retrieve and publish the next action
            // ActionData actionData; // Retrieve from queue
            // publishAction(actionData); // Publish the action data
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        } else {
            publishStatusUpdate();  // Publish a regular status update
            // Log the publishing event, if necessary
            increment++;  // Increment your counter, if needed for tracking
            std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Sleep for a short period
        }
    }
}

 
void PlcDDSPublisher::publishStatusUpdate() {
    Query_msg query;
    query.slave_id (2);
    query.func_code(4);
    query.write_addr(0);
    query.write_num(0);
    query.read_addr(0);
    query.read_num(13);
    // Set up the query message similar to Python code
    writer_->write(&query);
}

void PlcDDSPublisher::enqueueAction(const PlcActionData& actionData) {
    // Convert action data to DDS message and enqueue or directly publish
    convertToDdsMessage(actionData);
    std::cout << "cmd to publish" ;
}

 

void PlcDDSPublisher::convertToDdsMessage(const PlcActionData& actionData) {

    Query_msg ddsMessage;

   // ddsMessage.setSlaveId(actionData.target == "motor1" ? 1 : 2);

    int speed = actionData.parameters["speed"].toInt();
    int direction = actionData.parameters["direction"].toInt();
    int val = actionData.parameters["val"].toInt();
    ddsMessage.func_code(1);
    this->send = 1;
    std::array<uint32_t, 64> dataValues; // Initialize a vector with 64 zeros
    if (actionData.command_type == "mode") {

        dataValues[0] = (val+1) * 300; // Set the first value to speed

        ddsMessage.slave_id(2);
        ddsMessage.func_code(1);
        ddsMessage.write_addr(0);
        ddsMessage.write_num(1);
        ddsMessage.data(dataValues);
        writer_->write(&ddsMessage);  
        
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
     }
     else {
	    if (val > 0){
		std::fill(dataValues.begin(), dataValues.end(), 0);
		ddsMessage.slave_id(2) ;  
		ddsMessage.func_code(3);  
		ddsMessage.write_addr(0) ; 
		ddsMessage.write_num(7);
		dataValues[0] = 1;
		dataValues[1] = 1;
		dataValues[2] = 1;		
		ddsMessage.data(dataValues) ;
		writer_->write(&ddsMessage);  
        	std::this_thread::sleep_for(std::chrono::milliseconds(10));
           }
	    else{
	    	std::fill(dataValues.begin(), dataValues.end(), 0);
		ddsMessage.slave_id(2) ;  
		ddsMessage.func_code(3);  
		ddsMessage.write_addr(0) ; 
		ddsMessage.write_num(7);
		dataValues[0] = 0;
		dataValues[1] = 0;
		dataValues[2] = 0;		
		ddsMessage.data(dataValues) ;
		writer_->write(&ddsMessage);  
        	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	  }   
     
     
     
     }  



    this->send = 0;
}

void PlcDDSPublisher::publishAction(const PlcActionData& actionData) {
    //writer_.write(actionData); // Assuming 'writer' has a write method that accepts ActionData
}

