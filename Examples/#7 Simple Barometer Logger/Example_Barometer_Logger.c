/*
 ============================================================================
 Name        : Example_Barometer_Logger.c
 Author      : MArvin Lutz
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "lib/ip_connection.h"
#include "lib/bricklet_barometer.h"
#include "lib/csv_writer.h"

#include "lib/simple_queue.h"

#define HOST "localhost"
#define PORT 4223
#define UID "fVP" // Change to your UID

s_queue *q;
const s_queue *start;

void cb_altitude(int p, void *user_data) {

	csv_data *csv = malloc(sizeof(struct _csv_data_));	//alloc csv
	//ADD TIMESTAMP
	create_timestemp(csv);	//alloc timestamp
	csv->uid = UID;
	csv->name = "Barometer"; //TODO used #define in own barometer.c file
	csv->var_name = "altitude"; //TODO use #define from own barometer.c file
	csv->raw_data = p;


	s_queue_add(q, csv);
	fprintf(stdout, "Altitude: %i\n", csv->raw_data);
	fflush(stdout);
}

void cb_air_pressure(int p, void *user_data) {
	csv_data *csv = malloc(sizeof(struct _csv_data_));	//alloc csv
	//ADD TIMESTAMP
	create_timestemp(csv);	//alloc timestamp
	csv->uid = UID;
	csv->name = "Barometer"; //TODO used #define in own barometer.c file
	csv->var_name = "air_pressure"; //TODO use #define from own barometer.c file
	csv->raw_data = p;


	s_queue_add(q, csv);
	fprintf(stdout, "Air_Pressure: %i\n", csv->raw_data);
	fflush(stdout);
}

int main(void) {
	// Create IP connection
	IPConnection ipcon;
	ipcon_create(&ipcon);

	// Create device object
	Barometer b;
	barometer_create(&b, UID, &ipcon);

	// Connect to brickd
	if (ipcon_connect(&ipcon, HOST, PORT) < 0) {
		fprintf(stderr, "Could not connect to brickd\n");
		exit(1);
	}

	q = malloc(sizeof(struct _s_queue_));
	q->csv = NULL;
	q->next = NULL;

	start = q;

	barometer_set_altitude_callback_period(&b, 1000);
	barometer_register_callback(&b, BAROMETER_CALLBACK_ALTITUDE,
			(void*) cb_altitude, NULL);

	barometer_set_air_pressure_callback_period(&b, 2000);
	barometer_register_callback(&b, BAROMETER_CALLBACK_AIR_PRESSURE,
			(void*) cb_air_pressure, NULL);

	fprintf(stdout, "Press q to exit\n\n");
	fflush(stdout);
	getchar() == 'q';

	// Calls ipcon_disconnect internally
	ipcon_destroy(&ipcon);

	q = start;
	s_queue_print(q);
	q = start;
	s_queue_write(q);

	return EXIT_SUCCESS;
}
