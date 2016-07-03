#ifndef DS_SET_ITERATOR_H
#define DS_SET_ITERATOR_H

#include "php.h"
#include "../../ds/ds_set.h"

/**
 *
 */
zend_object_iterator *set_get_iterator(zend_class_entry *ce, zval *object, int by_ref);

#endif
