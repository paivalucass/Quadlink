#include "subscriber.h"

int main(int argc, char * argv[])
  {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<quadlink::Subscriber>());
    rclcpp::shutdown();
    return 0;
  }