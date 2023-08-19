def initialize(): 
    global robotState #0: Booting, #1: PackageSelecting, #2 SettingsMenu, #3 AskToStop
    robotState = 0 

    global loadedScript
    loadedScript = 0
    
    global loadedScriptName
    loadedScriptName = "None"

    global scriptRunning
    scriptRunning = True