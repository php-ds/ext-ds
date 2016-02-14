#ifndef DS_QUEUE_H
#define DS_QUEUE_H

#include "php.h"
#include "zend_exceptions.h"
#include "zend_interfaces.h"
#include "ext/spl/spl_iterators.h"

#include "php_deque.h"

typedef struct _Queue {
    zend_object    std;
    Deque         *deque;
} Queue;


#define QUEUE_SIZE(queue) (DEQUE_SIZE((queue)->deque))
#define QUEUE_IS_EMPTY(queue) (QUEUE_SIZE(queue) == 0)

#define Z_QUEUE(z)   ((Queue*)(Z_OBJ(z)))
#define Z_QUEUE_P(z) Z_QUEUE(*z)
#define THIS_QUEUE() Z_QUEUE_P(getThis())

#define ZVAL_QUEUE(z, queue)  ZVAL_OBJ(z, &queue->std)
#define ZVAL_NEW_QUEUE(z)     ZVAL_QUEUE(z, queue_init())

#define RETURN_QUEUE(queue) \
do { \
    ZVAL_QUEUE(return_value, queue); \
    return; \
} while(0)

#define QUEUE_FOREACH(queue, value)                 \
do {                                                \
    zval _tmp;                                      \
    while ( ! DEQUE_IS_EMPTY(queue->deque)) {       \
        deque_shift(queue->deque, &_tmp);           \
        value = &_tmp;

#define QUEUE_FOREACH_END()     \
    }                           \
    zval_ptr_dtor(&_tmp);       \
} while (0)                     \


Queue *queue_init();

zend_object *queue_create_object(zend_class_entry *ce);
zend_object *queue_create_clone(Queue *queue);

void queue_user_allocate(Queue *queue, zend_long capacity);
zend_long queue_capacity(Queue *queue);

void queue_push(Queue *queue, VA_PARAMS);
void queue_push_one(Queue *queue, zval *value);
void queue_clear(Queue *queue);
void queue_pop(Queue *queue, zval *return_value);
zval *queue_peek(Queue *queue);
void queue_push_all(Queue *queue, zval *value);
void queue_to_array(Queue *queue, zval *return_value);
void queue_destroy(Queue *queue);

int queue_serialize(zval *object, unsigned char **buffer, size_t *length, zend_serialize_data *data);
int queue_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t length, zend_unserialize_data *data);

#endif
