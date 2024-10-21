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
#include "../../../connection/quad_connection.h"

using std::placeholders::_1;

namespace quadlink {

  class Subscriber : public rclcpp::Node, public std::enable_shared_from_this<Subscriber>
  {
    public:
      Subscriber(); 

      template <typename T>
      quadlink::ConnectionStatus add_subscriber(const std::string topic);

      template <typename T>
      T read_from_topic(const std::string topic);
    private:
      template <typename T>    
      std::function<void(const T&)> topic_callback_wrapper(const std::string& topic_name);

      template <typename T>
      void topic_callback(const std::string topic, const T & msg);
      
      std::unordered_map<std::string, std::pair<rclcpp::SubscriptionBase::SharedPtr, std::any>> __subscriptions;
  };
}