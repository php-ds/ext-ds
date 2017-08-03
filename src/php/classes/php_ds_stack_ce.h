#ifndef PHP_DS_STACK_CE_H
#define PHP_DS_STACK_CE_H

#include "php.h"
#include "../../common.h"
#include "../arginfo.h"
#include "../objects/php_stack.h"

#define THIS_DS_STACK() Z_DS_STACK_P(getThis())

#define RETURN_DS_STACK(s)                  \
do {                                        \
    ds_stack_t *_s = s;                     \
    if (_s) {                               \
        ZVAL_DS_STACK(return_value, _s);    \
    } else {                                \
        ZVAL_NULL(return_value);            \
    }                                       \
    return;                                 \
} while(0)

extern zend_class_entry *php_ds_stack_ce;

ARGINFO_OPTIONAL_ZVAL(          Stack___construct, values);
ARGINFO_LONG(                   Stack_allocate, capacity);
ARGINFO_NONE_RETURN_LONG(       Stack_capacity);
ARGINFO_VARIADIC_ZVAL(          Stack_push, values);
ARGINFO_NONE(                   Stack_pop);
ARGINFO_NONE(                   Stack_peek);

void php_ds_register_stack();

#endif
