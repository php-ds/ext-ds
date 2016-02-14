#ifndef DS_MAP_ITERATOR_H
#define DS_MAP_ITERATOR_H

#include "php.h"
#include "../internal/php_map.h"

/**
 *
 */
zend_object_iterator *map_get_iterator(zend_class_entry *ce, zval *obj, int by_ref);

#endif
