
#include <stdio.h>

#include "ip_connection.h"
#include "bricklet_barometer.h"

#define HOST "localhost"
#define PORT 4223
#define UID "fVP" // Change to your UID

Barometer b;

void my_callback(int p, void *user_data) { //FIXME Fehler checken (p < 0)
    printf("Altitude(r): %d\n", p);
    int32_t altitude;
	if(barometer_get_altitude(&b, &altitude) < 0) {
		fprintf(stderr, "Could not get altitude, probably timeout\n");
   		exit(1);
	}
	printf("Altitude(f): %.2f m\n", altitude/100.0);
}

void temp_callback(){
	int16_t temperature;
	barometer_get_chip_temperature(&b, &temperature);
	printf("temperature: %i", temperature);

}

int main() {
	// Create IP connection
	IPConnection ipcon;
	ipcon_create(&ipcon);

	// Create device object

	barometer_create(&b, UID, &ipcon); 

	// Connect to brickd
	if(ipcon_connect(&ipcon, HOST, PORT) < 0) {
		fprintf(stderr, "Could not connect\n");
		exit(1);
	}
	// Don't use device before ipcon is connected

	barometer_set_altitude_callback_period(&b, 2000);
	barometer_register_callback(&b, BAROMETER_CALLBACK_ALTITUDE, (void*)my_callback, NULL);
	SetTimer(NULL, 1, 5000, (void*)temp_callback);

/*	while(1){
		// Get current air pressure (unit is mbar/1000)
			int32_t air_pressure;
			if(barometer_get_air_pressure(&b, &air_pressure) < 0) {
				fprintf(stderr, "Could not get air pressure, probably timeout\n");
				exit(1);
			}
			printf("Air Pressure: %f mbar\n", air_pressure/1000.0);

			// Get current altitude (unit is cm)
			int32_t altitude;
			if(barometer_get_altitude(&b, &altitude) < 0) {
				fprintf(stderr, "Could not get altitude, probably timeout\n");
				exit(1);			}

			printf("Altitude: %f m\n", altitude/100.0);


			//Chip Temp
			int16_t temperature;
			if(barometer_get_chip_temperature(&b, &temperature) < 0 ){
				fprintf(stderr, "Could not get air pressure, probably timeout\n");
						exit(1);
			}
			printf("Current chip temperature: %.2f GradC\n", (temperature/100.0));



			printf("Press q to exit\n\n");
			if(getchar() == 'q') break;
	}*/


	printf("Press q to exit\n\n");
	getchar();

	ipcon_destroy(&ipcon); // Calls ipcon_disconnect internally

	return 0;
}
