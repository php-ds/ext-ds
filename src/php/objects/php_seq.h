#ifndef PHP_DS_SEQ_H
#define PHP_DS_SEQ_H

#include "../../ds/ds_seq.h"

typedef struct php_ds_seq {
    ds_seq_t  *seq;
    zend_object  std;
} php_ds_seq_t;

static inline php_ds_seq_t *php_ds_seq_fetch_object(zend_object *obj) {
	return (php_ds_seq_t *)((char*)(obj) - XtOffsetOf(php_ds_seq_t, std));
}

#define Z_DS_SEQ(z)   (php_ds_seq_fetch_object(Z_OBJ(z))->seq)
#define Z_DS_SEQ_P(z) Z_DS_SEQ(*z)
#define THIS_DS_SEQ() Z_DS_SEQ_P(getThis())

#define ZVAL_DS_SEQ(z, d)  ZVAL_OBJ(z, php_ds_seq_create_object_ex(d))

#define RETURN_DS_SEQ(d)                  \
do {                                        \
    ds_seq_t *_d = d;                     \
    if (_d) {                               \
        ZVAL_DS_SEQ(return_value, _d);    \
    } else {                                \
        ZVAL_NULL(return_value);            \
    }                                       \
    return;                                 \
} while(0)


/**
 * Creates a new zend_object using an existing seq.
 */
zend_object *php_ds_seq_create_object_ex(ds_seq_t *seq);

/**
 * Creates a new seq zend_object.
 */
zend_object *php_ds_seq_create_object(zend_class_entry *ce);

/**
 * Creates an object clone of a seq.
 */
zend_object *php_ds_seq_create_clone(ds_seq_t *seq);

PHP_DS_SERIALIZE_FUNCIONS(php_ds_seq);

#endif
