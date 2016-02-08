#ifndef PHP_DS_QUEUE_ITERATOR_H
#define PHP_DS_QUEUE_ITERATOR_H

#include "php.h"
#include "../internal/php_queue.h"

typedef struct _QueueIterator {
    zend_object_iterator     intern;
    zend_long                position;
    Queue                   *queue;
} QueueIterator;

zend_object_iterator *queue_get_iterator(zend_class_entry *ce, zval *object, int by_ref);

#endif
