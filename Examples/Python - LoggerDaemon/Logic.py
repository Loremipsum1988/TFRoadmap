import datetime #CSV_Data
import os #CSV_Writer
import sys #CSV_Writer
import csv #CSV_Writer

class CSVData(object):
    '''
    This class is used as a temporary save spot for all csv relevant data.
    '''    
    
    def __init__(self, uid, name, var_name, raw_data):
        '''
        uid      = uid of the bricklet
        name     = name of the bricklet
        var_name = variable name of the logged value
        raw_data = the logged value
        
        The timestamp is added automaticaly.
        '''        
        self.uid = uid
        self.name = name;
        self.var_name = var_name
        self.raw_data = raw_data
        self.timestamp = None
        self.__set_timestamp()
        
        
    def __set_timestamp(self):
        """
        Adds a timestamp in ISO 8601 standard, with ms
        ISO 8061 = YYYY-MM-DD hh:mm:ss.msmsms
        """
        self.timestamp = datetime.datetime.now()
    
    def to_string(self):
        """
        Simple Debug function for easier display of the object.
        """
        return "UID =" + self.uid + "\nNAME=" + self.name + "\nVAR =" + self.var_name + "\nRAW =" + str(self.raw_data) + "\nTIME=" + str(self.timestamp) + "\n"
    
    
class CSVWriter(object):
    '''
    This class is used for writing a csv file.
    '''    
    
    def __init__(self, file_path):
        '''
        file_path = Path to the csv file
        ''' 
        self.__file_path = file_path
        self.__raw_file = None
        self.__csv_file = None
        
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
        """Opens a file in append mode."""

        #newline problem solved + import sys
        if sys.version_info >= (3, 0, 0):
            self.__raw_file = open(self.__file_path, 'a', newline='')
        else:
            self.__raw_file = open(self.__file_path, 'ab')
        
        self.__csv_file = csv.writer(self.__raw_file, delimiter=";", quotechar='"', quoting=csv.QUOTE_MINIMAL)
        
        #if the file is empty, create a csv header
        if self.__file_is_empty():
            self.__write_header()

    def __file_is_empty(self):
        """
        Simple check if the file is empty.
        Return:
            True  - File is empty or missing
            False - File is not empty
        """
        try:
            if os.stat(self.__file_path).st_size > 0:
                return False
            else:
                return True
        except OSError:
            return True

    def __write_header(self):
        """Writes a csv header into the file"""
        if(not self.__file_is_empty()):
            return
    
        self.__csv_file.writerow(["UID"] + ["NAME"] + ["VAR"] + ["RAW"] + ["TIMESTAMP"])
    
    
    def write_data_row(self, csv_data):
        """
        Write a row into the csv file.
        Return:
            True  - Row was written into thee file
            False - Row was not written into the File
        """
        if self.__raw_file == None or self.__csv_file == None:
            return False
        
        self.__csv_file.writerow([csv_data.uid] + [csv_data.name] + [csv_data.var_name] + [csv_data.raw_data] + [csv_data.timestamp])        
        return True
    
    def set_file_path(self, new_file_path):
        """
        Sets a new file path.
        Return:
            True  - File path was updated and successfully opened
            False - File path could not be updated or opened
        """
        if self.__file_path == new_file_path:
            return True
        
        if not self.close_file():
            #print "CSV_Writer.set_file_path(" + new_file_path + ") failed!"
            return False
        
        self.__file_path = new_file_path
        self.__open_file_A()
        return True
                          
    def reopen_file(self):
        """
        Tries to reopen a file, if the file was manually closed.
        Return:
            True  - File could be reopened
            False - File could not be reopened
        """
        if self.__raw_file != None and self.__csv_file != None:
            return False
        
        self.__open_file_A()        
        return True
    
    def close_file(self):
        """
        Tries to close the current file.
        Return:
            True  - File was close
            False - File could not be closed
        """
        if self.__raw_file == None or self.__csv_file == None:
            return False
        try:
            self.__raw_file.close()
            self.__csv_file = None
            self.__raw_file = None
            return True
        
        except ValueError:
            return False