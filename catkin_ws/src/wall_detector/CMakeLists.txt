cmake_minimum_required(VERSION 3.0.2)
project(wall_detector)

# Trouver les packages nécessaires
find_package(catkin REQUIRED COMPONENTS
  roscpp
  sensor_msgs
  std_msgs
)

# Déclarer la configuration du package
catkin_package(
  INCLUDE_DIRS include
  LIBRARIES ${PROJECT_NAME}
  CATKIN_DEPENDS roscpp sensor_msgs std_msgs
)

# Inclure les répertoires
include_directories(
  ${catkin_INCLUDE_DIRS}
)

# Ajouter l'exécutable
add_executable(${PROJECT_NAME}_node src/wall_detector.cpp)
add_executable(${PROJECT_NAME}_tech src/wall_techControl.cpp)

# Ajouter les dépendances
add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
add_dependencies(${PROJECT_NAME}_tech ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

# Lier les bibliothèques
target_link_libraries(${PROJECT_NAME}_node ${catkin_LIBRARIES})
target_link_libraries(${PROJECT_NAME}_tech ${catkin_LIBRARIES})