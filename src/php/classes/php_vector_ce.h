#ifndef DS_VECTOR_CE_H
#define DS_VECTOR_CE_H

#include "php.h"
#include "../objects/php_vector.h"

#define THIS_DS_VECTOR() Z_DS_VECTOR_P(getThis())

#define RETURN_DS_VECTOR(v)                 \
do {                                        \
    ds_vector_t *_v = v;                    \
    if (_v) {                               \
        ZVAL_DS_VECTOR(return_value, _v);   \
    } else {                                \
        ZVAL_NULL(return_value);            \
    }                                       \
    return;                                 \
} while(0)

extern zend_class_entry *php_ds_vector_ce;

void php_ds_register_vector();

#endif
