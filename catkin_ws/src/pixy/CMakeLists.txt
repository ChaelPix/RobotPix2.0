cmake_minimum_required(VERSION 3.0.2)
project(pixy)

find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  image_transport
  roscpp
  sensor_msgs
  std_msgs
)

catkin_package(
  LIBRARIES ${PROJECT_NAME}
  CATKIN_DEPENDS cv_bridge image_transport roscpp sensor_msgs std_msgs
)

include_directories(
  ${catkin_INCLUDE_DIRS}
  /usr/include/zbar/ 
)

# Add the executable
add_executable(${PROJECT_NAME}_node src/pixy_reader.cpp)

# Add dependencies
add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

# Link the libraries
target_link_libraries(${PROJECT_NAME}_node ${catkin_LIBRARIES} zbar) 

