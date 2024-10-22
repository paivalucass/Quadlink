#include "subscriber.h"

int main(int argc, char * argv[])
  {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<quadlink::Subscriber>();
    node->read_from_topic<sensor_msgs::msg::Image>("/camera1/image_raw", node);
    rclcpp::shutdown();
    return 0;
  }