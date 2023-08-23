from gpiozero import OutputDevice

#pin26 = InputDevice(26)
#pin27 = InputDevice(27)

#print(f"Pin 26: {'HIGH' if pin26.is_active else 'LOW'}")
#print(f"Pin 27: {'HIGH' if pin27.is_active else 'LOW'}")


pin26 = OutputDevice(26)

# Mettre les pins en état HIGH
pin26.on()
