#ifndef PHP_DS_QUEUE_H
#define PHP_DS_QUEUE_H

#include "../../ds/ds_queue.h"

#define Z_DS_QUEUE(z)   (((php_ds_queue_t*)(Z_OBJ(z)))->queue)
#define Z_DS_QUEUE_P(z) Z_DS_QUEUE(*z)
#define THIS_DS_QUEUE() Z_DS_QUEUE_P(getThis())

#define ZVAL_DS_QUEUE(z, queue) \
    ZVAL_OBJ(z, php_ds_queue_create_object_ex(queue))

#define RETURN_DS_QUEUE(q)                  \
do {                                        \
    ds_queue_t *_q = q;                     \
    if (_q) {                               \
        ZVAL_DS_QUEUE(return_value, _q);    \
    } else {                                \
        ZVAL_NULL(return_value);            \
    }                                       \
    return;                                 \
} while(0)

typedef struct _php_ds_queue_t {
    zend_object    std;
    ds_queue_t    *queue;
} php_ds_queue_t;

zend_object *php_ds_queue_create_object_ex(ds_queue_t *queue);
zend_object *php_ds_queue_create_object(zend_class_entry *ce);
zend_object *php_ds_queue_create_clone(ds_queue_t *queue);

PHP_DS_SERIALIZE_FUNCIONS(php_ds_queue);

#endif
