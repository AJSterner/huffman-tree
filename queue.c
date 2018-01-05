#include "queue.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

static size_t INITIAL_CAPACITY = 256;
void ensure_capacity(Queue_T queue);

struct Queue_T {
        unsigned head;
        unsigned capacity;
        unsigned size;
        void **arr;
};

// inline unsigned tail_index(Queue_T queue) {
//         return (unsigned)((size_t)queue->head + (size_t)queue->capacity - 1) % queue->capacity;
// }
size_t Queue_size(Queue_T queue)
{
        assert(queue != NULL);
        return queue->size;
}

void Queue_status(Queue_T queue)
{
        printf("head: %u\ncapacity: %u\nsize: %u\n", 
                queue->head, queue->capacity, queue->size);
}

Queue_T Queue_new(size_t hint)
{
        Queue_T queue = malloc(sizeof(*queue));
        queue->head = 0;
        if (hint == 0) {
                hint = INITIAL_CAPACITY;
        }
        queue->capacity = hint;
        queue->size = 0;
        queue->arr = calloc(hint, sizeof(*(queue->arr)));
        return queue;

}

void Queue_free(Queue_T *queue)
{
        assert(queue != NULL && *queue != NULL);
        free((*queue)->arr);
        free(*queue);
        *queue = NULL;
}

Queue_T Queue_addhi(Queue_T queue, void *elem)
{
        assert(queue != NULL);
        ensure_capacity(queue);
        
        if (queue->head == 0) {
                queue->head = queue->capacity;
        }

        queue->arr[--queue->head] = elem;
        queue->size++;
        return queue;
}

Queue_T Queue_addlo(Queue_T queue, void *elem)
{
        assert(queue != NULL);
        ensure_capacity(queue);
        unsigned tail = ((size_t)queue->head + queue->size) % queue->capacity; //TODO: overflow?

        queue->arr[tail] = elem;
        queue->size++;
        return queue;
}

void *Queue_pophi(Queue_T queue)
{
        assert(queue != NULL && queue->size > 0);

        void *elem = queue->arr[queue->head];
        queue->size--;
        if (++queue->head == queue->capacity) { // TODO: confirm deals with overflow
                queue->head = 0;
        }
        return elem;
}

void *Queue_poplo(Queue_T queue)
{
        assert(queue != NULL && queue->size > 0);

        unsigned tail = ((size_t)queue->head + --queue->size) % queue->capacity;
        return queue->arr[tail];
}

void *Queue_peakhi(Queue_T queue)
{
        assert(queue != NULL && queue->size > 0);
        return queue->arr[queue->head];
}

void *Queue_peaklo(Queue_T queue)
{
        assert(queue != NULL && queue->size > 0);
        return queue->arr[((size_t)queue->head + queue->size - 1) % queue->capacity];
}

void ensure_capacity(Queue_T queue)
{
        assert(queue != NULL);
        if (queue->size == queue->capacity) {
                unsigned new_capacity = queue->capacity * 2;
                assert(new_capacity > queue->capacity);

                void **new_arr = calloc(new_capacity, sizeof(*new_arr)); //TODO: deal with unsigned max
                assert(new_arr != NULL);

                //TODO: test this vs mod
                //copy everything from head to end of old array to start of new_arr
                for (unsigned i = 0; i < queue->capacity - queue->head; i++){
                        new_arr[i] = queue->arr[i + queue->head]; 
                }

                //copy everything from start of old array to continuation of new_arr
                for (unsigned i = 0; i < queue->head; i++) {
                        new_arr[i + (queue->capacity - queue->head)] = queue->arr[i];
                }

                free(queue->arr);
                queue->arr = new_arr;
                queue->head = 0;
                queue->capacity *= 2;
        }
}
