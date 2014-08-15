/*
 ============================================================================
 Name        : example_file.c
 Author      : MArvin Lutz
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CSV_UID "UID"
#define CSV_BRICKLET_NAME "NAME"
#define CSV_VAR_NAME "VARIABLE"
#define CSV_RAW_DATA "RAW"
#define CSV_TIMESTAMP "TIMESTAMP"

typedef struct _csv_data_ {
	char* uid;
	char* name;
	char* var_name;
	char* raw_data;
	char* timestamp;
} csv_data;

int get_working_directory(char** ref_path) {
	if ((*ref_path = _getcwd( NULL, 0)) == NULL) {
		perror("_getcwd error");
		return -1;
	}
	printf("DEBUG: %s\n", *ref_path);
	return 0;
}

int read_file(char* path) {
	FILE *fp;
	/**
	 r  - open for reading
	 w  - open for writing (file need not exist)
	 a  - open for appending (file need not exist)
	 r+ - open for reading and writing, start at beginning
	 w+ - open for reading and writing (overwrite file)
	 a+ - open for reading and writing (append if file exists)
	 */
	//TODO strings verknüpfen?
	printf("Content of File: %s\n", path);
	fp = fopen(path, "r");
	if (fp == NULL) { /* error opening file returns NULL */
		printf("Could not open read.txt!\n");
		return -1;
	}
	/* while character is not end of file */
	int character;
	while ((character = fgetc(fp)) != EOF) {
		putchar(character); /* print the character */
	}

	//FILE-CLOSE
	return fclose(fp);
}

int write_file(char* path, char* content, int append) {
	//append = 0 = false -> any other number = true
	FILE *fp;
	if (append != 0)
		fp = fopen(path, "a+");
	else
		fp = fopen(path, "w+");

	if (fp == NULL) { /* error opening file returns NULL */
		printf("Could not open %s\n", path);
		return -1;
	}

	//new_line when appended?
	fprintf(fp, content);

	//TODO DEBUG AUSGABE
	printf("Contetn written to File: %s\n%s\n", path, content);

	//FILE-CLOSE
	return fclose(fp);
}

int file_is_empty(FILE *file) {
	long savedOffset = ftell(file);
	fseek(file, 0, SEEK_END);

	if (ftell(file) == 0) {
		return 1; //true
	}

	fseek(file, savedOffset, SEEK_SET);
	return 0;  //false
}

void write_csv_header(FILE *fp) {
	if (file_is_empty(fp) == 0)
		return;

	fprintf(fp, "%s;%s;%s;%s;%s\n", CSV_UID, CSV_BRICKLET_NAME, CSV_VAR_NAME,
	CSV_RAW_DATA, CSV_TIMESTAMP);
}

void create_timestemp(csv_data* csv) {
	//Allocate memory
	int size_ts = sizeof("YYYY-MM-DD hh:mm:ss");
	csv->timestamp = malloc(size_ts);

	//Create the time struct
	time_t ltime; /* calendar time */
	ltime = time(NULL); /* get current cal time */
	struct tm* ts = localtime(&ltime);

	//Write the timestamp into csv.timestamp
	snprintf(csv->timestamp, size_ts, "%4d-%02d-%02d %02d:%02d:%02d",
			(ts->tm_year + 1900), (ts->tm_mon + 1), ts->tm_mday, ts->tm_hour,
			ts->tm_min, ts->tm_sec);
}

//5 ->  UID;NAME;VAR_NAME;RAW;TIMESTAMP
void write_csv_data(FILE *fp, csv_data *csv) {
	//Check for csv header
	write_csv_header(fp);
	//Write csv data into the file
	fprintf(fp,"%s;%s;%s;%s;%s\n", csv->uid, csv->name, csv->var_name, csv->raw_data, csv->timestamp);
}

void TEMP_add_csv_data(csv_data *csv){
	csv->uid = "hTp";
	csv->name = "Barometer";
	csv->var_name = "air_pressure";
	csv->raw_data = "1200";
}

int example_csv_create(char* path_to_csv){
	//create csv struct
	csv_data *csv = malloc(sizeof(struct _csv_data_));

	//ADD TIMESTAMP
	create_timestemp(csv);

	//Open csv file
	FILE *fp = fopen(path_to_csv, "a+");
	if (fp == NULL) {
		printf("Could not open %s\n", path_to_csv); //TODO
		return -1;
	}

	//TODO DEBUG
	TEMP_add_csv_data(csv);

	//write to csv
	write_csv_data(fp, csv);

	//close csv file
	if (fclose(fp) < 0)
		printf("Could not close fp!");

	/*FREE csv allocations in this order:
	 *- csv.timestamp
	 *
	 *- csv
	 */
	free(csv->timestamp);
	free(csv);

	//TODO DEBUG-READ-CSV
	read_file(path_to_csv);

	return 0;
}

int main(void) {
	printf("------------------\n"
			"|START OF EXAMPLE|\n"
			"------------------\n");

	example_csv_create("files/data.csv");

	//END
	printf("\n------------------\n"
			"|END OF EXAMPLE|\n"
			"------------------\n");
	return EXIT_SUCCESS;
}
