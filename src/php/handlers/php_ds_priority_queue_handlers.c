#include "php_ds_common_handlers.h"
#include "php_ds_priority_queue_handlers.h"
#include "../objects/php_ds_priority_queue.h"
#include "../../ds/ds_priority_queue.h"

zend_object_handlers php_ds_priority_queue_handlers;

static void php_ds_priority_queue_free_object(zend_object *object)
{
    php_ds_priority_queue_t *queue = (php_ds_priority_queue_t*) object;
    zend_object_std_dtor(&queue->std);
    ds_priority_queue_free(queue->queue);

    if (queue->gc_data != NULL) {
        efree(queue->gc_data);
    }
}

static int php_ds_priority_queue_count_elements(zval *obj, zend_long *count)
{
    *count = DS_PRIORITY_QUEUE_SIZE(Z_DS_PRIORITY_QUEUE_P(obj));
    return SUCCESS;
}

static zend_object *php_ds_priority_queue_clone_obj(zval *obj)
{
    return php_ds_priority_queue_create_clone(Z_DS_PRIORITY_QUEUE_P(obj));
}

static HashTable *php_ds_priority_queue_get_debug_info(zval *obj, int *is_temp)
{
    zval array;
    ds_priority_queue_t *queue = Z_DS_PRIORITY_QUEUE_P(obj);

    *is_temp = 1;

    ds_priority_queue_to_array(queue, &array);
    return Z_ARRVAL(array);
}

static void ds_priority_queue_copy_gc(
    php_ds_priority_queue_t *obj,
    zval **gc_data,
    int *gc_size
) {
    zval *value;
    zval *target = obj->gc_data;

    DS_PRIORITY_QUEUE_FOREACH_VALUE(obj->queue, value) {
        ZVAL_COPY_VALUE(target++, value);
    }
    DS_PRIORITY_QUEUE_FOREACH_END();

    *gc_data = obj->gc_data;
    *gc_size = obj->gc_size;
}

static HashTable *php_ds_priority_queue_get_gc(zval *object, zval **gc_data, int *gc_size)
{
    php_ds_priority_queue_t *obj = (php_ds_priority_queue_t *) Z_OBJ_P(object);

    // We have to reallocate the gc buffer if the size has changed.
    if (obj->gc_size != obj->queue->size) {
        obj->gc_size  = obj->queue->size;
        REALLOC_ZVAL_BUFFER(obj->gc_data, obj->gc_size);
    }

    // Copy every value from the queue in the gc buffer.
    ds_priority_queue_copy_gc(obj, gc_data, gc_size);

    return NULL;
}

void php_ds_register_priority_queue_handlers()
{
    memcpy(&php_ds_priority_queue_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    php_ds_priority_queue_handlers.offset            = 0;
    php_ds_priority_queue_handlers.get_gc            = php_ds_priority_queue_get_gc;
    php_ds_priority_queue_handlers.free_obj          = php_ds_priority_queue_free_object;
    php_ds_priority_queue_handlers.clone_obj         = php_ds_priority_queue_clone_obj;
    php_ds_priority_queue_handlers.cast_object       = php_ds_common_cast_object;
    php_ds_priority_queue_handlers.get_debug_info    = php_ds_priority_queue_get_debug_info;
    php_ds_priority_queue_handlers.count_elements    = php_ds_priority_queue_count_elements;

    //
    php_ds_priority_queue_handlers.read_dimension    = php_ds_common_read_dimension;
    php_ds_priority_queue_handlers.write_dimension   = php_ds_common_write_dimension;
    php_ds_priority_queue_handlers.has_dimension     = php_ds_common_has_dimension;
    php_ds_priority_queue_handlers.unset_dimension   = php_ds_common_unset_dimension;
}
