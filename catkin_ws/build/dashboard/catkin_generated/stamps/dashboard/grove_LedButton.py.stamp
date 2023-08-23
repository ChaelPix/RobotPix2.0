from grove.button import Button
from grove.factory import Factory

class GroveLedButton(object):
    def __init__(self, pin, button_action_callback):

        self.pin = pin
        self.led = Factory.getOneLed("GPIO-HIGH", pin)
        self.btn = Factory.getButton("GPIO-LOW", pin + 1)
        self.led.light(False)

        self.__on_event = None
        self.btn.on_event(self, GroveLedButton.__handle_event)
        self.button_action_callback = button_action_callback

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
            self.__on_event(evt['index'], evt['code'], evt['time'])
            return

        self.led.brightness = self.led.MAX_BRIGHT
        event = evt['code']
        if event & Button.EV_SINGLE_CLICK:
            self.button_action_callback(0, self.pin)

        #elif event & Button.EV_DOUBLE_CLICK:
            #self.button_action_callback(2, self.pin)

        elif event & Button.EV_LONG_PRESS:
            self.button_action_callback(1, self.pin)