#include <rclcpp/rclcpp.hpp>

#include "bievr_lio_ros2/compressor_node.h"

int main(int argc, char** argv) {
  rclcpp::init(argc, argv);

  // Topic to compress is provided as the first positional argument.
  if (argc < 2) {
    RCLCPP_ERROR(rclcpp::get_logger("process_cloud"), "Usage: process_cloud <topic>");
    return -1;
  }

  const std::string topic = argv[1];
  auto node = std::make_shared<bievr::PointCloudCompressionNode>("bievr_lio_compression_node", topic);

  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
