#ifndef DS_QUEUE_H
#define DS_QUEUE_H

#include "../ds_common.h"
#include "ds_deque.h"

#define QUEUE_SIZE(q)     ((q)->deque->size)
#define QUEUE_IS_EMPTY(q) ((q)->deque->size == 0)

#define QUEUE_FOREACH(queue, value)                 \
do {                                                \
    zval _tmp;                                      \
    while ( ! DS_DEQUE_IS_EMPTY(queue->deque)) {    \
        ds_deque_shift(queue->deque, &_tmp);        \
        value = &_tmp;

#define QUEUE_FOREACH_END()     \
    }                           \
    zval_ptr_dtor(&_tmp);       \
} while (0)                     \

typedef struct _ds_queue_t {
    ds_deque_t    *deque;
} ds_queue_t;

ds_queue_t *ds_queue_ex(ds_deque_t *deque);
ds_queue_t *ds_queue();
ds_queue_t *ds_queue_clone(ds_queue_t *queue);

void ds_queue_allocate(ds_queue_t *queue, zend_long capacity);
zend_long ds_queue_capacity(ds_queue_t *queue);

void  ds_queue_push(ds_queue_t *queue, VA_PARAMS);
void  ds_queue_push_one(ds_queue_t *queue, zval *value);
void  ds_queue_clear(ds_queue_t *queue);
void  ds_queue_pop(ds_queue_t *queue, zval *return_value);
void  ds_queue_pop_throw(ds_queue_t *queue, zval *return_value);
zval *ds_queue_peek(ds_queue_t *queue);
zval *ds_queue_peek_throw(ds_queue_t *queue);
void  ds_queue_push_all(ds_queue_t *queue, zval *value);
void  ds_queue_to_array(ds_queue_t *queue, zval *return_value);
void  ds_queue_free(ds_queue_t *queue);

int ds_queue_serialize(zval *object, unsigned char **buffer, size_t *length, zend_serialize_data *data);
int ds_queue_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t length, zend_unserialize_data *data);

#endif
