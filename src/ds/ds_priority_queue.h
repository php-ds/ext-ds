#ifndef DS_PRIORITY_QUEUE_H
#define DS_PRIORITY_QUEUE_H

#include "../common.h"

typedef struct _ds_priority_queue_node_t {
    zval        value;
    zend_long   priority;
} ds_priority_queue_node_t;

typedef struct _ds_priority_queue_t {
    ds_priority_queue_node_t   *nodes;
    uint32_t                    capacity;
    uint32_t                    size;
    uint32_t                    next;
} ds_priority_queue_t;

#define DS_PRIORITY_QUEUE_MIN_CAPACITY 8

/**
 * Has to exist because of the uint32_t insertion order stamp.
 */
#define DS_PRIORITY_QUEUE_MAX_CAPACITY (1 << 31)

#define DS_PRIORITY_QUEUE_SIZE(pq) ((pq)->size)
#define DS_PRIORITY_QUEUE_IS_EMPTY(pq) (DS_PRIORITY_QUEUE_SIZE(pq) == 0)

ds_priority_queue_t *ds_priority_queue();

void ds_priority_queue_allocate(ds_priority_queue_t *pq, uint32_t capacity);
uint32_t ds_priority_queue_capacity(ds_priority_queue_t *pq);
zval *ds_priority_queue_peek(ds_priority_queue_t *pq);
void ds_priority_queue_pop(ds_priority_queue_t *pq, zval *return_value);
void ds_priority_queue_push(ds_priority_queue_t *pq, zval *value, zend_long priority);
void ds_priority_queue_to_array(ds_priority_queue_t *pq, zval *array);
void ds_priority_queue_free(ds_priority_queue_t *pq);
void ds_priority_queue_clear(ds_priority_queue_t *pq);
ds_priority_queue_t *ds_priority_queue_clone(ds_priority_queue_t * pq);
ds_priority_queue_node_t* ds_priority_queue_create_sorted_buffer(ds_priority_queue_t *pq);

#endif
