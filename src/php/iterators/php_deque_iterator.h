#ifndef DS_DEQUE_ITERATOR_H
#define DS_DEQUE_ITERATOR_H

#include "php.h"
#include "../../ds/ds_deque.h"

typedef struct php_ds_deque_iterator {
    zend_object_iterator    intern;
    zend_long               position;
    ds_deque_t             *deque;
} php_ds_deque_iterator_t;

zend_object_iterator *ds_deque_get_iterator_ex(zend_class_entry *ce, zval *object, int by_ref, ds_deque_t *deque);
zend_object_iterator *ds_deque_get_iterator(zend_class_entry *ce, zval *object, int by_ref);

#endif
