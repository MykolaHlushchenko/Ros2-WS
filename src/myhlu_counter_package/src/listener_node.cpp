#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"

class ListenerNode : public rclcpp::Node
{
public:
    ListenerNode() : Node("listener_node")
    {
        // Subscribe on topic "counter"
        subscription_ = this->create_subscription<std_msgs::msg::Int32>(
            "counter", 10, std::bind(&ListenerNode::topic_callback, this, std::placeholders::_1));
            
        RCLCPP_INFO(this->get_logger(), "Listener Node (C++) running. Waiting messages...");
    }

private:
    void topic_callback(const std_msgs::msg::Int32::SharedPtr msg) const
    {
        RCLCPP_INFO(this->get_logger(), "Get counter answer: %d", msg->data);
    }
    
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ListenerNode>());
    rclcpp::shutdown();
    return 0;
}
