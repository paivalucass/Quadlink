#include "subscriber.h"
#include "publisher.h"

int main(int argc, char * argv[])
  {
    rclcpp::init(argc, argv);
    sensor_msgs::msg::Image image_out;
    auto subscriber_node = std::make_shared<quadlink::Subscriber>();
    auto publisher_node = std::make_shared<quadlink::Publisher>();
    while (rclcpp::ok()){
      image_out = subscriber_node->read_from_topic<sensor_msgs::msg::Image>("/camera1/image_raw", subscriber_node);
      publisher_node->publish_on_topic<sensor_msgs::msg::Image>("/tests_image", image_out);
    }
    rclcpp::shutdown();
    return 0;
  }