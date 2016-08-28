#ifndef PHP_DS_QUEUE_H
#define PHP_DS_QUEUE_H

#include "../../ds/ds_queue.h"

#define Z_DS_QUEUE(z)   (((php_ds_queue_t*)(Z_OBJ(z)))->queue)
#define Z_DS_QUEUE_P(z) Z_DS_QUEUE(*z)

#define ZVAL_DS_QUEUE(z, queue) \
    ZVAL_OBJ(z, php_ds_queue_create_object_ex(queue))

typedef struct _php_ds_queue_t {
    zend_object    std;
    ds_queue_t    *queue;
} php_ds_queue_t;

zend_object *php_ds_queue_create_object_ex(ds_queue_t *queue);
zend_object *php_ds_queue_create_object(zend_class_entry *ce);
zend_object *php_ds_queue_create_clone(ds_queue_t *queue);

PHP_DS_SERIALIZE_FUNCIONS(php_ds_queue);

#endif
