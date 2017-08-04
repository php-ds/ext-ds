#ifndef PHP_DS_MAP_ITERATOR_H
#define PHP_DS_MAP_ITERATOR_H

#include "php.h"

/**
 *
 */
zend_object_iterator *php_ds_map_get_iterator(zend_class_entry *ce, zval *obj, int by_ref);

#endif
