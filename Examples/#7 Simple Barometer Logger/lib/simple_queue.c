/*
 * simple_queue.c
 *
 *  Created on: 18.08.2014
 *      Author: Marv
 */

#include "simple_queue.h"

void s_queue_add(s_queue *q, csv_data *csv) {
	if (q->csv == NULL) { //check for first s_queue
		q->csv = csv;
		return;
	}

	while (q->next != NULL) {
		q = q->next;
	}
	//ALLOC s_queue
	q->next = malloc(sizeof(struct _s_queue_));
	q = q->next;

	q->csv = csv;
	q->next = NULL;
}

void s_queue_print(s_queue *q) {
	printf("s_queue print---------------------------------------\n");
	while (q != NULL) {
		printf("%s;%s;%s;%i;%s\n", q->csv->uid, q->csv->name, q->csv->var_name,
				q->csv->raw_data, q->csv->timestamp);
		q = q->next;
	}
	printf("s_queue print end-----------------------------------\n");
	fflush(stdout);
}

void s_queue_write(s_queue *q) {
	FILE *fp;
	fp = fopen(CSV_DEFAULT_LOG_PATH, "a+");
	while (q != NULL) {
		//write to file
		write_csv_data(fp, q->csv);

		q = q->next;
	}
	fclose(fp);
}
