#ifndef PHP_DS_STACK_ITERATOR_H
#define PHP_DS_STACK_ITERATOR_H

#include "php.h"
#include "../internal/php_stack.h"

typedef struct _StackIterator {
    zend_object_iterator     intern;
    zend_long                position;
    Stack                   *stack;
} StackIterator;

zend_object_iterator *stack_get_iterator(zend_class_entry *ce, zval *object, int by_ref);

#endif
