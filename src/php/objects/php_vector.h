#ifndef PHP_DS_VECTOR_H
#define PHP_DS_VECTOR_H

#include "../../ds/ds_vector.h"

#define Z_DS_VECTOR(z)   (((php_ds_vector_t*)(Z_OBJ(z)))->vector)
#define Z_DS_VECTOR_P(z) Z_DS_VECTOR(*z)

#define ZVAL_DS_VECTOR(z, v) ZVAL_OBJ(z, php_ds_vector_create_object_ex(v))

typedef struct php_ds_vector {
    zend_object      std;
    ds_vector_t     *vector;
} php_ds_vector_t;

zend_object *php_ds_vector_create_object_ex(ds_vector_t *vector);
zend_object *php_ds_vector_create_object(zend_class_entry *ce);
zend_object *php_ds_vector_create_clone(ds_vector_t *vector);

PHP_DS_SERIALIZE_FUNCIONS(php_ds_vector);

#endif
