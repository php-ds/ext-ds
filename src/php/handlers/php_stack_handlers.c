#include "php_stack_handlers.h"
#include "php_common_handlers.h"
// #include "php.h"
// #include "ext/spl/spl_exceptions.h"
#include "../../ds/ds_stack.h"
// #include "../classes/php_ce_stack.h"
// #include "php_stack_handlers.h"

zend_object_handlers php_stack_handlers;

static void php_ds_stack_write_dimension(zval *obj, zval *offset, zval *value)
{
    php_ds_stack_t *stack = Z_DS_STACK_P(obj);

    if (offset == NULL) {
        php_ds_stack_push_one(stack, value);
        return;
    }

    ARRAY_ACCESS_BY_KEY_NOT_SUPPORTED();
}

static void php_ds_stack_free_object(zend_object *object)
{
    php_ds_stack_t *stack = (php_ds_stack_t*) object;
    zend_object_std_dtor(&stack->std);
    php_ds_stack_destroy(stack);
}

static int php_ds_stack_count_elements(zval *obj, zend_long *count)
{
    *count = DS_STACK_SIZE(Z_DS_STACK_P(obj));
    return SUCCESS;
}

static zend_object *php_ds_stack_clone_obj(zval *obj)
{
    php_ds_stack_t *s = Z_DS_STACK_P(obj);
    return php_ds_stack_create_clone(s);
}

static HashTable *php_ds_stack_get_debug_info(zval *obj, int *is_temp)
{
    zval arr;
    php_ds_stack_t *s = Z_DS_STACK_P(obj);

    *is_temp = 1;

    php_ds_stack_to_array(s, &arr);
    return Z_ARRVAL(arr);
}

void php_register_php_ds_stack_handlers()
{
    memcpy(&php_stack_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    php_stack_handlers.offset = XtOffsetOf(php_ds_stack_t, std);

    php_stack_handlers.dtor_obj          = zend_objects_destroy_object;
    php_stack_handlers.free_obj          = php_ds_stack_free_object;

    php_stack_handlers.clone_obj         = php_ds_stack_clone_obj;
    php_stack_handlers.cast_object       = ds_default_cast_object;

    php_stack_handlers.get_debug_info    = php_ds_stack_get_debug_info;
    // php_stack_handlers.get_properties    = php_ds_stack_get_properties;

    php_stack_handlers.count_elements    = php_ds_stack_count_elements;

    php_stack_handlers.write_dimension   = php_ds_stack_write_dimension;
    // php_stack_handlers.read_dimension   = php_ds_stack_read_dimension;
    // php_stack_handlers.has_dimension    = php_ds_stack_has_dimension;
    // php_stack_handlers.unset_dimension  = php_ds_stack_unset_dimension;
}
