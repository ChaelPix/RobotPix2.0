from grove_rgb_lcd import *
import time

import subprocess
import time

import robotPix_buttonsHandler
import robotPix_globalVars

#----Settings-------
leftButtonPin = 24 #button 0
rightButtonPin = 26 #button 1

#--Pckgs
packageNames = ["wall_detector"]
nodeLaunchNames = ["wall_detector.launch"]

#-------------------
roscore_process = None
rosserial_process = None
__all__ = ["GroveLedButton"]

#----------ROS---------------------
def start_roscore():
    global roscore_process
    roscore_process = subprocess.Popen(["roscore"])
    time.sleep(2)

def start_rosserial():
    global rosserial_process
    rosserial_process = subprocess.Popen(["rosrun", "rosserial_python", "serial_node.py", "/dev/ttyUSB0"])

def start_ros():
    setText("    Starting\n   Roscore...")
    print("\n Start Roscore")
    start_roscore()
    
    setText("    Starting\n  Rosserial...")
    print("\n Start Rosserial")
    start_rosserial()

def bool_isRosRunning():
    return (roscore_process.poll() is None) and (rosserial_process.poll() is None)

def stop_roscore():
    global roscore_process
    if roscore_process is not None:
        roscore_process.terminate()
        roscore_process.wait() 
        roscore_process = None

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

    command = f"roslaunch {packageNames[robotPix_globalVars.loadedScript]} {nodeLaunchNames[robotPix_globalVars.loadedScript]}"
    subprocess.Popen(command, shell=True)

    setText("Robot Pix Power!")
    print("\n Package Launched")

def StopRobot():
    setText("Robot Stopped !")
    print("\n Roscore Stopped")
    subprocess.run(["rostopic", "pub", "-1", "/stopRos", "std_msgs/Float32", "0"])
    print("\n Stopped PUB")
    stop_roscore()
    stop_rosserial()

#----------MAIN--------------------
def main():

    setText("   Booting...")
    print("\n Booting...")
    robotPix_buttonsHandler.initialize()

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
