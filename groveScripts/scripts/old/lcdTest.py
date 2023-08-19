from grove_rgb_lcd import *
import smbus

setText("test i2c")

bus = smbus.SMBus(1)  # Assurez-vous d'utiliser le bon bus I2C (1 pour le Raspberry Pi 3 et supérieur)

for address in range(0x03, 0x78):  # Vérifier les adresses I2C de 0x03 à 0x77
    try:
        bus.write_quick(address)
        print(f"Adresse trouvée : 0x{address:02X}")
    except Exception as e:
        pass

#setRGB(0, 255, 255)
