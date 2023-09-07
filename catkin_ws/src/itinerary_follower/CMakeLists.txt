cmake_minimum_required(VERSION 3.0.2)
project(itinerary_follower)
set(CMAKE_CXX_STANDARD 17)

find_package(catkin REQUIRED COMPONENTS
  move_base_msgs
  sensor_msgs
  std_msgs
  roscpp
  actionlib
  geometry_msgs
  nav2d_navigator
)

# Déclarer la configuration du package
catkin_package(
  INCLUDE_DIRS include
  LIBRARIES ${PROJECT_NAME}
  CATKIN_DEPENDS roscpp sensor_msgs std_msgs move_base_msgs actionlib geometry_msgs nav2d_navigator
)

# Inclure les répertoires
include_directories(
  ${catkin_INCLUDE_DIRS}
)

# Ajouter l'exécutable
add_executable(${PROJECT_NAME}_moveBaseConverter src/moveBasetoArduino.cpp)
add_executable(${PROJECT_NAME}_node src/itinerary_controller.cpp)

# Ajouter les dépendances
add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
add_dependencies(${PROJECT_NAME}_moveBaseConverter ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

# Lier les bibliothèques
target_link_libraries(${PROJECT_NAME}_node ${catkin_LIBRARIES})
target_link_libraries(${PROJECT_NAME}_moveBaseConverter ${catkin_LIBRARIES})