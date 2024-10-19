#include "camera_subscriber.h"


quadlink::CameraSubscriber::CameraSubscriber() : Node("minimal_subscriber")
{
    subscription_ = this->create_subscription<std_msgs::msg::String>(
    "topic", 10, std::bind(&CameraSubscriber::topic_callback, this, _1));
}

void quadlink::CameraSubscriber::topic_callback(const std_msgs::msg::String & msg) 
{
    RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg.data.c_str());
}
