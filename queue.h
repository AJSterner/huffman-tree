#include <stdlib.h>

#ifndef QUEUE_INCLUDED
#define QUEUE_INCLUDED

typedef struct Queue_T *Queue_T;

Queue_T Queue_new(size_t hint);
void Queue_free(Queue_T *queue);

Queue_T Queue_addhi(Queue_T queue, void *data);
Queue_T Queue_addlo(Queue_T queue, void *data);

void *Queue_pophi(Queue_T queue);
void *Queue_poplo(Queue_T queue);

void *Queue_peakhi(Queue_T queue);
void *Queue_peaklo(Queue_T queue);

size_t Queue_size(Queue_T queue);

void Queue_status(Queue_T queue);

#endif