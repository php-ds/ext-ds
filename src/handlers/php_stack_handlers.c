
#include "php.h"
#include "ext/spl/spl_exceptions.h"
#include "../common.h"
#include "../classes/php_ce_stack.h"
#include "../internal/php_stack.h"

zend_object_handlers stack_handlers;

static void stack_write_dimension(zval *obj, zval *offset, zval *value)
{
    Stack *stack = Z_STACK_P(obj);

    if (offset == NULL) {
        stack_push_one(stack, value);
        return;
    }

    ARRAY_ACCESS_BY_KEY_NOT_SUPPORTED();
}

static void stack_free_object(zend_object *object)
{
    Stack *stack = (Stack*) object;
    zend_object_std_dtor(&stack->std);
    stack_destroy(stack);
}

static int stack_count_elements(zval *obj, zend_long *count)
{
    *count = STACK_SIZE(Z_STACK_P(obj));
    return SUCCESS;
}

static zend_object *stack_clone_obj(zval *obj)
{
    Stack *s = Z_STACK_P(obj);
    return stack_create_clone(s);
}

static HashTable *stack_get_debug_info(zval *obj, int *is_temp)
{
    zval arr;
    Stack *s = Z_STACK_P(obj);

    *is_temp = 1;

    stack_to_array(s, &arr);
    return Z_ARRVAL(arr);
}

void register_stack_handlers()
{
    memcpy(&stack_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    stack_handlers.offset = XtOffsetOf(Stack, std);

    stack_handlers.dtor_obj          = zend_objects_destroy_object;
    stack_handlers.free_obj          = stack_free_object;

    stack_handlers.clone_obj         = stack_clone_obj;
    stack_handlers.cast_object       = ds_default_cast_object;

    stack_handlers.get_debug_info    = stack_get_debug_info;
    // stack_handlers.get_properties    = stack_get_properties;

    stack_handlers.count_elements    = stack_count_elements;

    stack_handlers.write_dimension   = stack_write_dimension;
    // stack_handlers.read_dimension   = stack_read_dimension;
    // stack_handlers.has_dimension    = stack_has_dimension;
    // stack_handlers.unset_dimension  = stack_unset_dimension;
}
