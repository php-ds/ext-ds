#ifndef DS_VECTOR_ITERATOR_H
#define DS_VECTOR_ITERATOR_H

#include "php.h"
#include "../../ds/ds_vector.h"

typedef struct php_ds_vector_iterator {
    zend_object_iterator     intern;
    zend_object             *object; // TODO we can cast intern's std to php_ds_vector_t because offset is zero.
    ds_vector_t             *vector; // TODO isn't this on the object above?
    zend_long                position;
} php_ds_vector_iterator_t;

zend_object_iterator *php_ds_vector_get_iterator(zend_class_entry *ce, zval *obj, int by_ref);

#endif
