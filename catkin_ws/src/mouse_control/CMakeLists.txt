cmake_minimum_required(VERSION 3.0.2)
project(mouse_control)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
)

find_library(LIBEVDEV_LIBRARY evdev)

# Déclarer la configuration du package
catkin_package(
  LIBRARIES ${PROJECT_NAME}
  CATKIN_DEPENDS roscpp std_msgs
)

# Inclure les répertoires
include_directories(
  ${catkin_INCLUDE_DIRS}
  ${LIBEVDEV_INCLUDE_DIRS}
)

# Ajouter l'exécutable
add_executable(${PROJECT_NAME}_node src/robot_mouseControl.cpp)

# Ajouter les dépendances
add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

# Lier les bibliothèques
target_link_libraries(${PROJECT_NAME}_node ${catkin_LIBRARIES} ${LIBEVDEV_LIBRARY})