#ifndef DS_DEQUE_CE_H
#define DS_DEQUE_CE_H

#include "php.h"
#include "../objects/php_deque.h"

#define THIS_DS_DEQUE() Z_DS_DEQUE_P(getThis())

#define RETURN_DS_DEQUE(d)                  \
do {                                        \
    ds_deque_t *_d = d;                     \
    if (_d) {                               \
        ZVAL_DS_DEQUE(return_value, _d);    \
    } else {                                \
        ZVAL_NULL(return_value);            \
    }                                       \
    return;                                 \
} while(0)

extern zend_class_entry *php_ds_deque_ce;

void php_ds_register_deque();

#endif
