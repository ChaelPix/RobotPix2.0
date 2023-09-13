import sys
import os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from grove_rgb_lcd import *
import time

import subprocess
import time

import robotPix_buttonsHandler
import robotPix_globalVars

import rospy
import std_msgs
from std_srvs.srv import Empty
from std_msgs.msg import String
from geometry_msgs.msg import Vector3

#----Settings-------
leftButtonPin = 24 #button 0
rightButtonPin = 26 #button 1

#--Pckgs
packageNames = ["wall_detector", "wall_detector", "mouse_control", "itinerary_follower", "itinerary_follower"]
nodeLaunchNames = ["wall_techControl.launch", "mapping.launch", "mapping_manual.launch", "follow_itineraries.launch", "complete_run.launch"]
hasPackageLidar = [1, 1]
#-------------------
roscore_process = None
rosserial_process = None
__all__ = ["GroveLedButton"]
pub = rospy.Publisher('rostopic', std_msgs.msg.Float32, queue_size=10)
pubArduinoInit = rospy.Publisher('/initArduino', std_msgs.msg.Empty, queue_size=10)


#----------ROS---------------------
def start_roscore():
    global roscore_process
    roscore_process = subprocess.Popen(["roscore"])
    time.sleep(2)

def start_rosserial():
    global rosserial_process
    rosserial_process = subprocess.Popen(["rosrun", "rosserial_python", "serial_node.py", "/dev/ttyMegaPi"])

def SetLCDText(data):
    setText(data.data)

def CallButton(data):
    robotPix_buttonsHandler.SetButtonState(data.x, data.y)

def start_ros():
    setText("    Starting\n  Rosserial...")
    print("\n Start Rosserial")
    start_rosserial()

    rospy.init_node('dashboard_main')

    rospy.Subscriber('setLcdText', String, SetLCDText)
    rospy.Subscriber('setButtonState', Vector3, CallButton)


def bool_isRosRunning():
    return (rosserial_process.poll() is None)

def stop_roscore():
    subprocess.run(["killall", "-9", "rosmaster"], check=True)


def stop_rosserial():
    global rosserial_process
    if rosserial_process is not None:
        rosserial_process.terminate()
        rosserial_process.wait()  
        rosserial_process = None


#--------Launch Control-------------
def StartRobot():
    print("\n Starting Package")
    setText("Starting Package..")
    
    print("main loadedscript: ")
    print(robotPix_globalVars.loadedScript)
    command = f"roslaunch {packageNames[robotPix_globalVars.loadedScript]} {nodeLaunchNames[robotPix_globalVars.loadedScript]}"
    subprocess.Popen(command, shell=True)

    pubArduinoInit.publish(std_msgs.msg.Empty())

    setText("Robot Pix Power!")
    print("\n Package Launched: " + nodeLaunchNames[robotPix_globalVars.loadedScript])

def StopRobot():
    setText("|ARRET DU ROBOT|")

    #subprocess.run(["rostopic", "pub", "-1", "/stopRos", "std_msgs/Float32", "0"])
    pub.publish(std_msgs.msg.Float32(0))
    print("\n /stopRos published")
    robotPix_buttonsHandler.SetButtonState(0, 0)
    robotPix_buttonsHandler.SetButtonState(1, 0)
    time.sleep(0.7)

    subprocess.run(["rosnode", "kill", "-a"])
    print("\n Ros nodes killed")

    # stop_roscore()
    # print("\n Roscore Killed")
    
def shutdown_roscore():
    os.system("pkill -f roscore")


def RestartRobotAferStop():
    setText("   Booting...")
    print("\n Booting...")
    start_rosserial()
    time.sleep(5)
    robotPix_globalVars.robotState = 2
    robotPix_buttonsHandler.SettingsSelector(0)
    print("Roscore and Rosserial are ready.")


def startRos():
    rospy.init_node('dashboard_sub')
    rospy.spin()

#----------MAIN--------------------
def main():
    robotPix_buttonsHandler.initialize()
    setText("   Booting...")
    print("\n Booting...")

    hasRosBoot = False
    while not hasRosBoot:  
        start_ros()
        time.sleep(10)
        hasRosBoot = bool_isRosRunning()
        if(not hasRosBoot):
            setText("Booting Failed.    Retrying...")
       
    
    robotPix_globalVars.initialize()
    robotPix_globalVars.robotState = 2
    robotPix_buttonsHandler.SettingsSelector(0)
    print("Roscore and Rosserial are ready.")

    while robotPix_globalVars.scriptRunning:
        time.sleep(1)

if __name__ == '__main__':
    main()
