#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using std::placeholders::_1;
namespace quadlink {

  class CameraSubscriber : public rclcpp::Node
  {
    public:
      CameraSubscriber();

    private:
      void topic_callback(const std_msgs::msg::String & msg);
      rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
  };
}