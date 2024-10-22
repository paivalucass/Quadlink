#ifndef QUADLINK_SUBSCRIBER_H
#define QUADLINK_SUBSCRIBER_H
#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <any>



#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "sensor_msgs/msg/image.hpp"

using std::placeholders::_1;

namespace quadlink {
  enum class StatusROS{
      Success,
      Failed
  };


  class Subscriber : public rclcpp::Node, public std::enable_shared_from_this<Subscriber>
  {
    public:
      Subscriber(); 

      ~Subscriber() = default;

      template <typename T>
      quadlink::StatusROS add_subscriber(const std::string topic);

      template <typename T>
      T read_from_topic(const std::string topic, std::shared_ptr<quadlink::Subscriber> node_ptr);
    private:
      template <typename T>    
      std::function<void(const T&)> topic_callback_wrapper(const std::string& topic_name);

      template <typename T>
      void topic_callback(const std::string topic, const T & msg);
      
      std::unordered_map<std::string, std::pair<rclcpp::SubscriptionBase::SharedPtr, std::any>> __subscriptions;
  };


template <typename T>
std::function<void(const T&)> quadlink::Subscriber::topic_callback_wrapper(const std::string& topic_name){
    std::cout << "hey" << std::endl;
    return [this, topic_name](const T & msg) {
        topic_callback(topic_name, msg);
    };
}

template <typename T>
void quadlink::Subscriber::topic_callback(const std::string topic, const T & msg) 
{
    __subscriptions[topic].second = msg;
}

template <typename T>
quadlink::StatusROS quadlink::Subscriber::add_subscriber(const std::string topic)
{
    // parametrize queue size?
    /**
     * Creates a subscription in this node with a msgs type given by the user.
     * 
     */
    auto subscription = this->create_subscription<T>(
    topic, 
    10, 
    quadlink::Subscriber::topic_callback_wrapper<T>(topic) 
    );

    __subscriptions[topic] = {subscription, nullptr};

    return quadlink::StatusROS::Success;
}

template <typename T>
T quadlink::Subscriber::read_from_topic(const std::string topic, std::shared_ptr<quadlink::Subscriber> node_ptr)
{
    if (__subscriptions.find(topic) == __subscriptions.end()){
        /**
         * Creates a new subscription if one does not exist.
         */
        this->add_subscriber<T>(topic);
    }
    
    std::cout << topic << std::endl;

    rclcpp::spin_some(node_ptr);

    const auto& any_msg = __subscriptions[topic].second;
    if (any_msg.type() != typeid(T)) {
        // Print type information for debugging
        RCLCPP_ERROR(this->get_logger(), "Type mismatch. Expected %s but got %s",
                    typeid(T).name(), any_msg.type().name());
        throw std::bad_any_cast();
    }

    auto msg = std::any_cast<T>(__subscriptions[topic].second);

    return msg;
}

}

#endif