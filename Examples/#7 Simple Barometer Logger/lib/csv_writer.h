/*
 * csv_writer.h
 *
 * How to use the csv_writer:
 * 1.	Create a FILE *fp and open the file you want as destination for your csv data
 * 2. 	Create a csv_data* variable with the data you want to log
 * 			- allocate memory for the struct!
 * 3. 	Create a timestamp with [create_timestemp(csv_data* csv)]
 * 			- memory is allocated, so dont forget to FREE it at the end!
 * 4.  	Write the csv_data to a logger file with [write_csv_data(FILE *fp, csv_data *csv)]
 * 			- If the file is empty, a csv header will be created
 * 5. 	FREE the allocated data (csv.timestamp & csv-struct)
 * (5a)	Repeat step 2 to 5 for multiple data entries
 * 6	Close the File *fp
 *
 *  Created on: 18.08.2014
 *      Author: Marv
 */

#ifndef CSV_WRITER_H_
#define CSV_WRITER_H_

#include <stdint.h>
#include <stdio.h>

#define CSV_UID "UID"
#define CSV_BRICKLET_NAME "NAME"
#define CSV_VAR_NAME "VARIABLE"
#define CSV_RAW_DATA "RAW"
#define CSV_TIMESTAMP "TIMESTAMP"

#define CSV_DEFAULT_LOG_PATH "logged_data.csv"

typedef struct _csv_data_ {
	char* uid;
	char* name;
	char* var_name;
	int* raw_data;
	char* timestamp;
} csv_data;

//DEBUG ONLY
int get_working_directory(char** ref_path);
//DEBUG ONLY
int read_file(char* path);
//DEBUG ONLY
int write_file(char* path, char* content, int append);


/**
 * Checks if the file is empty.
 * The FILE needs to be open!
 *
 * VAR:
 * 	FILE *fp	-	The file to check
 *
 * RETURN:
 * 	0 (false)	- 	If FILE is not empty
 * 	1 (true)	- 	If FILE is empty
 */
int file_is_empty(FILE *file);

/**
 * Writes the header of our csv log file.
 * Only writes the header if the FILE is empty.
 *
 * This function is called automatically in write_csv_data.
 * The FILE needs to be open!
 *
 * VAR:
 * 	FILE *fp	-	The csv logger file
 */
void write_csv_header(FILE *fp);
/**
 * Create a timestamp for the logged data.
 * The timestamp is added to the csv_data struct.
 * !!! Memory is allocated for the csv.timestamp variable!!!
 * This function should be used asap when relevant logging data was received.!
 *
 * VAR:
 * 	csv_data *csv	-	The csv_data struct for the current logged data
 */
void write_csv_data(FILE *fp, csv_data *csv);
/**
 * Write the logged data into the csv logger file.
 * Creates a csv header, if the file is empty.
 * This function should be used to write into the
 * logger file.
 *
 * The FILE needs to be open!
 *
 * VAR:
 * 	FILE *fp		-	The csv logger file
 * 	csv_data *csv	-	The csv data to write
 */
void create_timestemp(csv_data* csv);

#endif /* CSV_WRITER_H_ */
