#ifndef DS_PQ_ITERATOR_H
#define DS_PQ_ITERATOR_H

#include "php.h"
#include "../../ds/ds_pq.h"

typedef struct _php_ds_pq_iterator {
    zend_object_iterator    intern;
    zend_long               position;
    ds_pq_t    *queue;
} php_ds_pq_iterator;

zend_object_iterator *php_ds_pq_get_iterator(zend_class_entry *ce, zval *object, int by_ref);

#endif
