#ifndef PHP_DS_PRIORITY_QUEUE_H
#define PHP_DS_PRIORITY_QUEUE_H

#include "../../ds/ds_priority_queue.h"

#define Z_DS_PRIORITY_QUEUE(z)   (((php_ds_priority_queue_t*)(Z_OBJ(z)))->queue)
#define Z_DS_PRIORITY_QUEUE_P(z) Z_DS_PRIORITY_QUEUE(*z)

#define ZVAL_DS_PRIORITY_QUEUE(z, pq) \
    ZVAL_OBJ(z, (php_ds_priority_queue_create_object_ex(pq)))

typedef struct php_ds_priority_queue {
    zend_object          std;
    ds_priority_queue_t *queue;
    zval                *gc_data;
    int                  gc_size;
} php_ds_priority_queue_t;

zend_object *php_ds_priority_queue_create_object_ex(ds_priority_queue_t *pq);
zend_object *php_ds_priority_queue_create_object(zend_class_entry *ce);
zend_object *php_ds_priority_queue_create_clone(ds_priority_queue_t *pq);

PHP_DS_SERIALIZE_FUNCIONS(php_ds_priority_queue);

#endif
