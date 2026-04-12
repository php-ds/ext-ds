#include "php_common_handlers.h"
#include "php_heap_handlers.h"

#include "../objects/php_heap.h"
#include "../../ds/ds_heap.h"

zend_object_handlers php_heap_handlers;

static void php_ds_heap_free_object(zend_object *object)
{
    php_ds_heap_t *obj = php_ds_heap_fetch_object(object);
    ds_heap_release(obj->heap);

    if (obj->has_comparator && ZEND_FCI_INITIALIZED(obj->fci)) {
        zval_ptr_dtor(&obj->fci.function_name);
    }

    zend_object_std_dtor(&obj->std);
}

static int php_ds_heap_count_elements(zend_object *obj, zend_long *count)
{
    *count = php_ds_heap_fetch_object(obj)->heap->size;
    return SUCCESS;
}

static zend_object *php_ds_heap_clone_obj(zend_object *obj)
{
    return php_ds_heap_create_clone(php_ds_heap_fetch_object(obj));
}

static HashTable *php_ds_heap_get_debug_info(zend_object *obj, int *is_temp)
{
    php_ds_heap_t *heap_obj = php_ds_heap_fetch_object(obj);
    ds_heap_t *heap = heap_obj->heap;
    zval arr;
    *is_temp = 1;
    ds_heap_to_array(heap, &arr, php_ds_heap_get_compare(heap_obj));
    return Z_ARRVAL(arr);
}

static HashTable *php_ds_heap_get_gc(zend_object *obj, zval **gc_data, int *gc_count)
{
    ds_heap_t *heap = php_ds_heap_fetch_object(obj)->heap;
    *gc_data  = heap->nodes;
    *gc_count = (int) heap->size;
    return NULL;
}

void php_ds_register_heap_handlers()
{
    memcpy(&php_heap_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    php_heap_handlers.offset          = XtOffsetOf(php_ds_heap_t, std);
    php_heap_handlers.dtor_obj        = zend_objects_destroy_object;
    php_heap_handlers.free_obj        = php_ds_heap_free_object;
    php_heap_handlers.get_gc          = php_ds_heap_get_gc;
    php_heap_handlers.cast_object     = php_ds_default_cast_object;
    php_heap_handlers.clone_obj       = php_ds_heap_clone_obj;
    php_heap_handlers.get_debug_info  = php_ds_heap_get_debug_info;
    php_heap_handlers.count_elements  = php_ds_heap_count_elements;
}
