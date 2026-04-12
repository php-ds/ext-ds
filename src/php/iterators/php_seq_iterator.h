#ifndef DS_SEQ_ITERATOR_H
#define DS_SEQ_ITERATOR_H

#include "php.h"
#include "../../ds/ds_seq.h"

typedef struct php_ds_seq_iterator {
    zend_object_iterator    intern;
    zend_object            *object;
    ds_seq_t             *seq;
    zend_long               position;
} php_ds_seq_iterator_t;

zend_object_iterator *php_ds_seq_get_iterator(zend_class_entry *ce, zval *obj, int by_ref);

#endif
