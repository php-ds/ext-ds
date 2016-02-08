#ifndef PHP_DS_PRIORITY_QUEUE_ITERATOR_H
#define PHP_DS_PRIORITY_QUEUE_ITERATOR_H

#include "php.h"
#include "../internal/php_priority_queue.h"

typedef struct _PriorityQueueIterator {
    zend_object_iterator    intern;
    zend_long               position;
    PriorityQueue          *queue;
} PriorityQueueIterator;

/**
 *
 */
zend_object_iterator *priority_queue_get_iterator(zend_class_entry *ce, zval *object, int by_ref);

#endif
