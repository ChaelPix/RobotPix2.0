import subprocess
import time
import signal
import os

from grove_rgb_lcd import *

def check_roscore_running():
    try:
        output = subprocess.check_output("pgrep -f roscore", shell=True)
        return True
    except subprocess.CalledProcessError:
        return False

def main():
    setText("    Starting\n   Roscore...")
    print("\n Start Roscore")
    roscore_process = subprocess.Popen("roscore", shell=True, preexec_fn=os.setsid)
    time.sleep(2)  


    dashboard_process = subprocess.Popen("rosrun dashboard main_robotPix.py", shell=True, preexec_fn=os.setsid)
    
    try:
        # Vérifiez régulièrement si roscore est toujours en cours d'exécution
        while check_roscore_running():
            time.sleep(5)
    except KeyboardInterrupt:
        pass
    finally:
        os.killpg(os.getpgid(roscore_process.pid), signal.SIGTERM)
        os.killpg(os.getpgid(dashboard_process.pid), signal.SIGTERM)

if __name__ == "__main__":
    main()
