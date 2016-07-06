#ifndef PHP_DS_PRIORITY_QUEUE_H
#define PHP_DS_PRIORITY_QUEUE_H

#include "../../ds/ds_priority_queue.h"

#define Z_DS_PRIORITY_QUEUE(z)   (((php_ds_priority_queue_t*)(Z_OBJ(z)))->pq)
#define Z_DS_PRIORITY_QUEUE_P(z) Z_DS_PRIORITY_QUEUE(*z)
#define THIS_DS_PRIORITY_QUEUE() Z_DS_PRIORITY_QUEUE_P(getThis())

#define ZVAL_DS_PRIORITY_QUEUE(z, pq)  ZVAL_OBJ(z, (php_ds_priority_queue_create_object_ex(pq)))
#define ZVAL_NEW_DS_PRIORITY_QUEUE(z)  ZVAL_DS_PRIORITY_QUEUE(z, ds_priority_queue())

#define RETURN_DS_PRIORITY_QUEUE(pq)                \
do {                                                \
    if (pq) {                                       \
        ZVAL_DS_PRIORITY_QUEUE(return_value, pq);   \
    } else {                                        \
        ZVAL_NULL(return_value);                    \
    }                                               \
    return;                                         \
} while(0)

typedef struct _php_ds_priority_queue_t {
    zend_object          std;
    ds_priority_queue_t *pq;
} php_ds_priority_queue_t;

zend_object *php_ds_priority_queue_create_object_ex(ds_priority_queue_t *pq);
zend_object *php_ds_priority_queue_create_object(zend_class_entry *ce);
zend_object *php_ds_priority_queue_create_clone(ds_priority_queue_t *pq);

int php_ds_priority_queue_serialize(
    zval                    *object,
    unsigned char          **buffer,
    size_t                  *length,
    zend_serialize_data     *data
);

int php_ds_priority_queue_unserialize(
    zval                    *object,
    zend_class_entry        *ce,
    const unsigned char     *buffer,
    size_t                   length,
    zend_unserialize_data   *data
);

#endif
