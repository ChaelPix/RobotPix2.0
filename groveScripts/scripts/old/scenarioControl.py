from grove_rgb_lcd import *
import time
from grove.button import Button
from grove.factory import Factory


__all__ = ["GroveLedButton"]
loadedScript = 0

class GroveLedButton(object):
    def __init__(self, pin):
        # High = light on
        self.led = Factory.getOneLed("GPIO-HIGH", pin)
        # Low = pressed
        self.btn = Factory.getButton("GPIO-LOW", pin + 1)
        self.__on_event = None
        self.btn.on_event(self, GroveLedButton.__handle_event)
        self.pin = pin
        if(pin == 22):
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
        if (pin == 24):
            if(action == 0):
                self.led.light(False)
                setText("Robot stopped!")
            elif(action == 2):
                self.led.light(True)
                setText("Starting Robot..")
        
        elif (pin == 26):
             if(action == 0):
                self.nextScript()
             elif(action == 2):
                global loadedScript
                setText("Script #" + str(loadedScript + 1) + " is    currently loaded") 

    def nextScript(self):
        global loadedScript
        loadedScript = (loadedScript+1)%3
        setText("Script #" + str(loadedScript + 1) + " loaded") 

def main():
    print("Program Started")
    scriptPin = 24 
    controlPin = 26

    controlBtn = GroveLedButton(controlPin)
    scriptBtn = GroveLedButton(scriptPin)

    while True:
        time.sleep(1)

if __name__ == '__main__':
    main()


