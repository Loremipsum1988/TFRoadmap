/*
 * main.c
 *
 *  Created on: 18.08.2014
 *      Author: Marv
 */

#include <stdio.h>
#include <stdlib.h>
#include "csv_writer.h"

void TEMP_add_csv_data(csv_data *csv){
	csv->uid = "hTp";
	csv->name = "Barometer";
	csv->var_name = "air_pressure";
	csv->raw_data = 1200;
}

int example_csv_create(char* path_to_csv){
	//create csv struct
	csv_data *csv = malloc(sizeof(struct _csv_data_));	//alloc

	//ADD TIMESTAMP
	create_timestemp(csv);	//alloc

	//Open csv file
	FILE *fp = fopen(path_to_csv, "a+");
	if (fp == NULL) {
		printf("Could not open %s\n", path_to_csv);
		return -1;
	}

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
	free(csv->timestamp);	//free
	free(csv);				//free

	//TODO DEBUG-READ-CSV
	read_file(path_to_csv);

	return 0;
}

int main(void) {
	printf("------------------\n"
			"|START OF EXAMPLE|\n"
			"------------------\n");

	example_csv_create(CSV_DEFAULT_LOG_PATH);

	//END
	printf("\n------------------\n"
			"|END OF EXAMPLE|\n"
			"------------------\n");
	return EXIT_SUCCESS;
}
