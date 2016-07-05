#ifndef PHP_DS_PAIR_H
#define PHP_DS_PAIR_H

#include "../../ds/ds_pair.h"

typedef struct _php_ds_pair_t {
    zend_object  std;
    ds_pair_t   *pair;
} php_ds_pair_t;

#define Z_DS_PAIR(z)   (((php_ds_pair_t*)(Z_OBJ(z)))->pair)
#define Z_DS_PAIR_P(z) Z_DS_PAIR(*z)
#define THIS_DS_PAIR() Z_DS_PAIR_P(getThis())

#define ZVAL_DS_PAIR(z, pair) ZVAL_OBJ(z, php_ds_pair_create_object_ex(pair))

#define RETURN_DS_PAIR(pair)                \
do {                                        \
    if (pair) {                             \
        ZVAL_DS_PAIR(return_value, pair);   \
    } else {                                \
        ZVAL_NULL(return_value);            \
    }                                       \
    return;                                 \
} while(0)

zend_object *php_ds_pair_create_object_ex(ds_pair_t *pair);
zend_object *php_ds_pair_create_object(zend_class_entry *ce);
zend_object *php_ds_pair_create_clone(ds_pair_t *pair);

int php_ds_pair_serialize(zval *object, unsigned char **buffer, size_t *length, zend_serialize_data *data);
int php_ds_pair_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t length, zend_unserialize_data *data);

#endif
