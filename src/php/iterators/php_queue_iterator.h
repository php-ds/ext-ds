#ifndef DS_QUEUE_ITERATOR_H
#define DS_QUEUE_ITERATOR_H

#include "php.h"
#include "../../ds/ds_queue.h"

typedef struct _ds_queue_iterator_t {
    zend_object_iterator     intern;
    zend_long                position;
    ds_queue_t              *queue;
} ds_queue_iterator_t;

zend_object_iterator *php_ds_queue_get_iterator(zend_class_entry *ce, zval *object, int by_ref);

#endif
