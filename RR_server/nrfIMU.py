import serial
import struct
import time
import RobotRaconteur as RR
import thread
import threading
import numpy
import traceback
import socket
import time
import sys


RRN = RR.RobotRaconteurNode.s

serial_port_name="/dev/ttyACM0"

def get_open_port():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind(('127.0.0.1', 0))
    port = sock.getsockname()
    sock.close()
    time.sleep(3)
    return port[1]

class NrfImuInterface(object):
    def __init__(self):
        self._lock=threading.RLock()
        self._serial=serial.Serial(serial_port_name,baudrate=57600)


    def IMU1_read(self):
        with self._lock:
            dat=struct.pack("BB",50,0)
            self._serial.write(dat)
            raw = self._serial.read(size = 14)
            unConverted = struct.unpack( "hhhhhhh", raw )
            converted = [unConverted[0]/16384.0, unConverted[1]/16384.0,unConverted[2]/16384.0,unConverted[3]/131.0,unConverted[4]/131.0,unConverted[5]/131]
            return converted

    def IMU2_read(self):
        with self._lock:
            dat=struct.pack("BB",50,1)
            self._serial.write(dat)
            raw = self._serial.read(size = 14)
            unConverted = struct.unpack( "hhhhhhh", raw )
            converted = [unConverted[0]/16384.0, unConverted[1]/16384.0,unConverted[2]/16384.0,unConverted[3]/131.0,unConverted[4]/131.0,unConverted[5]/131]
            return converted
		     

def main():
    t1 = RR.LocalTransport()
    t1.StartServerAsNodeName("arduinoIMU")
    RRN.RegisterTransport(t1)
    

    port = get_open_port()
    print "Accepting commands on port: " + str(port)
    t2 = RR.TcpTransport()
    t2.EnableNodeAnnounce()
    t2.StartServer(port)
    RRN.RegisterTransport(t2)

    with open('nrfIMU.robodef', 'r') as f:
        service_def = f.read()
    
    myGateway = NrfImuInterface()

    RRN.RegisterServiceType(service_def)
    RRN.RegisterService("arduinoIMUData", "arduinoIMU.NrfImuInterface", myGateway)

    raw_input("Press any key to end")

    RRN.Shutdown()
  


if __name__ == '__main__':
    main()
