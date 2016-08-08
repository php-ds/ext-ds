#include "php_stack_handlers.h"
#include "php_common_handlers.h"
#include "../../ds/ds_stack.h"
#include "../objects/php_stack.h"

zend_object_handlers php_ds_stack_handlers;

static void php_ds_stack_write_dimension(zval *obj, zval *offset, zval *value)
{
    ds_stack_t *stack = Z_DS_STACK_P(obj);

    if (offset == NULL) {
        ds_stack_push(stack, value);
        return;
    }

    ARRAY_ACCESS_BY_KEY_NOT_SUPPORTED();
}

static void php_ds_stack_free_object(zend_object *object)
{
    php_ds_stack_t *obj = (php_ds_stack_t*) object;
    zend_object_std_dtor(&obj->std);
    ds_stack_free(obj->stack);
}

static int php_ds_stack_count_elements(zval *obj, zend_long *count)
{
    *count = DS_STACK_SIZE(Z_DS_STACK_P(obj));
    return SUCCESS;
}

static zend_object *php_ds_stack_clone_obj(zval *obj)
{
    return php_ds_stack_create_clone(Z_DS_STACK_P(obj));
}

static HashTable *php_ds_stack_get_debug_info(zval *obj, int *is_temp)
{
    zval arr;
    ds_stack_t *stack = Z_DS_STACK_P(obj);

    *is_temp = 1;

    ds_stack_to_array(stack, &arr);
    return Z_ARRVAL(arr);
}

static HashTable *php_ds_stack_get_gc(zval *obj, zval **gc_data, int *gc_count)
{
    ds_stack_t *stack = Z_DS_STACK_P(obj);

    *gc_data  = (zval*) stack->vector->buffer;
    *gc_count = (int)   stack->vector->size;

    return NULL;
}

void php_register_ds_stack_handlers()
{
    memcpy(&php_ds_stack_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    php_ds_stack_handlers.offset = XtOffsetOf(php_ds_stack_t, std);

    php_ds_stack_handlers.dtor_obj          = zend_objects_destroy_object;
    php_ds_stack_handlers.get_gc            = php_ds_stack_get_gc;
    php_ds_stack_handlers.free_obj          = php_ds_stack_free_object;
    php_ds_stack_handlers.clone_obj         = php_ds_stack_clone_obj;
    php_ds_stack_handlers.cast_object       = php_ds_default_cast_object;
    php_ds_stack_handlers.get_debug_info    = php_ds_stack_get_debug_info;
    php_ds_stack_handlers.count_elements    = php_ds_stack_count_elements;
    php_ds_stack_handlers.write_dimension   = php_ds_stack_write_dimension;
}
