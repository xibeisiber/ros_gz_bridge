// Copyright 2022 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "get_mappings.hpp"


bool find_command_option(const std::vector<std::string> & args, const std::string & option)
{
  return std::find(args.begin(), args.end(), option) != args.end();
}

bool get_flag_option(const std::vector<std::string> & args, const std::string & option)
{
  auto it = std::find(args.begin(), args.end(), option);
  return it != args.end();
}

int main(int argc, char * argv[])
{
  std::vector<std::string> args(argv, argv + argc);

  if (find_command_option(args, "-h") || find_command_option(args, "--help")) {
    std::stringstream ss;
    ss << "Usage:" << std::endl;
    ss << " -h, --help: This message." << std::endl;
    ss << " --print-pairs: Print a list of the supported ROS 2 <=> GZ conversion pairs.";
    ss << std::endl;
    ss << " --print-gz: Print a list of the supported Gazebo types.";
    ss << std::endl;
    ss << " --print-ros: Print a list of the supported ROS 2 types.";
    ss << std::endl;
    ss << " --get-gz-to-ros <MSG>: Get the ROS 2 mapping for an Gazebo Type.";
    ss << std::endl;
    ss << " --get-ros-to-gz <MSG>: Get the Gazebo mapping for a ROS 2 Type.";
    std::cout << ss.str();
    return 0;
  }

  if (get_flag_option(args, "--get-ros-to-gz")) {
    if (args.size() < 3) {
      std::cout << "get-ros-to-gz requires an argument" << std::endl;
      return 1;
    }

    std::string gz_type_name;
    auto found = ros_gz_bridge::get_ros_to_gz_mapping(args[2], gz_type_name);
    if (found) {
      std::cout << gz_type_name << std::endl;
      return 0;
    } else {
      std::cout << "Type [" << args[2] <<
        "] has no mapping, use --print-ros to see supported types" << std::endl;
      return 1;
    }
  } else if (get_flag_option(args, "--get-gz-to-ros")) {
    if (args.size() < 3) {
      std::cout << "--get-gz-to-ros requires an argument" << std::endl;
      return 1;
    }

    std::string ros_type_name;
    auto found = ros_gz_bridge::get_gz_to_ros_mapping(args[2], ros_type_name);
    if (found) {
      std::cout << ros_type_name << std::endl;
      return 0;
    } else {
      std::cout << "Type [" << args[2] <<
        "] has no mapping, use --print-gz to see supported types" << std::endl;
      return 1;
    }
  } else if (get_flag_option(args, "--print-gz")) {
    auto mappings = ros_gz_bridge::get_all_message_mappings_ros_to_gz();
    if (mappings.size() > 0) {
      for (auto & pair : mappings) {
        std::cout << pair.second << std::endl;
      }
    } else {
      std::cout << "No message type conversion pairs supported." << std::endl;
    }
  } else if (get_flag_option(args, "--print-ros")) {
    auto mappings = ros_gz_bridge::get_all_message_mappings_ros_to_gz();
    if (mappings.size() > 0) {
      for (auto & pair : mappings) {
        std::cout << pair.first << std::endl;
      }
    } else {
      std::cout << "No message type conversion pairs supported." << std::endl;
    }
  } else if (get_flag_option(args, "--print-pairs")) {
    auto mappings = ros_gz_bridge::get_all_message_mappings_ros_to_gz();
    if (mappings.size() > 0) {
      std::cout << "Supported ROS <=> Gazebo message type conversion pairs:" << std::endl;
      for (auto & pair : mappings) {
        std::cout << "  - '" << pair.first <<
          "' (ROS) <=> '" << pair.second << "' (GZ)" << std::endl;
      }
    } else {
      std::cout << "No message type conversion pairs supported." << std::endl;
    }
  }
}
