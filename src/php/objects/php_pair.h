#ifndef PHP_DS_PAIR_H
#define PHP_DS_PAIR_H

#include "../../ds/ds_pair.h"

#define Z_DS_PAIR(z)   (((php_ds_pair_t*)(Z_OBJ(z)))->pair)
#define Z_DS_PAIR_P(z) Z_DS_PAIR(*z)
#define THIS_DS_PAIR() Z_DS_PAIR_P(getThis())

#define ZVAL_DS_PAIR(z, p) ZVAL_OBJ(z, php_ds_pair_create_object_ex(p))

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

typedef struct _php_ds_pair_t {
    zend_object  std;
    ds_pair_t   *pair;
} php_ds_pair_t;

zend_object *php_ds_pair_create_object_ex(ds_pair_t *pair);
zend_object *php_ds_pair_create_object(zend_class_entry *ce);
zend_object *php_ds_pair_create_clone(ds_pair_t *pair);

PHP_DS_SERIALIZE_FUNCIONS(php_ds_pair);

#endif
