#include "php_common_handlers.h"
#include "php_deque_handlers.h"

#include "../objects/php_queue.h"
#include "../../ds/ds_queue.h"

zend_object_handlers php_queue_handlers;


static void php_ds_queue_write_dimension
#if PHP_VERSION_ID >= 80000
(zend_object *obj, zval *offset, zval *value) {
    ds_queue_t *queue = php_ds_queue_fetch_object(obj)->queue;
#else
(zval *obj, zval *offset, zval *value) {
    ds_queue_t *queue = Z_DS_QUEUE_P(obj);
#endif
    if (offset == NULL) {
        ds_queue_push_one(queue, value);
        return;
    }
    ARRAY_ACCESS_BY_KEY_NOT_SUPPORTED();
}

static void php_ds_queue_free_object(zend_object *object)
{
    php_ds_queue_t *queue = php_ds_queue_fetch_object(object);
    ds_queue_free(queue->queue);
    zend_object_std_dtor(&queue->std);
}

static int php_ds_queue_count_elements
#if PHP_VERSION_ID >= 80000
(zend_object *obj, zend_long *count) {
    ds_queue_t *queue = php_ds_queue_fetch_object(obj)->queue;
#else
(zval *obj, zend_long *count) {
    ds_queue_t *queue = Z_DS_QUEUE_P(obj);
#endif
    *count = QUEUE_SIZE(queue);
    return SUCCESS;
}

static zend_object *php_ds_queue_clone_obj
#if PHP_VERSION_ID >= 80000
(zend_object *obj) {
    ds_queue_t *queue = php_ds_queue_fetch_object(obj)->queue;
#else
(zval *obj) {
    ds_queue_t *queue = Z_DS_QUEUE_P(obj);
#endif
    return php_ds_queue_create_clone(queue);
}

static HashTable *php_ds_queue_get_debug_info
#if PHP_VERSION_ID >= 80000
(zend_object *obj, int *is_temp) {
    ds_queue_t *queue = php_ds_queue_fetch_object(obj)->queue;
#else
(zval *obj, int *is_temp) {
    ds_queue_t *queue = Z_DS_QUEUE_P(obj);
#endif
    zval arr;
    *is_temp = 1;
    ds_queue_to_array(queue, &arr);
    return Z_ARRVAL(arr);
}

static HashTable *php_ds_queue_get_gc
#if PHP_VERSION_ID >= 80000
(zend_object *obj, zval **gc_data, int *gc_count) {
    ds_queue_t *queue = php_ds_queue_fetch_object(obj)->queue;
#else
(zval *obj, zval **gc_data, int *gc_count) {
    ds_queue_t *queue = Z_DS_QUEUE_P(obj);
#endif
    ds_deque_t *deque = queue->deque;

    *gc_data  = deque->buffer;
    *gc_count = deque->head == 0 ? deque->size : deque->capacity;
    return NULL;
}

void php_ds_register_queue_handlers()
{
    memcpy(&php_queue_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    php_queue_handlers.offset = XtOffsetOf(php_ds_queue_t, std);

    php_queue_handlers.dtor_obj          = zend_objects_destroy_object;
    php_queue_handlers.get_gc            = php_ds_queue_get_gc;
    php_queue_handlers.free_obj          = php_ds_queue_free_object;
    php_queue_handlers.clone_obj         = php_ds_queue_clone_obj;
    php_queue_handlers.cast_object       = php_ds_default_cast_object;
    php_queue_handlers.get_debug_info    = php_ds_queue_get_debug_info;
    php_queue_handlers.count_elements    = php_ds_queue_count_elements;
    php_queue_handlers.write_dimension   = php_ds_queue_write_dimension;
    php_queue_handlers.read_dimension    = php_ds_read_dimension_by_key_not_supported;
    php_queue_handlers.unset_dimension   = php_ds_unset_dimension_by_key_not_supported;
    php_queue_handlers.has_dimension     = php_ds_has_dimension_by_key_not_supported;
}
