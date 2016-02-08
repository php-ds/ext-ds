
#include "php.h"
#include "ext/spl/spl_exceptions.h"
#include "../common.h"
#include "../classes/php_ce_queue.h"
#include "../internal/php_queue.h"

zend_object_handlers queue_handlers;

static void queue_write_dimension(zval *obj, zval *offset, zval *value)
{
    Queue *q = Z_QUEUE_P(obj);

    if (offset == NULL) {
        queue_push_one(q, value);
        return;
    }

    ARRAY_ACCESS_BY_KEY_NOT_SUPPORTED();
}

static void queue_free_object(zend_object *object)
{
    Queue *q = (Queue*) object;
    zend_object_std_dtor(&q->std);
    queue_destroy(q);
}

static int queue_count_elements(zval *obj, zend_long *count)
{
    *count = QUEUE_SIZE(Z_QUEUE_P(obj));
    return SUCCESS;
}

static zend_object *queue_clone_obj(zval *obj)
{
    Queue *q = Z_QUEUE_P(obj);
    return queue_create_clone(q);
}

static HashTable *queue_get_debug_info(zval *obj, int *is_temp)
{
    zval arr;
    Queue *q = Z_QUEUE_P(obj);

    *is_temp = 1;

    queue_to_array(q, &arr);
    return Z_ARRVAL(arr);
}

void register_queue_handlers()
{
    memcpy(&queue_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    queue_handlers.offset = XtOffsetOf(Queue, std);

    queue_handlers.dtor_obj          = zend_objects_destroy_object;
    queue_handlers.free_obj          = queue_free_object;

    queue_handlers.clone_obj         = queue_clone_obj;
    queue_handlers.cast_object       = ds_default_cast_object;

    queue_handlers.get_debug_info    = queue_get_debug_info;
    // queue_handlers.get_properties    = queue_get_properties;

    queue_handlers.count_elements    = queue_count_elements;

    queue_handlers.write_dimension   = queue_write_dimension;
    // queue_handlers.read_dimension   = queue_read_dimension;
    // queue_handlers.has_dimension    = queue_has_dimension;
    // queue_handlers.unset_dimension  = queue_unset_dimension;
}
