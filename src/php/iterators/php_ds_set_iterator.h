#ifndef PHP_DS_SET_ITERATOR_H
#define PHP_DS_SET_ITERATOR_H

#include "php.h"
#include "../../ds/ds_set.h"

/**
 *
 */
zend_object_iterator *php_ds_set_get_iterator(zend_class_entry *ce, zval *object, int by_ref);

#endif
