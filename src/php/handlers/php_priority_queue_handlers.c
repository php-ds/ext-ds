#include "php_common_handlers.h"
#include "php_deque_handlers.h"

#include "../objects/php_priority_queue.h"
#include "../../ds/ds_priority_queue.h"

zend_object_handlers php_priority_queue_handlers;

static void ds_priority_queue_free_object(zend_object *object)
{
    php_ds_priority_queue_t *queue = (php_ds_priority_queue_t*) object;
    zend_object_std_dtor(&queue->std);
    ds_priority_queue_free(queue->pq);
}

static int ds_priority_queue_count_elements(zval *obj, zend_long *count)
{
    *count = DS_PRIORITY_QUEUE_SIZE(Z_DS_PRIORITY_QUEUE_P(obj));
    return SUCCESS;
}

static zend_object *php_ds_priority_queue_create_clone_obj(zval *obj)
{
    ds_priority_queue_t *pq = Z_DS_PRIORITY_QUEUE_P(obj);
    return php_ds_priority_queue_create_clone(pq);
}

static HashTable *ds_priority_queue_get_debug_info(zval *obj, int *is_temp)
{
    zval array;
    ds_priority_queue_t *pq = Z_DS_PRIORITY_QUEUE_P(obj);

    *is_temp = 1;

    ds_priority_queue_to_array(pq, &array);
    return Z_ARRVAL(array);
}

void php_ds_register_priority_queue_handlers()
{
    memcpy(&php_priority_queue_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    php_priority_queue_handlers.offset = XtOffsetOf(php_ds_priority_queue_t, std);

    php_priority_queue_handlers.free_obj          = ds_priority_queue_free_object;
    php_priority_queue_handlers.clone_obj         = php_ds_priority_queue_create_clone_obj;
    php_priority_queue_handlers.cast_object       = ds_default_cast_object;
    php_priority_queue_handlers.get_debug_info    = ds_priority_queue_get_debug_info;

    php_priority_queue_handlers.count_elements    = ds_priority_queue_count_elements;
 // php_priority_queue_handlers.get_properties    = ds_priority_queue_get_properties;
}
