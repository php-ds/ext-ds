#ifndef PHP_DS_VECTOR_H
#define PHP_DS_VECTOR_H

#include "../../ds/ds_vector.h"

#define Z_DS_VECTOR(z)   (((php_ds_vector_t*)(Z_OBJ(z)))->vector)
#define Z_DS_VECTOR_P(z) Z_DS_VECTOR(*z)
#define THIS_DS_VECTOR() Z_DS_VECTOR_P(getThis())

#define ZVAL_DS_VECTOR(z, v)  \
do { \
    ZVAL_OBJ(z, php_ds_vector_create_object_ex(v)); \
} while(0)

#define ZVAL_NEW_DS_VECTOR(z) \
do { \
    ZVAL_OBJ(z, php_ds_vector_create_object_ex(ds_vector())); \
} while(0)

#define RETURN_DS_VECTOR(v)                 \
do {                                        \
    if (v) {                                \
        ZVAL_DS_VECTOR(return_value, v);    \
    } else {                                \
        ZVAL_NULL(return_value);            \
    }                                       \
    return;                                 \
} while(0)

typedef struct php_ds_vector {
    zend_object      std;
    ds_vector_t     *vector;
} php_ds_vector_t;

zend_object *php_ds_vector_create_object_ex(ds_vector_t *vector);
zend_object *php_ds_vector_create_object(zend_class_entry *ce);
zend_object *php_ds_vector_create_clone(ds_vector_t *vector);

int php_ds_vector_serialize(
    zval                    *object,
    unsigned char          **buffer,
    size_t                  *length,
    zend_serialize_data     *data
);

int php_ds_vector_unserialize(
    zval                    *object,
    zend_class_entry        *ce,
    const unsigned char     *buffer,
    size_t                   length,
    zend_unserialize_data   *data
);

#endif
