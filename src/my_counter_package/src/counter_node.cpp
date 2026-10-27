#include <chrono>
#include <memory>
#include <string>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"
#include "std_srvs/srv/set_bool.hpp"

using namespace std::chrono_literals;

class CounterNode : public rclcpp::Node
{
public:
    CounterNode() : Node("counter_node"), count_(0), is_running_(true)
    {
        // Create Publisher in topic "counter"
        publisher_ = this->create_publisher<std_msgs::msg::Int32>("counter", 10);

        // Create Service for (start/stop)
        service_ = this->create_service<std_srvs::srv::SetBool>(
            "toggle_counter",
            std::bind(&CounterNode::handle_toggle, this, std::placeholders::_1, std::placeholders::_2));

        // Timer, which run every 2 seconds
        timer_ = this->create_wall_timer(2s, std::bind(&CounterNode::timer_callback, this));
        
        RCLCPP_INFO(this->get_logger(), "Counter Node (C++) with service toggle_counter running.");
    }

private:
    void timer_callback()
    {
        if (is_running_) {
            auto message = std_msgs::msg::Int32();
            message.data = count_;
            RCLCPP_INFO(this->get_logger(), "Send: %d", message.data);
            publisher_->publish(message);
            count_++;
        }
    }

    void handle_toggle(
        const std::shared_ptr<std_srvs::srv::SetBool::Request> request,
        std::shared_ptr<std_srvs::srv::SetBool::Response> response)
    {
        is_running_ = request->data;
        std::string status = is_running_ ? "running" : "stop";
        
        response->success = true;
        response->message = "Counter success " + status + ".";
        
        RCLCPP_INFO(this->get_logger(), "Service running. Counter %s.", status.c_str());
    }

    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr publisher_;
    rclcpp::Service<std_srvs::srv::SetBool>::SharedPtr service_;
    int32_t count_;
    bool is_running_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<CounterNode>());
    rclcpp::shutdown();
    return 0;
}
