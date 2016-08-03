#include "php_common_handlers.h"
#include "php_deque_handlers.h"

#include "../objects/php_queue.h"
#include "../../ds/ds_queue.h"

zend_object_handlers php_queue_handlers;

static void ds_queue_write_dimension(zval *obj, zval *offset, zval *value)
{
    ds_queue_t *q = Z_DS_QUEUE_P(obj);

    if (offset == NULL) {
        ds_queue_push_one(q, value);
        return;
    }

    ARRAY_ACCESS_BY_KEY_NOT_SUPPORTED();
}

static void ds_queue_free_object(zend_object *object)
{
    php_ds_queue_t *q = (php_ds_queue_t*) object;
    zend_object_std_dtor(&q->std);
    ds_queue_free(q->queue);
}

static int ds_queue_count_elements(zval *obj, zend_long *count)
{
    *count = QUEUE_SIZE(Z_DS_QUEUE_P(obj));
    return SUCCESS;
}

static zend_object *ds_queue_clone_obj(zval *obj)
{
    ds_queue_t *q = Z_DS_QUEUE_P(obj);
    return php_ds_queue_create_clone(q);
}

static HashTable *ds_queue_get_debug_info(zval *obj, int *is_temp)
{
    zval arr;
    ds_queue_t *q = Z_DS_QUEUE_P(obj);

    *is_temp = 1;

    ds_queue_to_array(q, &arr);
    return Z_ARRVAL(arr);
}

void php_ds_register_queue_handlers()
{
    memcpy(&php_queue_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    php_queue_handlers.offset = XtOffsetOf(php_ds_queue_t, std);

    php_queue_handlers.dtor_obj          = zend_objects_destroy_object;
    php_queue_handlers.free_obj          = ds_queue_free_object;
    php_queue_handlers.clone_obj         = ds_queue_clone_obj;
    php_queue_handlers.cast_object       = ds_default_cast_object;
    php_queue_handlers.get_debug_info    = ds_queue_get_debug_info;
    php_queue_handlers.count_elements    = ds_queue_count_elements;
    php_queue_handlers.write_dimension   = ds_queue_write_dimension;
}
