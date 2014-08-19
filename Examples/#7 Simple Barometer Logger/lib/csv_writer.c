/*
 * csv_writer.c
 *
 *  Created on: 18.08.2014
 *      Author: Marv
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "csv_writer.h"

//TODO DEBUG ONLY
int get_working_directory(char** ref_path) {
	if ((*ref_path = _getcwd( NULL, 0)) == NULL) {
		perror("_getcwd error");
		return -1;
	}
	printf("DEBUG: %s\n", *ref_path);
	return 0;
}

//TODO DEBUG ONLY
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

//TODO DEBUG ONLY
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


int file_is_empty(FILE *fp) {
	long savedOffset = ftell(fp);
	fseek(fp, 0, SEEK_END);

	if (ftell(fp) == 0) {
		return 1; //true
	}

	fseek(fp, savedOffset, SEEK_SET);
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

void write_csv_data(FILE *fp, csv_data *csv) {
	//Check for csv header
	write_csv_header(fp);
	//Write csv data into the file
	fprintf(fp,"%s;%s;%s;%i;%s\n", csv->uid, csv->name, csv->var_name, csv->raw_data, csv->timestamp);
}
