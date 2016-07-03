#ifndef DS_PRIORITY_QUEUE_ITERATOR_H
#define DS_PRIORITY_QUEUE_ITERATOR_H

#include "php.h"
#include "../../ds/ds_priority_queue.h"

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
