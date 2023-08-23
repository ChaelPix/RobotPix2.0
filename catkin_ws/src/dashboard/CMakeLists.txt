cmake_minimum_required(VERSION 3.0.2)
project(dashboard)

find_package(catkin REQUIRED COMPONENTS
  rospy
  std_msgs
)

# Déclarer la configuration du package
catkin_package(
  LIBRARIES ${PROJECT_NAME}
  CATKIN_DEPENDS rospy std_msgs
)

# Inclure les répertoires
include_directories(
  ${catkin_INCLUDE_DIRS}
)

catkin_install_python(PROGRAMS
  scripts/main_robotPix.py
  scripts/robotPix_buttonsHandler.py
  scripts/grove_LedButton.py
  scripts/grove_rgb_lcd.py
  scripts/robotPix_globalVars.py

  scripts/factory/__init__.py
  scripts/factory/factory.py
  
  scripts/button/__init__.py
  scripts/button/button_gpio.py
  scripts/button/button_i2c.py
  scripts/button/button.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

