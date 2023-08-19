#!/usr/bin/env python
#
#
from __future__ import print_function
import _rpi_vl53l0x as vl

VERSION_REQUIRED_MAJOR = 1
VERSION_REQUIRED_MINOR = 0
VERSION_REQUIRED_BUILD = 2

_GPIO_NEW_SAMPLE_READY = 0x04

class VL53L0X(object):
    def __init__(self, address = 0x29, bus = 0x1):
        self.dev_addr = address
        self.dev = vl.new_VL53L0X_Dev_t()
        vl.VL53L0X_Dev_t_I2cDevAddr_set(self.dev, self.dev_addr)

        fd = vl.VL53L0X_i2c_init("/dev/i2c-%d" % bus, self.dev_addr)
        if fd < 0:
            self._err = vl.VL53L0X_ERROR_CONTROL_INTERFACE
            print("Failed to initialize i2c interface")
        else:
            self._err = vl.VL53L0X_ERROR_NONE
            vl.VL53L0X_Dev_t_fd_set(self.dev, fd)

    def __del__(self):
        st = vl.VL53L0X_StopMeasurement(self.dev)
        if st == vl.VL53L0X_ERROR_NONE:
            st = self.__wait_stoped()
        if st == vl.VL53L0X_ERROR_NONE:
            vl.VL53L0X_ClearInterruptMask(self.dev, _GPIO_NEW_SAMPLE_READY)
        vl.delete_VL53L0X_Dev_t(self.dev)
        vl.VL53L0X_i2c_close()
        self._err = st

    def get_libver(self):
        version = None
        ver = vl.new_VL53L0X_Version_t()
        st = vl.VL53L0X_GetVersion(ver)
        if st == vl.VL53L0X_ERROR_NONE:
            major = vl.VL53L0X_Version_t_major_get(ver)
            minor = vl.VL53L0X_Version_t_minor_get(ver)
            build = vl.VL53L0X_Version_t_build_get(ver)
            revision = vl.VL53L0X_Version_t_revision_get(ver)
            version = {
                'major'   : major,
                'minor'   : minor,
                'build'   : build,
                'revision': revision,
            }
            '''
            if major != VERSION_REQUIRED_MAJOR or \
               minor != VERSION_REQUIRED_MINOR or build != VERSION_REQUIRED_BUILD:
                print("VL53L0X API Version Error: ")
                print("  Your firmware has %d.%d.%d (revision %d)." % (major, minor, build, revision))
                print("  This example requires %d.%d.%d.\n" % \
                    (VERSION_REQUIRED_MAJOR, VERSION_REQUIRED_MINOR, VERSION_REQUIRED_BUILD))
            '''
        vl.delete_VL53L0X_Version_t(ver)
        self._err = st
        return version

    def get_devver(self):
        version = None
        devinfo = vl.new_VL53L0X_DeviceInfo_t()
        st = vl.VL53L0X_GetDeviceInfo(self.dev, devinfo)
        if st == vl.VL53L0X_ERROR_NONE:
            version = {
                'type' : vl.VL53L0X_DeviceInfo_t_Type_get(devinfo),
                'name' : vl.VL53L0X_DeviceInfo_t_Name_get(devinfo),
                'id'   : vl.VL53L0X_DeviceInfo_t_ProductId_get(devinfo),
                'major': vl.VL53L0X_DeviceInfo_t_ProductRevisionMajor_get(devinfo),
                'minor': vl.VL53L0X_DeviceInfo_t_ProductRevisionMinor_get(devinfo),
            }
        vl.delete_VL53L0X_DeviceInfo_t(devinfo)
        self._err = st
        return version

    def begin(self):
        refSpandCount   = vl.new_uint32_t(1)
        isApertureSpads = vl.new_uint8_t(1)
        VhvSettings     = vl.new_uint8_t(1)
        PhaseCal        = vl.new_uint8_t(1)

        st = vl.VL53L0X_DataInit(self.dev)
        if st == vl.VL53L0X_ERROR_NONE:
            st = vl.VL53L0X_StaticInit(self.dev)
        if st == vl.VL53L0X_ERROR_NONE:
            st = vl.VL53L0X_PerformRefCalibration(self.dev, VhvSettings, PhaseCal)
            '''
            print("VhvSettings = {} PhaseCal = {}".format(
                vl.uint8_t___getitem__(VhvSettings,0),
                vl.uint8_t___getitem__(PhaseCal,0)))
            '''
        if st == vl.VL53L0X_ERROR_NONE:
            st = vl.VL53L0X_PerformRefSpadManagement(self.dev, refSpandCount, isApertureSpads)
        if st == vl.VL53L0X_ERROR_NONE:
            st = vl.VL53L0X_SetDeviceMode(self.dev, vl.VL53L0X_DEVICEMODE_CONTINUOUS_RANGING)
        if st == vl.VL53L0X_ERROR_NONE:
            st = vl.VL53L0X_StartMeasurement(self.dev)

        vl.delete_uint8_t(PhaseCal)
        vl.delete_uint8_t(VhvSettings)
        vl.delete_uint8_t(isApertureSpads)
        vl.delete_uint32_t(refSpandCount)
        self._err = st
        return st

    def __wait_stoped(self):
        st = vl.VL53L0X_ERROR_NONE
        stop_completed = vl.new_uint32_t(1)

        loop_nr = 0
        while True:
            st = vl.VL53L0X_GetStopCompletedStatus(self.dev, stop_completed)
            if vl.uint32_t___getitem__(stop_completed, 0) == 0 or \
              st != vl.VL53L0X_ERROR_NONE:
                break
            loop_nr += 1
            vl.VL53L0X_PollingDelay(self.dev)
            if loop_nr >= vl.VL53L0X_DEFAULT_MAX_LOOP:
                st = vl.VL53L0X_ERROR_TIME_OUT
                break
        vl.delete_uint32_t(stop_completed)
        return st

    def is_ready(self):
        data_ready = vl.new_uint8_t(1)
        ret_val = False

        st = vl.VL53L0X_GetMeasurementDataReady(self.dev, data_ready)
        if st != vl.VL53L0X_ERROR_NONE:
            ret_val = None
        elif vl.uint8_t___getitem__(data_ready,0) == 0x01:
            ret_val = True

        vl.delete_uint8_t(data_ready)
        self._err = st
        return ret_val

    def wait_ready(self):
        '''
        return None  -- Error status
               False -- Timeout
               True  -- Ready
        '''
        loop_nr = 0
        while True:
            r = self.is_ready()
            if r is None or r:
                return r
            loop_nr += 1
            vl.VL53L0X_PollingDelay(self.dev)
            if loop_nr >= vl.VL53L0X_DEFAULT_MAX_LOOP:
                break
        return False

    def get_distance(self):
        measured = vl.new_VL53L0X_RangingMeasurementData_t()
        st = vl.VL53L0X_GetRangingMeasurementData(self.dev, measured)
        dis = vl.VL53L0X_RangingMeasurementData_t_RangeMilliMeter_get(measured)
        vl.VL53L0X_ClearInterruptMask(self.dev, _GPIO_NEW_SAMPLE_READY)
        vl.delete_VL53L0X_RangingMeasurementData_t(measured)
        self._err = st
        return dis

def main():
    print("VL53L0X Ranging example")

    vl53 = VL53L0X()
    vl53.begin()

    '''
    version = vl53.get_devver()
    print("VL53L0X_GetDeviceInfo:")
    print("Device Type   : %s" % version["type"])
    print("Device Name   : %s" % version["name"])
    print("Device ID     : %s" % version["id"])
    print("RevisionMajor : %d" % version["major"])
    print("RevisionMinor : %d" % version["minor"])
    '''

    st = vl53.wait_ready()
    if st:
        print("Distance = {} mm".format(vl53.get_distance()))


if __name__ == '__main__':
    main()
