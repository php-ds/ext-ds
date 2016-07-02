#ifndef DS_VECTOR_ITERATOR_H
#define DS_VECTOR_ITERATOR_H

#include "php.h"
#include "../internal/ds_vector.h"

typedef struct php_ds_vector_iterator {
    zend_object_iterator   intern;
    zend_long              position;
    ds_vector_t           *vector;
} php_ds_vector_iterator_t;

/**
 *
 */
zend_object_iterator *php_ds_vector_get_iterator_ex(zend_class_entry *ce, zval *obj, int by_ref, ds_vector_t *vector);
zend_object_iterator *php_ds_vector_get_iterator(zend_class_entry *ce, zval *obj, int by_ref);

#endif
