import Queue

from Logic import CSVData

from tinkerforge.ip_connection import IPConnection
from threading import Timer


HOST = "localhost"
PORT = 4223
ipcon = None

###Variables###
q = Queue.Queue()
timers = []

class LoggerTimer(object):
    def __init__(self, interval, func):
        interval /= 1000 #for ms
        if interval < 0:
            interval = 0
        
        self.__interval = interval
        self.__func = func      
        self.__t = Timer(self.__interval, self.__loop)
    
    def __loop(self):
        #print "FUNC"
        self.__func()
        self.cancel()
        self.__t = Timer(self.__interval, self.__loop)
        self.start()
        
    
    def start(self):
        if self.__interval == 0:
            self.cancel()
            return     
        #print "START -> " + str(self.__interval)   
        self.__t.start()
    
    def cancel(self):
        #print "CANCEL"
        self.__t.cancel()
        
    def join(self):
        if self.__interval == 0: #quick fix for no timer.start()
            return
        self.__t.join();

###Sections###
GENERAL_SECTION = "GENERAL"
GENERAL_LOG_TO_FILE = "log_to_file"
GENERAL_PATH_TO_FILE = "path_to_file"

XIVELY_SECTION = "XIVELY"
XIVELY_ACTIVE = "active"
XIVELY_AGENT_DESCRIPTION = "agent_description"
XIVELY_FEED = "feed"
XIVELY_API_KEY = "api_key"
XIVELY_UPDATE_RATE = "update_rate"
###Bricklets and Variables###
#TODO: What to do with the REACHED Callbacks?

#Ambient Light
AMBIENT_LIGHT = "Ambient Light"
AMBIENT_LIGHT_ILLUMINANCE = "Illuminance"
AMBIENT_LIGHT_ANALOG_VALUE = "Analog Value"

#Analog In    
#Analog Out
#Barometer
from tinkerforge.bricklet_barometer import Barometer

BAROMETER = "Barometer"
BAROMETER_AIR_PRESSURE = "Air Pressure"
BAROMETER_ALTITUDE = "Altitude"

class Barometer_Bricklet():
    
    def __init__(self, uid):
        self.uid = uid
        self.__device = Barometer(self.uid, ipcon)

    def start_timer(self, data):
        
        t = LoggerTimer(1000, self.__timer_air_pressure)
        timers.append(t)
        t.start()
        
        t = LoggerTimer(1000, self.__timer_altitude)
        timers.append(t)
        t.start()
        
#         self.__timer_air_pressure()
#         t = Timer(5, self.__timer_air_pressure)        
#         t.start()
        #TODO: check with hasKey


    def __timer_air_pressure(self):
        value = self.__device.get_air_pressure()
        csv = CSVData(self.uid, BAROMETER, BAROMETER_AIR_PRESSURE, value)
        q.put(csv)
        print q.get().to_string()
        
    def __timer_altitude(self):
        value = self.__device.get_altitude()
        csv = CSVData(self.uid, BAROMETER, BAROMETER_ALTITUDE, value)
        q.put(csv)   
        print q.get().to_string() 

#Breakout
#Color
#Current12
#Current25
#Distance IR
#Distance US
#Dual Button
#Dual Relay
#GPS
#Hall Effect
#Humidity
HUMIDITY = "Humidity"
HUMIDITY_HUMIDITY = "Humidity"
HUMIDITY_ANALOG_VALUE = "Analog Value"
#Industrial Digital In 4
#Industrial Digital Out 4
#Industrial Dual 0-20mA
#Industrial Quad Relay
#IO-16
#IO-4
#Joystick
#LCD 16x2
#LCD 20x4
#LED Strip
#Line
#Linear Poti
#Moisture
#Motion Detector
#Multi Touch
#NFC/RFID
#Piezo Buzzer
#Piezo Speaker
#PTC
#Remote Switch
#Rotary Encoder
#Rotary Poti
#Segment Display 4x7
#Solid State Relay
#Sound Intensity
#Temperature
#Temperature IR
#Tilt
#Voltage
#Voltage/Current

class Bricklet_Info(object):
    
    def __init__(self, uid, name, variables):
        self.uid = uid
        self.name = name
        self.variables = variables
        
    def to_string(self):
        print "[----------------------------------------------------------"
        print "uid: " + self.uid + "; name: " + self.name + "; variables: "
        print self.variables
        print "----------------------------------------------------------]"

def main_switch(data):  
    general_switch(data)
    xively_switch(data)
    bricklet_switch(data)

def general_switch(data):
    pass

def xively_switch(data):
    pass

def bricklet_switch(data):
    tmp = data[0]
    tmp.to_string()
    b = Barometer_Bricklet(tmp.uid)
    b.start_timer(tmp.variables)
        

if __name__ == '__main__':      
    ipcon = IPConnection()  # Create IP connection
  

    ipcon.connect(HOST, PORT)  # Connect to brickd
    # Don't use device before ipcon is connected
    #DUMMYS
    bricklets = []
    b1 = Bricklet_Info("fVP", BAROMETER, {BAROMETER_AIR_PRESSURE:0,BAROMETER_ALTITUDE:0})
    b2 = Bricklet_Info("Amb", AMBIENT_LIGHT, {AMBIENT_LIGHT_ANALOG_VALUE:1250,AMBIENT_LIGHT_ILLUMINANCE:3000})
    b3 = Bricklet_Info("Hum", HUMIDITY, {HUMIDITY_ANALOG_VALUE:1111,HUMIDITY_HUMIDITY:2750})
    bricklets.append(b1)
    bricklets.append(b2)
    bricklets.append(b3)
    
    main_switch(bricklets)
    
    raw_input('Press key to close\n')  # Use input() in Python 3
    
    #stop Timers
    for t in timers:
        t.cancel()
    #check if timer stopped
    for t in timers:
        t.join()
    
    ipcon.disconnect()