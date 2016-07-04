#include "../common.h"

#include "../php/iterators/php_stack_iterator.h"
#include "../php/handlers/php_stack_handlers.h"
#include "../php/classes/php_ce_stack.h"

#include "ds_stack.h"

static php_ds_stack_t *php_ds_stack_init_ex(ds_vector_t *vector)
{
    php_ds_stack_t *stack = ecalloc(1, sizeof(php_ds_stack_t));
    zend_object_std_init(&stack->std, php_ds_stack_ce);
    stack->std.handlers = &php_ds_stack_handlers;
    stack->vector = vector;
    return stack;
}

php_ds_stack_t *php_ds_stack_init()
{
    return php_ds_stack_init_ex(ds_vector());
}

zend_object *php_ds_stack_create_object(zend_class_entry *ce)
{
    return &php_ds_stack_init()->std;
}

zend_object *php_ds_stack_create_clone(php_ds_stack_t *stack)
{
    ds_vector_t *cloned = ds_vector_clone(stack->vector);
    return &php_ds_stack_init_ex(cloned)->std;
}

void php_ds_stack_user_allocate(php_ds_stack_t *stack, zend_long capacity)
{
    ds_vector_user_allocate(stack->vector, capacity);
}

zend_long php_ds_stack_capacity(php_ds_stack_t *stack)
{
    return stack->vector->capacity;
}

void php_ds_stack_push(php_ds_stack_t *stack, VA_PARAMS)
{
    ds_vector_push_va(stack->vector, argc, argv);
}

void php_ds_stack_push_one(php_ds_stack_t *stack, zval *value)
{
    ds_vector_push(stack->vector, value);
}

void php_ds_stack_clear(php_ds_stack_t *stack)
{
    ds_vector_clear(stack->vector);
}

void php_ds_stack_push_all(php_ds_stack_t *stack, zval *value)
{
    ds_vector_push_all(stack->vector, value);
}

void php_ds_stack_to_array(php_ds_stack_t *stack, zval *return_value)
{
    zend_ulong size = DS_STACK_SIZE(stack);

    if (size == 0) {
        array_init(return_value);

    } else {
        zval *value;
        array_init_size(return_value, size);

        DS_VECTOR_FOREACH_REVERSED(stack->vector, value) {
            add_next_index_zval(return_value, value);
            Z_TRY_ADDREF_P(value);
        }
        DS_VECTOR_FOREACH_END();
    }
}

void php_ds_stack_pop(php_ds_stack_t *stack, zval *return_value)
{
    ds_vector_pop(stack->vector, return_value);
}

void php_ds_stack_destroy(php_ds_stack_t *stack)
{
    ds_vector_destroy(stack->vector);
}

zval *php_ds_stack_peek(php_ds_stack_t *stack)
{
    return ds_vector_get_last(stack->vector);
}

int php_ds_stack_serialize(zval *object, unsigned char **buffer, size_t *length, zend_serialize_data *data)
{
    php_ds_stack_t *stack = Z_DS_STACK_P(object);

    php_serialize_data_t serialize_data = (php_serialize_data_t) data;
    PHP_VAR_SERIALIZE_INIT(serialize_data);

    if (DS_STACK_SIZE(stack) == 0) {
        SERIALIZE_SET_ZSTR(ZSTR_EMPTY_ALLOC());

    } else {

        zval *value;
        smart_str buf = {0};

        DS_VECTOR_FOREACH(stack->vector, value) {
            php_var_serialize(&buf, value, &serialize_data);
        }
        DS_VECTOR_FOREACH_END();

        smart_str_0(&buf);
        SERIALIZE_SET_ZSTR(buf.s);
        zend_string_release(buf.s);
    }

    PHP_VAR_SERIALIZE_DESTROY(serialize_data);
    return SUCCESS;
}

int php_ds_stack_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t length, zend_unserialize_data *data)
{
    php_ds_stack_t *stack = php_ds_stack_init();

    php_unserialize_data_t unserialize_data = (php_unserialize_data_t) data;

    const unsigned char *pos = buffer;
    const unsigned char *max = buffer + length;

    PHP_VAR_UNSERIALIZE_INIT(unserialize_data);

    while (*pos != '}') {

        zval *value = var_tmp_var(&unserialize_data);

        if (php_var_unserialize(value, &pos, max, &unserialize_data)) {
            var_push_dtor(&unserialize_data, value);
        } else {
            goto error;
        }

        php_ds_stack_push_one(stack, value);
    }

    if (*(++pos) != '\0') {
        goto error;
    }

    ZVAL_OBJ(object, &stack->std);
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    return SUCCESS;

error:
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    UNSERIALIZE_ERROR();
    return FAILURE;
}
