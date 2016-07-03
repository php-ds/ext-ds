#ifndef DS_PRIORITY_QUEUE_H
#define DS_PRIORITY_QUEUE_H

#include "php.h"

typedef struct _PriorityHeapNode {
    zval        value;
    zend_long   priority;
} PriorityNode;

typedef struct _PriorityQueue {
    zend_object     std;
    PriorityNode   *nodes;
    uint32_t        capacity;
    uint32_t        size;
    uint32_t        next;
} PriorityQueue;

#define PRIORITY_QUEUE_MIN_CAPACITY 8
#define PRIORITY_QUEUE_MAX_CAPACITY (1 << 31)

#define PRIORITY_QUEUE_SIZE(pq) ((pq)->size)
#define PRIORITY_QUEUE_IS_EMPTY(pq) (PRIORITY_QUEUE_SIZE(pq) == 0)

#define Z_PRIORITY_QUEUE(z)   ((PriorityQueue*)(Z_OBJ(z)))
#define Z_PRIORITY_QUEUE_P(z) Z_PRIORITY_QUEUE(*z)
#define THIS_PRIORITY_QUEUE() Z_PRIORITY_QUEUE_P(getThis())

#define ZVAL_PRIORITY_QUEUE(z, pq)  ZVAL_OBJ(z, &pq->std)
#define ZVAL_NEW_PRIORITY_QUEUE(z)  ZVAL_PRIORITY_QUEUE(z, priority_queue_init())

#define RETURN_PRIORITY_QUEUE(pq) \
do { \
    ZVAL_PRIORITY_QUEUE(return_value, pq); \
    return; \
} while(0)

PriorityQueue *priority_queue_init();

void priority_queue_user_allocate(PriorityQueue *queue, uint32_t capacity);
uint32_t priority_queue_capacity(PriorityQueue *queue);
zend_object *priority_queue_init_object(zend_class_entry *ce);
zend_object *priority_queue_init_clone(PriorityQueue *queue);
zval *priority_queue_peek(PriorityQueue *pq);
void priority_queue_pop(PriorityQueue *pq, zval *return_value);
void priority_queue_push(PriorityQueue *pq, zval *value, zend_long priority);
void priority_queue_to_array(PriorityQueue *pq, zval *array);
void priority_queue_destroy(PriorityQueue *pq);
void priority_queue_clear(PriorityQueue *queue);

int priority_queue_serialize(zval *object, unsigned char **buffer, size_t *buf_len, zend_serialize_data *data);
int priority_queue_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t length, zend_unserialize_data *data);

#endif
