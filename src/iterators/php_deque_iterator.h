#ifndef DS_DEQUE_ITERATOR_H
#define DS_DEQUE_ITERATOR_H

#include "php.h"
#include "../internal/php_deque.h"

typedef struct _DequeIterator {
    zend_object_iterator    intern;
    zend_long               position;
    Deque                  *deque;
} DequeIterator;

zend_object_iterator *deque_get_iterator_ex(zend_class_entry *ce, zval *object, int by_ref, Deque *deque);
zend_object_iterator *deque_get_iterator(zend_class_entry *ce, zval *object, int by_ref);

#endif
