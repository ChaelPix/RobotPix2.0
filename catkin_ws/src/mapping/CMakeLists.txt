cmake_minimum_required(VERSION 3.0.2)
project(mapping)

find_package(catkin REQUIRED COMPONENTS
  explore_lite
  gmapping
  move_base
  roscpp
  rplidar_ros
  std_msgs
)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES ${PROJECT_NAME}
  CATKIN_DEPENDS roscpp std_srvs std_msgs
)

# Inclure les répertoires
include_directories(
  ${catkin_INCLUDE_DIRS}
)

# Ajouter l'exécutable
add_executable(${PROJECT_NAME}_node src/mapping_node.cpp)

# Ajouter les dépendances
add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

# Lier les bibliothèques
target_link_libraries(${PROJECT_NAME}_node ${catkin_LIBRARIES})

add_executable(map_monitor src/map_monitor.cpp)

target_link_libraries(map_monitor ${catkin_LIBRARIES})
