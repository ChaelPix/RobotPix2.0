import usb.core
import os

device = None

def find_device(vendor_id, product_id):
    return usb.core.find(idVendor=vendor_id, idProduct=product_id)

def disable_device():
    global device
    bus = device.bus
    address = device.address
    path = f"/dev/bus/usb/{bus:03d}/{address:03d}"
    with open(path + "/authorized", "w") as f:
        f.write("0")

def enable_device():
    global device
    bus = device.bus
    address = device.address
    path = f"/dev/bus/usb/{bus:03d}/{address:03d}"
    with open(path + "/authorized", "w") as f:
        f.write("1")

def initialize():
    VENDOR_ID = 0x10c4
    PRODUCT_ID = 0xea60

    global device
    device = find_device(VENDOR_ID, PRODUCT_ID)
    disable_device()
