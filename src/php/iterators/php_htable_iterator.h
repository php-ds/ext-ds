#ifndef DS_HTABLE_ITERATOR_H
#define DS_HTABLE_ITERATOR_H

#include "php.h"
#include "../../ds/ds_htable.h"

typedef struct _ds_htable_tIterator {

    zend_object_iterator     intern;
    uint32_t                 position;
    ds_htable_bucket_t                 *b;
    ds_htable_t                  *h;

} ds_htable_tIterator;

/**
 *
//  */
// zend_object_iterator *ds_htable_get_value_iterator (zend_class_entry *ce, zval *obj, int by_ref);
// zend_object_iterator *ds_htable_get_key_iterator   (zend_class_entry *ce, zval *obj, int by_ref);
// zend_object_iterator *ds_htable_get_pair_iterator  (zend_class_entry *ce, zval *obj, int by_ref);
// zend_object_iterator *ds_htable_get_assoc_iterator (zend_class_entry *ce, zval *obj, int by_ref);

zend_object_iterator *ds_htable_get_value_iterator_ex  (zend_class_entry *ce, zval *obj, int by_ref, ds_htable_t *h);
zend_object_iterator *ds_htable_get_key_iterator_ex    (zend_class_entry *ce, zval *obj, int by_ref, ds_htable_t *h);
zend_object_iterator *ds_htable_get_pair_iterator_ex   (zend_class_entry *ce, zval *obj, int by_ref, ds_htable_t *h);
zend_object_iterator *ds_htable_get_assoc_iterator_ex  (zend_class_entry *ce, zval *obj, int by_ref, ds_htable_t *h);

#endif
