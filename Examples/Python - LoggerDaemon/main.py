import Queue, threading, time

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_barometer import Barometer

from Logic import CSVData, CSVWriter


HOST = "localhost"
PORT = 4223
UID = "fVP"  # Change to your UID


q = Queue.Queue()
THREADS = []
EXIT_FLAG = 0
#CB_SUM / CB_COUNT = CB_MED
#CB_MED / CB_COUNT = Thread time for each write
THREAD_SLEEP = (3500+2000)/1000/2/2


DEFAULT_FILE_PATH = "logged_data.csv"

"""CALLBACKS"""
# Callback function for air pressure callback (parameter has unit mbar/1000)
def cb_air_pressure(air_pressure):
    csv = CSVData(UID, "Barometer", "AIR_PRESSURE", air_pressure)
    # print('Air Pressure CSV:\n' + csv.to_string())
    print "CB--Air Pressure: " + str(air_pressure)
    q.put(csv)

# Callback function for altitude callback (parameter has unit cm)
def cb_altitude(altitude):
    csv = CSVData(UID, "Barometer", "ALTITUDE", altitude) 
    # print('Altitude CSV:\n' + csv.to_string())
    print "CB--Altitude    : " + str(altitude)
    q.put(csv)

"""WRITER_THREAD"""
def writer_thread():
    csv_writer = CSVWriter(DEFAULT_FILE_PATH)
    
    while (True):
        if not q.empty():
            csv_data = q.get()
            #print "WR--THREAD      : %s" % (str(data.raw_data))
            if not csv_writer.write_data_row(csv_data):
                print "csv_writer.write_data_row failed!"
        #TODO: sleep time?
        time.sleep(THREAD_SLEEP)
        
        if(EXIT_FLAG and q.empty()): 
            
            exit = csv_writer.close_file()
            if exit:
                print "csv_writer closed!"
            else:
                print "csv_writer not closed! -> " + str(exit)
            break
        

    
"""MAIN"""
if __name__ == '__main__':    
    
    """PARSE-INI"""    
    # TODO
    
    """START-WRITE-THREAD"""
    t = threading.Thread(name="Writer_Thread", target=writer_thread)
    THREADS.append(t)
    t.start()
    
    """CREATE-CONNECTION-TO-MASTERBRICK"""
    ipcon = IPConnection()  # Create IP connection
    b = Barometer(UID, ipcon)  # Create device object

    ipcon.connect(HOST, PORT)  # Connect to brickd
    # Don't use device before ipcon is connected
    
    """SWITCH-CASE"""
    # TODO
    
    """INIT-CALLBACK"""


    
    """END_CONDITIONS"""
    raw_input('Press key to close\n')  # Use input() in Python 3
    ipcon.disconnect()
    
    # Notify threads it's time to exit
    EXIT_FLAG = 1
    # Wait for all threads to complete
    for t in THREADS:
        t.join()
    print "THREADS stopped"
    
    """DEBUG-READ-CSV"""  
    # read
   # read_file(DEFAULT_FILE_PATH)