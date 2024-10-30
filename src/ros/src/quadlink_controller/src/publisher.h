#ifndef QUADLINK_PUBLISHER_H
#define QUADLINK_PUBLISHER_H
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
#include "subscriber.h"

namespace quadlink{


    class Publisher : public rclcpp::Node, public std::enable_shared_from_this<Subscriber>{
        public: 
            Publisher();

            ~Publisher() = default;

            template <typename DataType>
            StatusROS add_publisher(const std::string topic);

            template <typename DataType>
            void publish_on_topic(const std::string topic, DataType data);

        private:
            std::unordered_map<std::string, rclcpp::PublisherBase::SharedPtr> __publishers;
    };

    template <typename DataType>
    quadlink::StatusROS quadlink::Publisher::add_publisher(const std::string topic){

        __publishers[topic] = this->create_publisher<DataType>(topic, 10);

        std::this_thread::sleep_for(std::chrono::seconds(2));

        return quadlink::StatusROS::Success;
    }

    template <typename DataType>
    void publish_on_topic(const std::string topic, DataType data){
        if (__publishers.find(topic) == __publishers.end()){

            this->add_publisher<DataType>(topic);

        }

        __publishers[topic]->publish(data);

    }
}
#endif