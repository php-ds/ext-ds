#ifndef PHP_DS_DEQUE_H
#define PHP_DS_DEQUE_H

#include "../../ds/ds_deque.h"

#define Z_DS_DEQUE(z)        ((((php_ds_sequence_t*)(Z_OBJ(z))))->deque)
#define Z_DS_DEQUE_P(z)      Z_DS_DEQUE(*z)

#define ZVAL_DS_SEQUENCE(z, deque)  ZVAL_OBJ(z, php_ds_sequence_create_object_ex(deque))

/**
 *
 */
typedef struct php_ds_sequence {
    zend_object  std;
    ds_deque_t  *deque;
} php_ds_sequence_t;

/**
 * Creates a new zend_object using an existing deque.
 */
zend_object *php_ds_sequence_create_object_ex(ds_deque_t *deque);

/**
 * Creates a new deque zend_object.
 */
zend_object *php_ds_sequence_create_object(zend_class_entry *ce);

/**
 * Creates an object clone of a deque.
 */
zend_object *php_ds_sequence_create_clone(ds_deque_t *deque);

PHP_DS_SERIALIZE_FUNCIONS(php_ds_sequence);

#endif