#ifndef PHP_DS_VECTOR_ITERATOR_H
#define PHP_DS_VECTOR_ITERATOR_H

#include "php.h"
#include "../internal/php_vector.h"

typedef struct _vectorIterator {

    zend_object_iterator        intern;
    zend_long                   position;
    Vector                     *vector;

} VectorIterator;

/**
 *
 */
zend_object_iterator *vector_get_iterator_ex(zend_class_entry *ce, zval *obj, int by_ref, Vector *vector);
zend_object_iterator *vector_get_iterator(zend_class_entry *ce, zval *obj, int by_ref);

#endif
