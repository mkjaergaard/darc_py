cmake_minimum_required(VERSION 2.8)
project(darc_py)

find_package(catkin)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES darc_component
#  CATKIN_DEPENDS darc_common iris darc_ns darc_pubsub darc_procedure
)

find_package(catkin REQUIRED COMPONENTS darc_component)

include_directories(include)
include_directories(${catkin_INCLUDE_DIRS})

add_subdirectory(pybindings)
