#ifndef PHP_DS_PRIORITY_QUEUE_H
#define PHP_DS_PRIORITY_QUEUE_H

#include "../../ds/ds_priority_queue.h"

#define Z_DS_PRIORITY_QUEUE(z)   (((php_ds_priority_queue_t*)(Z_OBJ(z)))->pq)
#define Z_DS_PRIORITY_QUEUE_P(z) Z_DS_PRIORITY_QUEUE(*z)
#define THIS_DS_PRIORITY_QUEUE() Z_DS_PRIORITY_QUEUE_P(getThis())

#define ZVAL_DS_PRIORITY_QUEUE(z, pq) \
    ZVAL_OBJ(z, (php_ds_priority_queue_create_object_ex(pq)))

#define RETURN_DS_PRIORITY_QUEUE(pq)                \
do {                                                \
    ds_priority_queue_t *_pq = pq;                  \
    if (_pq) {                                      \
        ZVAL_DS_PRIORITY_QUEUE(return_value, _pq);  \
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

PHP_DS_SERIALIZE_FUNCIONS(php_ds_priority_queue);

#endif
