#ifndef DS_HTABLE_ITERATOR_H
#define DS_HTABLE_ITERATOR_H

#include "php.h"
#include "../internal/php_htable.h"

typedef struct _HTableIterator {

    zend_object_iterator     intern;
    uint32_t                 position;
    HBucket                 *b;
    HTable                  *h;

} HTableIterator;

/**
 *
//  */
// zend_object_iterator *htable_get_value_iterator (zend_class_entry *ce, zval *obj, int by_ref);
// zend_object_iterator *htable_get_key_iterator   (zend_class_entry *ce, zval *obj, int by_ref);
// zend_object_iterator *htable_get_pair_iterator  (zend_class_entry *ce, zval *obj, int by_ref);
// zend_object_iterator *htable_get_assoc_iterator (zend_class_entry *ce, zval *obj, int by_ref);

zend_object_iterator *htable_get_value_iterator_ex  (zend_class_entry *ce, zval *obj, int by_ref, HTable *h);
zend_object_iterator *htable_get_key_iterator_ex    (zend_class_entry *ce, zval *obj, int by_ref, HTable *h);
zend_object_iterator *htable_get_pair_iterator_ex   (zend_class_entry *ce, zval *obj, int by_ref, HTable *h);
zend_object_iterator *htable_get_assoc_iterator_ex  (zend_class_entry *ce, zval *obj, int by_ref, HTable *h);

#endif
