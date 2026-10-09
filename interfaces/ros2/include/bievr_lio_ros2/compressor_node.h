#ifndef BIEVR_LIO_ROS2_COMPRESSOR_NODE_H_
#define BIEVR_LIO_ROS2_COMPRESSOR_NODE_H_

#include <point_cloud_transport/point_cloud_codec.hpp>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>

namespace bievr {

class PointCloudCompressionNode : public rclcpp::Node {
 public:
  explicit PointCloudCompressionNode(const std::string& node_name, const std::string& topic)
      : rclcpp::Node(node_name) {

    compressed_pub_ = create_generic_publisher(topic + "/compressed",
                                              "point_cloud_interfaces/msg/CompressedPointCloud2",
                                               rclcpp::SensorDataQoS());

    cloud_sub_ = create_subscription<sensor_msgs::msg::PointCloud2>(topic, rclcpp::SensorDataQoS().keep_last(1),
                                                                    std::bind(&PointCloudCompressionNode::onCloud, this, std::placeholders::_1));
  }

 private:
  void onCloud(sensor_msgs::msg::PointCloud2::ConstSharedPtr cloud) {
    rclcpp::SerializedMessage serialized;

    if (!codec_.encode("draco", *cloud, serialized)) {
      RCLCPP_ERROR_THROTTLE(get_logger(), *get_clock(), 5000, "Failed to compress point cloud");
      return;
    }

    compressed_pub_->publish(serialized);
  }

  point_cloud_transport::PointCloudCodec codec_;
  rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr cloud_sub_;
  std::shared_ptr<rclcpp::GenericPublisher> compressed_pub_;
};

}  // namespace bievr
#endif  // BIEVR_LIO_ROS2_COMPRESSOR_NODE_H_
