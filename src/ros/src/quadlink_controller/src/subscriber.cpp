#include "subscriber.h"

quadlink::Subscriber::Subscriber() : Node("subscriber") // parametrize node name? different nodes for different purposes??
{
}

template <typename T>
std::function<void(const T&)> topic_callback_wrapper(const std::string& topic_name){
    return [this, topic_name](const T & msg) {
        topic_callback(topic_name, msg);
    };
}

template <typename T>
void quadlink::Subscriber::topic_callback(const std::string topic, const T & msg) 
{
    __subscriptions[topic].second = msg
}

template <typename T>
quadlink::ConnectionStatus quadlink::Subscriber::add_subscriber(const std::string topic)
{
    // parametrize queue size?
    /**
     * Creates a subscription in this node with a msgs type given by the user.
     * 
     */
    subscription = this->create_subscription<T>(
    topic, 
    10, 
    std::bind(&Subscriber::topic_callback_wrapper<T>(topic), 
    this, 
    _1));

    __subscriptions[topic] = {subscription, NULL}

    return quadlink::ConnectionStatus::Finished;
}

template <typename T>
T quadlink::Subscriber::read_from_topic(const std::string topic)
{
    if (__subscriptions.find(topic) == __subscriptions.end()){
        /**
         * Creates a new subscription if one does not exist.
         */
        this->add_subscriber<T>(topic);
    }
    
    rclcpp::spin_some(shared_from_this());

    T msg = std::any_cast<T>(__subscriptions[topic].second)

    return msg
}