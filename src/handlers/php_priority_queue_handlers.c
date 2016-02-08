#include "../common.h"
#include "../classes/php_ce_priority_queue.h"
#include "../internal/php_priority_queue.h"

zend_object_handlers priority_queue_handlers;

static void priority_queue_free_object(zend_object *object)
{
    PriorityQueue *pq = (PriorityQueue*) object;
    zend_object_std_dtor(&pq->std);
    priority_queue_destroy(pq);
}

static int priority_queue_count_elements(zval *obj, zend_long *count)
{
    *count = PRIORITY_QUEUE_SIZE(Z_PRIORITY_QUEUE_P(obj));
    return SUCCESS;
}

static zend_object *priority_queue_clone_obj(zval *obj)
{
    PriorityQueue *pq = Z_PRIORITY_QUEUE_P(obj);
    return priority_queue_init_clone(pq);
}

static HashTable *priority_queue_get_debug_info(zval *obj, int *is_temp)
{
    zval array;
    PriorityQueue *pq = Z_PRIORITY_QUEUE_P(obj);

    *is_temp = 1;

    priority_queue_to_array(pq, &array);
    return Z_ARRVAL(array);
}

void register_priority_queue_handlers()
{
    memcpy(&priority_queue_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    priority_queue_handlers.offset = XtOffsetOf(PriorityQueue, std);

    priority_queue_handlers.free_obj          = priority_queue_free_object;
    priority_queue_handlers.clone_obj         = priority_queue_clone_obj;
    priority_queue_handlers.cast_object       = ds_default_cast_object;
    priority_queue_handlers.get_debug_info    = priority_queue_get_debug_info;

    priority_queue_handlers.count_elements    = priority_queue_count_elements;
 // priority_queue_handlers.get_properties    = priority_queue_get_properties;
}
