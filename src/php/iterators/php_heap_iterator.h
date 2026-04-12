#ifndef DS_HEAP_ITERATOR_H
#define DS_HEAP_ITERATOR_H

#include "php.h"
#include "../../ds/ds_heap.h"

typedef struct _php_ds_heap_iterator_t {
    zend_object_iterator       intern;
    zend_object               *object;
    ds_heap_t                 *heap;    // Cloned heap — popped during iteration.
    ds_heap_compare_func_t     compare;
    zend_long                  position;
} php_ds_heap_iterator_t;

zend_object_iterator *php_ds_heap_get_iterator(zend_class_entry *ce, zval *obj, int by_ref);

#endif
