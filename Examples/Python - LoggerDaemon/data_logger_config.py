'''
@author: Roland Dudko
'''

import ConfigParser,codecs, os
from ConfigParser import SafeConfigParser


class Data_Logger_Config(object):
    '''
    This class provides the read-in functionality for the Data Logger configuration file
    '''
    __GENERAL_SECTION = "GENERAL"
    __XIVELY_SECTION = "XIVELY"
    
    __NAME_KEY = "name"
    __UID_KEY = "uid"

    def __init__(self,name):
        self.__isParsed = False
        self.filenName = name
        self.__general = {}
        self.__xively = {}
        self.__bricklets = []
        
        
    def __get_section_as_hashmap(self,section_name ,parser ):
        '''
        saves variables out of an section to a hashmap.
        '''
        hashMap = {}
        for section_key in parser.options(section_name):
            hashMap[section_key] =  parser.get(section_name, section_key)
        return hashMap     
    
    def read_config_file(self):
        '''
        reads the entries out of the configuration file and 
        saves them into a <Bricklet_Info> structure            
        '''
        parser = SafeConfigParser()
        # Open the file with the correct encoding
        with codecs.open(self.filenName, 'r', encoding='utf-8') as f:
            parser.readfp(f)
      
        for section_name in parser.sections():
            if (section_name == self.__GENERAL_SECTION):
                # Get GENERAL section
                self.__general =self.__get_section_as_hashmap(section_name,parser)

            elif (section_name == self.__XIVELY_SECTION):
                # Get XIVELY section
                self.__xively = self.__get_section_as_hashmap(section_name,parser)

            else:
                # Get all other variables  
                bricklet_name = parser.get(section_name, self.__NAME_KEY)
                bricklet_uid =  parser.get(section_name, self.__UID_KEY)
                
                tmp_bricklet = Bricklet_Info(bricklet_name,bricklet_uid)
                for section_key in parser.options(section_name):
                    if (section_key != self.__NAME_KEY and section_key != self.__UID_KEY):
                        tmp_bricklet.addKeyValuePair(section_key, parser.get(section_name, section_key))

                self.__bricklets.append(tmp_bricklet)
                
        # configuration file is parsed an ready to use     
        self.__isParsed = True
        
    def get_general_section(self):
        '''
        Returns the variables out of the "GENERAL" section in the configuration file if it
        was already parsed otherwise it call the <read_config_file()> function first
        '''
        if(not self.__isParsed):
            self.read_config_file()
            
        return self.__general

    def get_xively_section(self):
        '''
        Returns the variables out of the "XIVELY" section in the configuration file if it
        was already parsed otherwise it call the <read_config_file()> function first
        '''
        if(not self.__isParsed):
            self.read_config_file()
            
        return self.__xively
        
    def get_bricklets(self):
        '''
        Returns an array of bricklets out of the configuration file if it
        was already parsed. Otherwise it call the <read_config_file()> function first
        '''
        if(not self.__isParsed):
            self.read_config_file()
        
        return self.__bricklets
        
        
class Bricklet_Info(object):
    '''
    This class holds the information about an entry out of the configuration file
    '''
    
    def __init__(self, name, uid):
        self.__name = name
        self.__uid = uid
        self.__variables = { }
        
    def addKeyValuePair(self, key, value):
        '''
        adds a key-value pair to the bricklet dictionary
        '''
        self.__variables[key] = value
 
    def get_variables(self):
        '''
        Returns the dictionary which provides the variable name and its value 
        '''
        return self.__variables  
    
    def get_name(self):
        '''
        Returns the bricklet name
        '''
        return self.__name
    
    def get_uid(self):
        '''
        Returns the bricklet uid
        '''
        return self.__uid