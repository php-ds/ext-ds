#ifndef PHP_DS_STACK_ITERATOR_H
#define PHP_DS_STACK_ITERATOR_H

#include "php.h"
#include "../internal/php_stack.h"

typedef struct _php_ds_stack_iterator_t {
    zend_object_iterator     intern;
    zend_long                position;
    php_ds_stack_t                   *stack;
} php_ds_stack_iterator_t;

zend_object_iterator *php_ds_stack_get_iterator(zend_class_entry *ce, zval *object, int by_ref);

#endif
