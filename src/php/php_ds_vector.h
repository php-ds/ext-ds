#ifndef PHP_DS_VECTOR_H
#define PHP_DS_VECTOR_H

#include "../common.h"
#include "../internal/ds_vector.h"

extern zend_class_entry *ds_vector_ce;

typedef struct php_ds_vector {
    zend_object      std;
    ds_vector_t     *vector;
} php_ds_vector_t;

#define Z_VECTOR(z)   ((php_ds_vector_t*)(Z_OBJ(z)))->vector
#define Z_VECTOR_P(z) Z_VECTOR(*z)
#define THIS_VECTOR() Z_VECTOR_P(getThis())

#define ZVAL_VECTOR(z, v)  \
do { \
    ZVAL_OBJ(z, php_ds_vector_create_object_ex(v)); \
} while(0)

#define ZVAL_NEW_VECTOR(z) \
do { \
    ds_vector_t *_v = ds_vector(); \
    ZVAL_OBJ(z, php_ds_vector_create_object_ex(_v)); \
} while(0)

#define RETURN_VECTOR(v)              \
do {                                  \
    if (v) {                          \
        ZVAL_VECTOR(return_value, v); \
    } else {                          \
        ZVAL_NULL(return_value);      \
    }                                 \
    return;                           \
} while(0)

zend_object *php_ds_vector_create_object_ex(ds_vector_t *vector);
zend_object *php_ds_vector_create_object(zend_class_entry *ce);
zend_object *php_ds_vector_create_clone(ds_vector_t *vector);

int php_ds_vector_serialize(zval *object, unsigned char **buffer, size_t *length, zend_serialize_data *data);
int php_ds_vector_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t length, zend_unserialize_data *data);

#endif
