#ifndef DS_PRIORITY_QUEUE_ITERATOR_H
#define DS_PRIORITY_QUEUE_ITERATOR_H

#include "php.h"
#include "../../ds/ds_priority_queue.h"

typedef struct _php_ds_priority_queue_iterator {
    zend_object_iterator    intern;
    zend_object            *object;
    ds_priority_queue_t    *queue;
    zend_long               position;
} php_ds_priority_queue_iterator;

zend_object_iterator *php_ds_priority_queue_get_iterator(zend_class_entry *ce, zval *object, int by_ref);

#endif
