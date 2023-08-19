from grove_rgb_lcd import *
import time
from grove.button import Button
from grove.factory import Factory

import subprocess

__all__ = ["GroveLedButton"]
loadedScript = 0
scriptPin = 24 
controlPin = 26
import subprocess
import signal
import time

roscore_process = None

    def start_roscore():
        global roscore_process
        roscore_process = subprocess.Popen(["roscore"])
        time.sleep(2)  # Attendre un court instant pour que roscore démarre complètement

    def start_rosserial():
        global rosserial_process
        rosserial_process = subprocess.Popen(["rosrun", "rosserial_python", "serial_node.py", "/dev/ttyAMA0"])

    def stop_roscore():
        global roscore_process
        if roscore_process is not None:
            roscore_process.terminate()
            roscore_process.wait()  # Attendre que le processus se termine complètement
            roscore_process = None
    def stop_rosserial():
        global rosserial_process
        if rosserial_process is not None:
            rosserial_process.terminate()
            rosserial_process.wait()  # Attendre que le processus se termine complètement
            rosserial_process = None

    def StartRobot():
        setText("Starting Roscore..")
        print("\n Start Roscore")
        start_roscore()

        setText("Starting Rosserial..")
        print("\n Start Rosserial")
        time.sleep(5)
        start_rosserial()

        time.sleep(5)
        print("\n tarting Package")
        setText("Starting Package..")

        command = f"/home/robot/RobotPix/ros_catkin_ws/devel/bin/roslaunch pkg_avancer pkg_avancer.launch"
        subprocess.Popen(command, shell=True)
        print("\n Package Launched")
        setText("Moving Forward !")
    def StopRobot():
        setText("Robot Stopped !")
        print("\n Roscore Stopped")
        subprocess.run(["rostopic", "pub", "-1", "/stopRos", "std_msgs/Float32", "0"])
        print("\n Stopped PUB")
        stop_rosserial()
        stop_roscore()

    def ShowScenarioName(id):
        if(id == 0):
            setText("Avancer")
        else:
            setText("Reculer")
    class GroveLedButton(object):
        def __init__(self, pin):
            # High = light on
            self.led = Factory.getOneLed("GPIO-HIGH", pin)
            # Low = pressed
            self.btn = Factory.getButton("GPIO-LOW", pin + 1)
            self.__on_event = None
            self.btn.on_event(self, GroveLedButton.__handle_event)
            self.pin = pin
            self.led.light(False)
        @property
        def on_event(self):
            return self.__on_event
        @on_event.setter
        def on_event(self, callback):
            if not callable(callback):
                return
            self.__on_event = callback
        def __handle_event(self, evt):
            if callable(self.__on_event):
                # the customized behavior
                self.__on_event(evt['index'], evt['code'], evt['time'])
                return
            # the default behavior
            self.led.brightness = self.led.MAX_BRIGHT
            event = evt['code']
            if event & Button.EV_SINGLE_CLICK:
                self.checkEvent(self.pin, 0)
            #elif event & Button.EV_DOUBLE_CLICK:
            #    self.led.blink()
            #    setText("Script #1 loaded")
            elif event & Button.EV_LONG_PRESS:
                self.checkEvent(self.pin, 2)
        def checkEvent(self, pin, action):
            global controlPin
            if (pin == controlPin):
                if(action == 0):
                    self.led.light(False)
                    StopRobot()
                elif(action == 2):
                    self.led.light(True)
                    StartRobot()
            
            elif (pin == scriptPin):
                if(action == 0):
                    self.nextScript()
                elif(action == 2):
                    global loadedScript
                    ShowScenarioName(loadedScript)
        def nextScript(self):
            global loadedScript
            loadedScript = (loadedScript+1)%2
            ShowScenarioName(loadedScript)
            
    def main():
        print("Program Started")
        setText("Welcome!")
        controlBtn = GroveLedButton(controlPin)
        scriptBtn = GroveLedButton(scriptPin)
        while True:
            time.sleep(1)

    if __name__ == '__main__':
        main()