#ifndef PHP_DS_PQ_H
#define PHP_DS_PQ_H

#include "../../ds/ds_pq.h"

#define Z_DS_PQ(z)   (((php_ds_pq_t*)(Z_OBJ(z)))->pq)
#define Z_DS_PQ_P(z) Z_DS_PQ(*z)

#define ZVAL_DS_PQ(z, queue) \
    ZVAL_OBJ(z, (php_ds_pq_create_object_ex(queue)))

typedef struct _php_ds_pq_t {
    zend_object          std;
    ds_pq_t             *pq;
    zval                *gc_data;
    int                  gc_size;

} php_ds_pq_t;

zend_object *php_ds_pq_create_object_ex(ds_pq_t *queue);
zend_object *php_ds_pq_create_object(zend_class_entry *ce);
zend_object *php_ds_pq_create_clone(ds_pq_t *queue);

PHP_DS_SERIALIZE_FUNCIONS(php_ds_pq);

#endif
