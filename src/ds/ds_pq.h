#ifndef DS_PQ_H
#define DS_PQ_H

#include "../common.h"

typedef struct _ds_pq_node_t {
    zval        value;
    zend_long   priority;
} ds_pq_node_t;

typedef struct _ds_pq_t {
    ds_pq_node_t   *nodes;
    uint32_t                    capacity;
    uint32_t                    size;
    uint32_t                    next;
} ds_pq_t;

#define DS_PQ_MIN_CAPACITY 8

#define DS_PQ_FOREACH_NODE(queue, node)                     \
do {                                                                    \
    ds_pq_t      *_queue = queue;                           \
    ds_pq_node_t *_node  = &_queue->nodes[0];               \
    ds_pq_node_t *_last  = &_queue->nodes[queue->size - 1]; \
                                                                        \
    for (; _node <= _last; ++_node) {                                   \
        node = _node;


#define DS_PQ_FOREACH_VALUE(queue, value)                   \
ds_pq_node_t *__node = NULL;                                \
DS_PQ_FOREACH_NODE(queue, __node)                           \
    value = &__node->value;


#define DS_PQ_FOREACH(queue, value, priority)               \
ds_pq_node_t *__node = NULL;                                \
DS_PQ_FOREACH_NODE(queue, __node)                           \
    value    = &__node->value;                                          \
    priority = __node->priority;


#define DS_PQ_FOREACH_END() \
    }                                   \
} while (0)                             \

/**
 * Has to exist because of the uint32_t insertion order stamp.
 */
#define DS_PQ_MAX_CAPACITY (1 << 31)

#define DS_PQ_SIZE(queue) ((queue)->size)
#define DS_PQ_IS_EMPTY(queue) (DS_PQ_SIZE(queue) == 0)

ds_pq_t *ds_pq();

void ds_pq_allocate(ds_pq_t *queue, uint32_t capacity);
uint32_t ds_pq_capacity(ds_pq_t *queue);
zval *ds_pq_peek(ds_pq_t *queue);
void ds_pq_pop(ds_pq_t *queue, zval *return_value);
void ds_pq_push(ds_pq_t *queue, zval *value, zend_long priority);
void ds_pq_to_array(ds_pq_t *queue, zval *array);
void ds_pq_free(ds_pq_t *queue);
void ds_pq_clear(ds_pq_t *queue);
ds_pq_t *ds_pq_clone(ds_pq_t * queue);
ds_pq_node_t* ds_pq_create_sorted_buffer(ds_pq_t *queue);

#endif
