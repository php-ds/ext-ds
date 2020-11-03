#include "php_stack_handlers.h"
#include "php_common_handlers.h"
#include "../../ds/ds_stack.h"
#include "../objects/php_stack.h"

zend_object_handlers php_ds_stack_handlers;

static void php_ds_stack_write_dimension
#if PHP_VERSION_ID >= 80000
(zend_object *obj, zval *offset, zval *value) {
    ds_stack_t *stack = php_ds_stack_fetch_object(obj)->stack;
#else
(zval *obj, zval *offset, zval *value) {
    ds_stack_t *stack = Z_DS_STACK_P(obj);
#endif
    if (offset == NULL) {
        ds_stack_push(stack, value);
        return;
    }
    ARRAY_ACCESS_BY_KEY_NOT_SUPPORTED();
}

static void php_ds_stack_free_object(zend_object *object)
{
    php_ds_stack_t *obj = php_ds_stack_fetch_object(object);
	if (obj->stack) {
	    ds_stack_free(obj->stack);
		obj->stack = NULL;
	}
    zend_object_std_dtor(&obj->std);
}

static int php_ds_stack_count_elements
#if PHP_VERSION_ID >= 80000
(zend_object *obj, zend_long *count) {
    ds_stack_t *stack = php_ds_stack_fetch_object(obj)->stack;
#else
(zval *obj, zend_long *count) {
    ds_stack_t *stack = Z_DS_STACK_P(obj);
#endif
    *count = DS_STACK_SIZE(stack);
    return SUCCESS;
}

static zend_object *php_ds_stack_clone_obj
#if PHP_VERSION_ID >= 80000
(zend_object *obj) {
    ds_stack_t *stack = php_ds_stack_fetch_object(obj)->stack;
#else
(zval *obj) {
    ds_stack_t *stack = Z_DS_STACK_P(obj);
#endif 
    return php_ds_stack_create_clone(stack);
}

static HashTable *php_ds_stack_get_debug_info
#if PHP_VERSION_ID >= 80000
(zend_object *obj, int *is_temp) {
    ds_stack_t *stack = php_ds_stack_fetch_object(obj)->stack;
#else
(zval *obj, int *is_temp) {
    ds_stack_t *stack = Z_DS_STACK_P(obj);
#endif 
    zval arr;
    *is_temp = 1;

    ds_stack_to_array(stack, &arr);
    return Z_ARRVAL(arr);
}

static HashTable *php_ds_stack_get_gc
#if PHP_VERSION_ID >= 80000
(zend_object *obj, zval **gc_data, int *gc_count) {
    ds_stack_t *stack = php_ds_stack_fetch_object(obj)->stack;
#else
(zval *obj, zval **gc_data, int *gc_count) {
    ds_stack_t *stack = Z_DS_STACK_P(obj);
#endif 
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
    php_ds_stack_handlers.read_dimension    = php_ds_read_dimension_by_key_not_supported;
    php_ds_stack_handlers.unset_dimension   = php_ds_unset_dimension_by_key_not_supported;
    php_ds_stack_handlers.has_dimension     = php_ds_has_dimension_by_key_not_supported;
}
