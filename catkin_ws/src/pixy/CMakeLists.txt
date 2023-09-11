cmake_minimum_required(VERSION 3.0.2)
project(pixy)
set(CMAKE_CXX_STANDARD 17)

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
add_executable(${PROJECT_NAME}_analyzer src/pixy_info_analyzer.cpp)
add_executable(${PROJECT_NAME}_qrcode_demo src/pixy_qrcode_demo.cpp)

# Add dependencies
add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
add_dependencies(${PROJECT_NAME}_analyzer ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
add_dependencies(${PROJECT_NAME}_qrcode_demo ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

# Link the libraries
target_link_libraries(${PROJECT_NAME}_node ${catkin_LIBRARIES} zbar) 
target_link_libraries(${PROJECT_NAME}_analyzer ${catkin_LIBRARIES} zbar) 
target_link_libraries(${PROJECT_NAME}_qrcode_demo ${catkin_LIBRARIES} zbar) 