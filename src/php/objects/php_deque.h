#ifndef PHP_DS_DEQUE_H
#define PHP_DS_DEQUE_H

#include "../../ds/ds_deque.h"

typedef struct php_ds_deque {
    ds_deque_t  *deque;
    zend_object  std;
} php_ds_deque_t;

static inline php_ds_deque_t *php_ds_deque_fetch_object(zend_object *obj) {
	return (php_ds_deque_t *)((char*)(obj) - XtOffsetOf(php_ds_deque_t, std));
}

#define Z_DS_DEQUE(z)   (php_ds_deque_fetch_object(Z_OBJ(z))->deque)
#define Z_DS_DEQUE_P(z) Z_DS_DEQUE(*z)
#define THIS_DS_DEQUE() Z_DS_DEQUE_P(getThis())

#define ZVAL_DS_DEQUE(z, d)  ZVAL_OBJ(z, php_ds_deque_create_object_ex(d))

#define RETURN_DS_DEQUE(d)                  \
do {                                        \
    ds_deque_t *_d = d;                     \
    if (_d) {                               \
        ZVAL_DS_DEQUE(return_value, _d);    \
    } else {                                \
        ZVAL_NULL(return_value);            \
    }                                       \
    return;                                 \
} while(0)


/**
 * Creates a new zend_object using an existing deque.
 */
zend_object *php_ds_deque_create_object_ex(ds_deque_t *deque);

/**
 * Creates a new deque zend_object.
 */
zend_object *php_ds_deque_create_object(zend_class_entry *ce);

/**
 * Creates an object clone of a deque.
 */
zend_object *php_ds_deque_create_clone(ds_deque_t *deque);

PHP_DS_SERIALIZE_FUNCIONS(php_ds_deque);

#endif
