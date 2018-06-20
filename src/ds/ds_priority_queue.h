#ifndef DS_PRIORITY_QUEUE_H
#define DS_PRIORITY_QUEUE_H

#include "../common.h"

typedef struct _ds_priority_queue_node_t {
    zval value;
    zval priority;
} ds_priority_queue_node_t;

typedef struct _ds_priority_queue_t {
    ds_priority_queue_node_t   *nodes;
    uint32_t                    capacity;
    uint32_t                    size;
    uint32_t                    next;
} ds_priority_queue_t;

#define DS_PRIORITY_QUEUE_MIN_CAPACITY 8

#define DS_PRIORITY_QUEUE_FOREACH_NODE(queue, node)                     \
do {                                                                    \
    ds_priority_queue_t      *_queue = queue;                           \
    ds_priority_queue_node_t *_node  = &_queue->nodes[0];               \
    ds_priority_queue_node_t *_last  = &_queue->nodes[queue->size - 1]; \
                                                                        \
    for (; _node <= _last; ++_node) {                                   \
        node = _node;


#define DS_PRIORITY_QUEUE_FOREACH_VALUE(queue, value)                   \
ds_priority_queue_node_t *__node = NULL;                                \
DS_PRIORITY_QUEUE_FOREACH_NODE(queue, __node)                           \
    value = &__node->value;


#define DS_PRIORITY_QUEUE_FOREACH(queue, value, priority)               \
ds_priority_queue_node_t *__node = NULL;                                \
DS_PRIORITY_QUEUE_FOREACH_NODE(queue, __node)                           \
    value    = &__node->value;                                          \
    priority = &__node->priority;


#define DS_PRIORITY_QUEUE_FOREACH_END() \
    }                                   \
} while (0)                             \

/**
 * Has to exist because of the uint32_t insertion order stamp.
 *
 * @todo this isn't necessary because we can re-index when the stamp == max int
 */
#define DS_PRIORITY_QUEUE_MAX_CAPACITY (1 << 31)

#define DS_PRIORITY_QUEUE_SIZE(queue) ((queue)->size)
#define DS_PRIORITY_QUEUE_IS_EMPTY(queue) (DS_PRIORITY_QUEUE_SIZE(queue) == 0)

ds_priority_queue_t *ds_priority_queue();

void ds_priority_queue_allocate(ds_priority_queue_t *queue, uint32_t capacity);

uint32_t ds_priority_queue_capacity(ds_priority_queue_t *queue);

zval *ds_priority_queue_peek(ds_priority_queue_t *queue);

void ds_priority_queue_pop(ds_priority_queue_t *queue, zval *return_value);

void ds_priority_queue_push(ds_priority_queue_t *queue, zval *value, zval *priority);

void ds_priority_queue_to_array(ds_priority_queue_t *queue, zval *array);

void ds_priority_queue_free(ds_priority_queue_t *queue);

void ds_priority_queue_clear(ds_priority_queue_t *queue);

ds_priority_queue_t *ds_priority_queue_clone(ds_priority_queue_t * queue);

ds_priority_queue_node_t* ds_priority_queue_create_sorted_buffer(ds_priority_queue_t *queue);

#endif
