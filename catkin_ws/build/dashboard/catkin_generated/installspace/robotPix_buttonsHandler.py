from grove_rgb_lcd import *
import main_robotPix
import robotPix_globalVars

import subprocess
from grove_LedButton import GroveLedButton 

from std_msgs.msg import String
from geometry_msgs.msg import Vector3
#------------------
LeftButton = 0
RightButton = 1
Click = 0
Hold = 1
LeftButtonClicked = 0
LeftButtonHeld = 1
RightButtonClicked = 2
RightButtonHeld = 3
#------------------
packagesDisplayNames = ["Detection Mur", "Creer la Map", "Ctrl Souris", "Suivre Points", "Run Complete"]
settingsDisplayNames = ["Choix du Package", "Desactiver Robot", "Eteindre Raspi"]
#------------------
rightBtn = None
leftBtn = None
selectedSetting = 0
isOff = False

def getButtonInput(action, buttonPin):
    print("action : " + str(action) + " button : " + str(buttonPin))
    buttonId = LeftButton if buttonPin == main_robotPix.leftButtonPin else RightButton

    if(buttonId == LeftButton):
        if(action == Click):
            return LeftButtonClicked
        elif(action == Hold):
            return LeftButtonHeld
    elif(buttonId == RightButton):
        if(action == Click):
            return RightButtonClicked
        elif(action == Hold):
            return RightButtonHeld



#------Button EVENT-----------------
def buttonAction(action, buttonPin): 
    buttonInput = getButtonInput(action, buttonPin)
    robotState = robotPix_globalVars.robotState
    
    print(buttonInput)
    print(robotState)

    #Package Selection
    if(robotState == 1):
        if(buttonInput == LeftButtonClicked):
             PackageSelector(-1)
        elif(buttonInput == RightButtonClicked):
             PackageSelector(1)
        elif(buttonInput == LeftButtonHeld): #Start
             ChangeRobotState(5)
             AskToStart()
        elif(buttonInput == RightButtonHeld): 
             ChangeRobotState(2)
             SettingsSelector(0)

    #Settings Selection
    elif(robotState == 2):
        if(buttonInput == LeftButtonClicked):
             SettingsSelector(-1)
        elif(buttonInput == RightButtonClicked):
             SettingsSelector(1)
        elif(buttonInput == LeftButtonHeld):
             SettingsAction()

    #Asking to Stop code
    elif(robotState == 3):
        if(buttonInput == LeftButtonClicked):
             StopCode()
        elif(buttonInput == RightButtonClicked):
             ChangeRobotState(2)
             SettingsSelector(0)

    #Asking to Stop Pi
    elif(robotState == 4):
        if(buttonInput == LeftButtonClicked):
             StopPi()
        elif(buttonInput == RightButtonClicked):
             ChangeRobotState(1)
             SettingsSelector(0)

    #Asking to Start
    elif(robotState == 5):
        if(buttonInput == LeftButtonClicked):
             StartRobot()
             ChangeRobotState(6)
        elif(buttonInput == RightButtonClicked):
             ChangeRobotState(2)
             SettingsSelector(0)

    #Running, Force Stop
    elif(robotState == 6):
        if(buttonInput == RightButtonClicked):
             ChangeRobotState(-1)
             StopRobot()

    #Stopped, reboot
    elif(robotState == 7):
        if(buttonInput == LeftButtonClicked):
                main_robotPix.RestartRobotAferStop()
        if(buttonInput == RightButtonClicked):
                StopCode()
        if(buttonInput == RightButtonHeld):
                StopPi()
            
             
             
def ChangeRobotState(newState):
     robotPix_globalVars.robotState = newState

#---Robot State = 1 ------
def PackageSelector(x):
    id = robotPix_globalVars.loadedScript + x    
    if(id >= len(packagesDisplayNames)):
         id = 0
    elif(id < 0):
         id = len(packagesDisplayNames) - 1

    robotPix_globalVars.loadedScript = id
    setText("#" + str(id+1) 
            + ":" +  packagesDisplayNames[id]
            +"\n<-|Strt  Opts|->")
    
    print(id)
    print(robotPix_globalVars.loadedScript)
    
#---Robot State = 2 ------
def SettingsSelector(x):
    global selectedSetting
    selectedSetting += x    
    if(selectedSetting >= len(settingsDisplayNames)):
         selectedSetting = 0
    elif(selectedSetting < 0):
         selectedSetting = len(settingsDisplayNames) - 1

    setText(settingsDisplayNames[selectedSetting]
            +"\n<-|Choose     ->")
    

#---------------------------------
def SettingsAction():
    global selectedSetting
    if(selectedSetting == 0):
        ChangeRobotState(1)
        PackageSelector(0)
    elif(selectedSetting == 1):
        AskToStopCode()
    elif(selectedSetting == 2):
        AskToStopRapi()

def AskToStopCode():
    ChangeRobotState(3)
    setText("Stopper le Code?" + "\nOui          Non")

def StopCode():
    main_robotPix.StopRobot()
    global isOff
    if(isOff):
         return
    isOff = True
    SetButtonState(0, 0)
    setText("----------------Robot Desactive")
    main_robotPix.shutdown_roscore()
    robotPix_globalVars.scriptRunning = False


def AskToStopRapi():
    ChangeRobotState(4)
    setText("Eteindre Raspi?" + "\nOui          Non")

def StopPi():
    global isOff
    if(isOff):
         return
    isOff = True
    setText("----------------Raspi Desactivee")
    subprocess.Popen(['sudo','poweroff'])
    #subprocess.run(["systemctl", "start", "poweroff_nopass.service"])

#---------------------------------

def AskToStart():
    global leftBtn
    leftBtn.led.light(False)
    setText("Demarrer Robot ?" + "\nOui          Non")

def StartRobot():
    global rightBtn
    rightBtn.led.light(True)
    main_robotPix.StartRobot()

def StopRobot():
    setText("Arret!")
    main_robotPix.StopRobot()
    global rightBtn
    rightBtn.led.light(False)
    time.sleep(2)
    global leftBtn
    setText("Robot Arrete... <-On  Off/Stp|->")
    leftBtn.led.light(True)
    ChangeRobotState(7)

#Main
def initialize():
    global rightBtn, leftBtn
    rightBtn = GroveLedButton(main_robotPix.rightButtonPin, buttonAction)
    leftBtn = GroveLedButton(main_robotPix.leftButtonPin, buttonAction)

    rightBtn.led.light(False)
    leftBtn.led.light(False)


def SetButtonState(ledId, action):
    if(ledId== 0):
         global leftBtn
         leftBtn.led.light(action == 1)
    elif(ledId== 1):
         global rightBtn
         rightBtn.led.light(action == 1)


#global rightBtn
#rightBtn.led.light(True)