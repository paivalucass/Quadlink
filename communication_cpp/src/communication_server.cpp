#include "communication/communication_server.hpp"

/**
 * @brief A class representing the communication node for handling various drone commands.
 */
class CommunicationNode : public rclcpp::Node {
public:
    /**
     * @brief Constructor for CommunicationNode.
     * @param fc A unique pointer to the flight controller interface.
     */
    CommunicationNode(std::unique_ptr<rcpilot::FlightControllerInterface> fc)
        : Node("communication"), flight_controller(std::move(fc)) {

        // Create services for various drone commands
        service_server_takeoff = this->create_service<library::srv::Action>(
            "communication_server_takeoff",
            std::bind(&CommunicationNode::communication_server_takeoff_callback,
            this, std::placeholders::_1, std::placeholders::_2));

        service_server_land = this->create_service<library::srv::Action>(
            "communication_server_land",
            std::bind(&CommunicationNode::communication_server_land_callback,
            this, std::placeholders::_1, std::placeholders::_2));

        service_server_arm = this->create_service<library::srv::Action>(
            "communication_server_arm",
            std::bind(&CommunicationNode::communication_server_arm_callback,
            this, std::placeholders::_1, std::placeholders::_2));

        service_server_connect = this->create_service<library::srv::Action>(
            "communication_server_connect",
            std::bind(&CommunicationNode::communication_server_connect_callback,
            this, std::placeholders::_1, std::placeholders::_2));

        service_server_change_flight_mode = this->create_service<library::srv::Action>(
            "communication_server_change_flight_mode",
            std::bind(&CommunicationNode::communication_server_change_mode_callback,
            this, std::placeholders::_1, std::placeholders::_2));

        service_server_goto_body = this->create_service<library::srv::Movement>(
            "communication_server_goto_body",
            std::bind(&CommunicationNode::communication_server_goto_body_callback,
            this, std::placeholders::_1, std::placeholders::_2));

        service_server_goto_ned = this->create_service<library::srv::Movement>(
            "communication_server_goto_ned",
            std::bind(&CommunicationNode::communication_server_goto_ned_callback,
            this, std::placeholders::_1, std::placeholders::_2));

        service_server_change_velocity_body = this->create_service<library::srv::Movement>(
            "communication_server_change_velocity_body",
            std::bind(&CommunicationNode::communication_server_change_velocity_body_callback,
            this, std::placeholders::_1, std::placeholders::_2));

        service_server_yaw = this->create_service<library::srv::Movement>(
            "communication_server_yaw",
            std::bind(&CommunicationNode::communication_server_yaw_callback,
            this, std::placeholders::_1, std::placeholders::_2));

        RCLCPP_INFO(rclcpp::get_logger("communication_node"), "%sCOMMUNICATION SERVERS SUCCESSFULLY STARTED%s", LogColors::GREEN.c_str(), LogColors::RESET.c_str());
    }

private:

    /**
     * @brief Logs the connection status with appropriate messages.
     * @param status The connection status.
     * @param success_msg The success message.
     * @param timeout_msg The timeout message.
     * @param failure_msg The failure message.
     */
    void log_connection_status(rcpilot::ConnectionStatus status, const std::string& success_msg, const std::string& timeout_msg, const std::string& failure_msg) {
        switch (status) {
            case rcpilot::ConnectionStatus::Finished:
                RCLCPP_INFO(rclcpp::get_logger("communication_node"), "%s%s%s", LogColors::GREEN.c_str(), success_msg.c_str(), LogColors::RESET.c_str());
                break;
            case rcpilot::ConnectionStatus::Timeout:
                RCLCPP_WARN(rclcpp::get_logger("communication_node"), "%s%s%s", LogColors::YELLOW.c_str(), timeout_msg.c_str(), LogColors::RESET.c_str());
                break;
            default:
                RCLCPP_ERROR(rclcpp::get_logger("communication_node"), "%s%s%s", LogColors::RED.c_str(), failure_msg.c_str(), LogColors::RESET.c_str());
                break;
        }
    }

    /**
     * @brief Callback for the takeoff service.
     * @param request The service request.
     * @param response The service response.
     */
    void communication_server_takeoff_callback(const std::shared_ptr<library::srv::Action::Request> request,
        std::shared_ptr<library::srv::Action::Response> response) {

        flight_controller->set_takeoff_height(request->act.payload_number);
        rcpilot::ConnectionStatus status = flight_controller->takeoff();

        log_connection_status(status, "COMMUNICATION SERVER: TAKEOFF", "COMMUNICATION SERVER: TIMEOUT WHEN TAKING OFF", "COMMUNICATION SERVER: FAILED TO TAKEOFF");

        response->result = static_cast<uint8_t>(status);
    }

    /**
     * @brief Callback for the land service.
     * @param request The service request.
     * @param response The service response.
     */
    void communication_server_land_callback(const std::shared_ptr<library::srv::Action::Request> request,
        std::shared_ptr<library::srv::Action::Response> response) {

        rcpilot::ConnectionStatus status = flight_controller->land();

        log_connection_status(status, "COMMUNICATION SERVER: LANDING", "COMMUNICATION SERVER: TIMEOUT WHEN LANDING", "COMMUNICATION SERVER: FAILED TO LAND");

        response->result = static_cast<uint8_t>(status);
    }

    /**
     * @brief Callback for the arm service.
     * @param request The service request.
     * @param response The service response.
     */
    void communication_server_arm_callback(const std::shared_ptr<library::srv::Action::Request> request,
        std::shared_ptr<library::srv::Action::Response> response) {

        rcpilot::ConnectionStatus status = flight_controller->arm();

        log_connection_status(status, "COMMUNICATION SERVER: ARMING", "COMMUNICATION SERVER: TIMEOUT WHEN ARMING", "COMMUNICATION SERVER: FAILED TO ARM");

        response->result = static_cast<uint8_t>(status);
    }

    /**
     * @brief Callback for the connect service.
     * @param request The service request.
     * @param response The service response.
     */
    void communication_server_connect_callback(const std::shared_ptr<library::srv::Action::Request> request,
        std::shared_ptr<library::srv::Action::Response> response) {

        rcpilot::ConnectionStatus status = flight_controller->connect(request->act.payload_string);

        log_connection_status(status, "COMMUNICATION SERVER: SUCCESSFULLY CONNECTED TO FLIGHT CONTROLLER", "COMMUNICATION SERVER: CONNECTION TO FLIGHT CONTROLLER TIMEOUT", "COMMUNICATION SERVER: CONNECTION TO FLIGHT CONTROLLER FAILED");

        response->result = static_cast<uint8_t>(status);
    }

    /**
     * @brief Callback for the change flight mode service.
     * @param request The service request.
     * @param response The service response.
     */
    void communication_server_change_mode_callback(const std::shared_ptr<library::srv::Action::Request> request,
        std::shared_ptr<library::srv::Action::Response> response) {

        rcpilot::ConnectionStatus status = flight_controller->change_mode(request->act.payload_enum);

        log_connection_status(status, "COMMUNICATION SERVER: SUCCESSFULLY CHANGED FLIGHT MODE", "COMMUNICATION SERVER: TIMEOUT WHEN CHANGING FLIGHT MODE", "COMMUNICATION SERVER: FAILED TO CHANGE FLIGHT MODE");

        response->result = static_cast<uint8_t>(status);
    }

    /**
     * @brief Callback for the goto body service.
     * @param request The service request.
     * @param response The service response.
     */
    void communication_server_goto_body_callback(const std::shared_ptr<library::srv::Movement::Request> request,
        std::shared_ptr<library::srv::Movement::Response> response) {

        double x = request->mov.target_pos.x;
        double y = request->mov.target_pos.y;
        double z = request->mov.target_pos.z;

        rcpilot::ConnectionStatus status = flight_controller->goto_body(x, y, (-z));

        log_connection_status(status, "COMMUNICATION SERVER: SUCCESSFULL GOTO BODY", "COMMUNICATION SERVER: TIMEOUT WHEN GOTO BODY", "COMMUNICATION SERVER: FAILED WHEN GOTO BODY");

        response->result = static_cast<uint8_t>(status);
    }

    /**
     * @brief Callback for the goto NED service.
     * @param request The service request.
     * @param response The service response.
     */
    void communication_server_goto_ned_callback(const std::shared_ptr<library::srv::Movement::Request> request,
        std::shared_ptr<library::srv::Movement::Response> response) {

        double x = request->mov.target_pos.x;
        double y = request->mov.target_pos.y;
        double z = request->mov.target_pos.z;

        rcpilot::ConnectionStatus status = flight_controller->goto_ned(x, y, (-z));

        log_connection_status(status, "COMMUNICATION SERVER: SUCCESSFULL GOTO NED", "COMMUNICATION SERVER: TIMEOUT WHEN GOTO NED", "COMMUNICATION SERVER: FAILED WHEN GOTO NED");

        response->result = static_cast<uint8_t>(status);
    }

    /**
     * @brief Callback for the change velocity body service.
     * @param request The service request.
     * @param response The service response.
     */
    void communication_server_change_velocity_body_callback(const std::shared_ptr<library::srv::Movement::Request> request,
        std::shared_ptr<library::srv::Movement::Response> response) {

        double vx = request->mov.vx;
        double vy = request->mov.vy;
        double vz = request->mov.vz;

        rcpilot::ConnectionStatus status = flight_controller->change_body_velocity(vx, vy, vz);

        log_connection_status(status, "COMMUNICATION SERVER: SUCCESSFULL VELOCITY CHANGE", "COMMUNICATION SERVER: TIMEOUT WHEN CHANGING VELOCITY", "COMMUNICATION SERVER: FAILED TO CHANGE VELOCITY");

        response->result = static_cast<uint8_t>(status);
    }

    /**
     * @brief Callback for the yaw service.
     * @param request The service request.
     * @param response The service response.
     */
    void communication_server_yaw_callback(const std::shared_ptr<library::srv::Movement::Request> request,
        std::shared_ptr<library::srv::Movement::Response> response) {

        double yaw = request->mov.yaw;
        double yaw_rate = request->mov.yaw_rate;

        flight_controller->set_yaw_rate(yaw_rate);

        rcpilot::ConnectionStatus status = flight_controller->yaw(yaw);

        log_connection_status(status, "COMMUNICATION SERVER: SUCCESSFULL YAW", "COMMUNICATION SERVER: TIMEOUT WHEN YAW", "COMMUNICATION SERVER: FAILED TO YAW");

        response->result = static_cast<uint8_t>(status);
    }

    // Service pointers for various drone commands
    rclcpp::Service<library::srv::Action>::SharedPtr service_server_takeoff;
    rclcpp::Service<library::srv::Action>::SharedPtr service_server_land;
    rclcpp::Service<library::srv::Action>::SharedPtr service_server_arm;
    rclcpp::Service<library::srv::Action>::SharedPtr service_server_connect;
    rclcpp::Service<library::srv::Action>::SharedPtr service_server_change_flight_mode;
    rclcpp::Service<library::srv::Movement>::SharedPtr service_server_goto_body;
    rclcpp::Service<library::srv::Movement>::SharedPtr service_server_goto_ned;
    rclcpp::Service<library::srv::Movement>::SharedPtr service_server_change_velocity_body;
    rclcpp::Service<library::srv::Movement>::SharedPtr service_server_yaw;
    std::unique_ptr<rcpilot::FlightControllerInterface> flight_controller; ///< Unique pointer to the flight controller interface.
};

/**
 * @brief Main function to initialize and run the communication node.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status.
 */
int main(int argc, char **argv) {
    rclcpp::init(argc, argv);

    // TODO: Add config file for choosing controller. This should be a ROS2 PARAM

    auto flight_controller = std::make_unique<rcpilot::ControllerPixhawkArdupilot>();
    auto node = std::make_shared<CommunicationNode>(std::move(flight_controller));

    rclcpp::spin(node);
    rclcpp::shutdown();
}