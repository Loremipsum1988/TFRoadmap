/*
 * simple_queue.h
 *
 *  Created on: 18.08.2014
 *      Author: Marv
 */

#ifndef SIMPLE_QUEUE_H_
#define SIMPLE_QUEUE_H_

#include "csv_writer.h"
#include <stdlib.h>

typedef struct _s_queue_{
	csv_data *csv;
	struct _s_queue_ *next;
} s_queue;

void s_queue_add(s_queue *q, csv_data *csv);
void s_queue_print(s_queue *q);
void s_queue_write(s_queue *q);

#endif /* SIMPLE_QUEUE_H_ */
