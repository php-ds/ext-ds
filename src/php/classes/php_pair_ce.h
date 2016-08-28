#ifndef DS_PAIR_CE_H
#define DS_PAIR_CE_H

#include "php.h"
#include "../../common.h"
#include "../arginfo.h"
#include "../objects/php_pair.h"

#define THIS_DS_PAIR() Z_DS_PAIR_P(getThis())

#define RETURN_DS_PAIR(p)                   \
do {                                        \
    ds_pair_t *_p = p;                      \
    if (_p) {                               \
        ZVAL_DS_PAIR(return_value, _p);     \
    } else {                                \
        ZVAL_NULL(return_value);            \
    }                                       \
    return;                                 \
} while(0)


extern zend_class_entry *php_ds_pair_ce;

ARGINFO_OPTIONAL_ZVAL_OPTIONAL_ZVAL(    Pair___construct, key, value);
ARGINFO_NONE_RETURN_DS(                 Pair_copy, Pair);
ARGINFO_NONE_RETURN_ARRAY(              Pair_toArray);
ARGINFO_NONE(                           Pair_jsonSerialize);

void php_ds_register_pair();

#endif
