#ifndef PHP_DS_QUEUE_CE_H
#define PHP_DS_QUEUE_CE_H

#include "php.h"
#include "../../ds_common.h"
#include "../php_ds_arginfo.h"
#include "../objects/php_ds_queue.h"

#define THIS_DS_QUEUE() Z_DS_QUEUE_P(getThis())

#define RETURN_DS_QUEUE(q)                  \
do {                                        \
    ds_queue_t *_q = q;                     \
    if (_q) {                               \
        ZVAL_DS_QUEUE(return_value, _q);    \
    } else {                                \
        ZVAL_NULL(return_value);            \
    }                                       \
    return;                                 \
} while(0)

extern zend_class_entry *php_ds_queue_ce;

ARGINFO_OPTIONAL_ZVAL(          Queue___construct, values);
ARGINFO_LONG(                   Queue_allocate, capacity);
ARGINFO_NONE_RETURN_LONG(       Queue_capacity);
ARGINFO_VARIADIC_ZVAL(          Queue_push, values);
ARGINFO_NONE(                   Queue_pop);
ARGINFO_NONE(                   Queue_peek);

void php_ds_register_queue();

#endif
