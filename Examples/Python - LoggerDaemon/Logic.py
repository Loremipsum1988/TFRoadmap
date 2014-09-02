import datetime #CSV_Data
import os #CSV_Writer
import sys #CSV_Writer
import csv #CSV_Writer

class CSVData(object):
    '''
    classdocs
    '''    
    
    def __init__(self, uid, name, var_name, raw_data):
        '''
        Constructor
        '''        
        self.uid = uid
        self.name = name;
        self.var_name = var_name
        self.raw_data = raw_data
        self.__set_timestamp()
        
        
    def __set_timestamp(self):
        self.timestamp = datetime.datetime.now()
    
    def to_string(self):
        return "UID =" + self.uid + "\nNAME=" + self.name + "\nVAR =" + self.var_name + "\nRAW =" + str(self.raw_data) + "\nTIME=" + str(self.timestamp) + "\n"
    
    
class CSVWriter(object):
    '''
    classdocs
    '''    
    
    def __init__(self, file_path):
        '''
        Constructor
        ''' 
        self.file_path = file_path
        self.raw_file = None
        self.csv_file = None
        
        self.__open_file_A()
        """
        1. Open File
        2. Check if File is empty
            2.a) If Empty, write the Header
        3. File is ready to be used
        4. Write Data Rows in File
        5. Close File
        """
    
    def __open_file_A(self):
        print "Open CSV File: " + self.file_path

        """newline problem solved + import sys"""
        if sys.version_info >= (3, 0, 0):
            self.raw_file = open(self.file_path, 'a', newline='')
        else:
            self.raw_file = open(self.file_path, 'ab')
        
        self.csv_file = csv.writer(self.raw_file, delimiter=";", quotechar='"', quoting=csv.QUOTE_MINIMAL)
        
        if self.__file_is_empty():
            self.__write_header()

    def __file_is_empty(self):
        try:
            if os.stat(self.file_path).st_size > 0:
                return False
            else:
                return True
        except OSError:
            return True

    def __write_header(self):
        print "Write CSV Header "
        if(not self.__file_is_empty()):
            print "File is not Empty"
            return
    
        self.csv_file.writerow(["UID"] + ["NAME"] + ["VAR"] + ["RAW"] + ["TIMESTAMP"])
    
    
    def write_data_row(self, csv_data):
        if self.raw_file == None or self.csv_file == None:
            return False
        
        print "TH--CSV WRITE   : " +str(csv_data.raw_data)
        self.csv_file.writerow([csv_data.uid] + [csv_data.name] + [csv_data.var_name] + [csv_data.raw_data] + [csv_data.timestamp])        
        return True
    
    def set_file_path(self, new_file_path):
        if self.file_path == new_file_path:
            return True
        
        if not self.close_file():
            print "CSV_Writer.set_file_path(" + new_file_path + ") failed!"
            return False
        
        self.file_path = new_file_path
        self.__open_file_A()
        return True
                          
    def reopen_file(self):
        if self.raw_file != None and self.csv_file != None:
            return False
        
        self.__open_file_A()        
        return True
    
    def close_file(self):
        if self.raw_file == None or self.csv_file == None:
            return False
        try:
            self.raw_file.close()
            self.csv_file = None
            self.raw_file = None
            return True
        
        except ValueError:
            return False