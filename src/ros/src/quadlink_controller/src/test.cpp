#include "camera_subscriber.h"

int main(int argc, char * argv[])
  {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<quadlink::CameraSubscriber>());
    rclcpp::shutdown();
    return 0;
  }