#include "php_common_handlers.h"
#include "php_deque_handlers.h"

#include "../objects/php_priority_queue.h"
#include "../../ds/ds_priority_queue.h"

zend_object_handlers php_priority_queue_handlers;

static void php_ds_priority_queue_free_object(zend_object *object)
{
    php_ds_priority_queue_t *queue = php_ds_priority_queue_fetch_object(object);
    ds_priority_queue_free(queue->queue);

    if (queue->gc_data != NULL) {
        efree(queue->gc_data);
    }
    zend_object_std_dtor(&queue->std);
}

static int php_ds_priority_queue_count_elements
#if PHP_VERSION_ID >= 80000
(zend_object *obj, zend_long *count) {
    ds_priority_queue_t *pq = php_ds_priority_queue_fetch_object(obj)->queue;
#else
(zval *obj, zend_long *count) {
    ds_priority_queue_t *pq = Z_DS_PRIORITY_QUEUE_P(obj);
#endif
    *count = DS_PRIORITY_QUEUE_SIZE(pq);
    return SUCCESS;
}

static zend_object *php_ds_priority_queue_clone_obj
#if PHP_VERSION_ID >= 80000
(zend_object *obj) {
    ds_priority_queue_t *pq = php_ds_priority_queue_fetch_object(obj)->queue;
#else
(zval *obj) {
    ds_priority_queue_t *pq = Z_DS_PRIORITY_QUEUE_P(obj);
#endif
    return php_ds_priority_queue_create_clone(pq);  
}

static HashTable *php_ds_priority_queue_get_debug_info
#if PHP_VERSION_ID >= 80000
(zend_object *obj, int *is_temp) {
    ds_priority_queue_t *pq = php_ds_priority_queue_fetch_object(obj)->queue;
#else
(zval *obj, int *is_temp) {
    ds_priority_queue_t *pq = Z_DS_PRIORITY_QUEUE_P(obj);
#endif
    zval arr;
    *is_temp = 1;
    ds_priority_queue_to_array(pq, &arr);
    return Z_ARRVAL(arr);
}

static HashTable *php_ds_priority_queue_get_gc
#if PHP_VERSION_ID >= 80000
(zend_object *obj, zval **gc_data, int *gc_size) {
    ds_priority_queue_t *pq = php_ds_priority_queue_fetch_object(obj)->queue;
#else
(zval *obj, zval **gc_data, int *gc_size) {
    ds_priority_queue_t *pq = Z_DS_PRIORITY_QUEUE_P(obj);
#endif
    if (DS_PRIORITY_QUEUE_IS_EMPTY(pq)) {
        *gc_data = NULL;
        *gc_size = 0;
    } else {
        *gc_data = (zval*) pq->nodes;
        *gc_size = pq->size * 2;
    }
    return NULL;
}

void php_ds_register_priority_queue_handlers()
{
    memcpy(&php_priority_queue_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    php_priority_queue_handlers.offset = XtOffsetOf(php_ds_priority_queue_t, std);

    php_priority_queue_handlers.get_gc            = php_ds_priority_queue_get_gc;
    php_priority_queue_handlers.free_obj          = php_ds_priority_queue_free_object;
    php_priority_queue_handlers.clone_obj         = php_ds_priority_queue_clone_obj;
    php_priority_queue_handlers.cast_object       = php_ds_default_cast_object;
    php_priority_queue_handlers.get_debug_info    = php_ds_priority_queue_get_debug_info;
    php_priority_queue_handlers.count_elements    = php_ds_priority_queue_count_elements;
}
