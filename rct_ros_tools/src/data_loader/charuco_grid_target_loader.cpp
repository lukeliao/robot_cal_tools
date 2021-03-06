#include <rct_ros_tools/exceptions.h>
#include <rct_ros_tools/loader_utils.h>
#include <rct_ros_tools/target_loaders.h>
#include <rct_image_tools/charuco_grid_target.h>

const int DEFAULT_ARUCO_DICTIONARY = cv::aruco::DICT_6X6_250;

namespace rct_ros_tools
{
template<>
rct_image_tools::CharucoGridTarget TargetLoader<rct_image_tools::CharucoGridTarget>::load(const ros::NodeHandle& nh, const std::string& key)
{
  XmlRpc::XmlRpcValue xml;
  if (!nh.getParam(key, xml)) throw ros::InvalidParameterException(key);

  int rows = 0;
  int cols = 0;
  double chessboard_dim;
  double aruco_marker_dim;
  int dictionary;

  if (!read(xml, "cols", cols)) throw ros::InvalidParameterException(key + "/cols");
  if (!read(xml, "rows", rows)) throw ros::InvalidParameterException(key + "/rows");
  if (!read(xml, "chessboard_dim", chessboard_dim)) throw ros::InvalidParameterException(key + "/chessboard_dim");
  if (!read(xml, "aruco_marker_dim", aruco_marker_dim)) throw ros::InvalidParameterException(key + "/aruco_marker_dim");
  if (!read(xml, "dictionary", dictionary))
  {
    dictionary = DEFAULT_ARUCO_DICTIONARY;
    ROS_WARN_STREAM("Using default ArUco dictionary ID: " << DEFAULT_ARUCO_DICTIONARY);
  }

  return rct_image_tools::CharucoGridTarget(rows, cols, static_cast<float>(chessboard_dim), static_cast<float>(aruco_marker_dim), dictionary);
}

template<>
rct_image_tools::CharucoGridTarget TargetLoader<rct_image_tools::CharucoGridTarget>::load(const std::string &path)
{
  try
  {
    YAML::Node n = YAML::LoadFile(path);
    int cols = n["target_definition"]["cols"].as<int>();
    int rows = n["target_definition"]["rows"].as<int>();
    float chessboard_dim = n["target_definition"]["chessboard_dim"].as<float>();
    float aruco_marker_dim = n["target_definition"]["aruco_marker_dim"].as<float>();

    int dictionary;
    try
    {
      dictionary = n["target_definition"]["dictionary"].as<int>();
    }
    catch (const YAML::Exception&)
    {
      dictionary = DEFAULT_ARUCO_DICTIONARY;
      ROS_WARN_STREAM("Using default ArUco dictionary ID: " << DEFAULT_ARUCO_DICTIONARY);
    }

    return rct_image_tools::CharucoGridTarget(rows, cols, chessboard_dim, aruco_marker_dim, dictionary);
  }
  catch (const YAML::Exception &ex)
  {
    throw BadFileException(std::string("YAML failure: ") + ex.what());
  }
}

} // namespace rct_ros_tools
