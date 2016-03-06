#include "../common.h"
#include "../classes/php_ce_stack.h"
#include "../handlers/php_stack_handlers.h"
#include "php_vector.h"
#include "php_stack.h"

static Stack *stack_init_ex(ds_vector_t *vector)
{
    Stack *stack = ecalloc(1, sizeof(Stack));
    zend_object_std_init(&stack->std, stack_ce);
    stack->std.handlers = &stack_handlers;
    stack->vector = vector;
    return stack;
}

Stack *stack_init()
{
    return stack_init_ex(ds_vector());
}

zend_object *stack_create_object(zend_class_entry *ce)
{
    return &stack_init()->std;
}

zend_object *stack_create_clone(Stack *stack)
{
    ds_vector_t *cloned = ds_vector_create_copy(stack->vector);
    return &stack_init_ex(cloned)->std;
}

void stack_user_allocate(Stack *stack, zend_long capacity)
{
    ds_vector_user_allocate(stack->vector, capacity);
}

zend_long stack_capacity(Stack *stack)
{
    return stack->vector->capacity;
}

void stack_push(Stack *stack, VA_PARAMS)
{
    ds_vector_push_va(stack->vector, argc, argv);
}

void stack_push_one(Stack *stack, zval *value)
{
    ds_vector_push(stack->vector, value);
}

void stack_clear(Stack *stack)
{
    ds_vector_clear(stack->vector);
}

void stack_push_all(Stack *stack, zval *value)
{
    ds_vector_push_all(stack->vector, value);
}

void stack_to_array(Stack *stack, zval *return_value)
{
    zend_ulong size = STACK_SIZE(stack);

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

void stack_pop(Stack *stack, zval *return_value)
{
    ds_vector_pop(stack->vector, return_value);
}

void stack_destroy(Stack *stack)
{
    ds_vector_destroy(stack->vector);
}

zval *stack_peek(Stack *stack)
{
    return ds_vector_get_last(stack->vector);
}

int stack_serialize(zval *object, unsigned char **buffer, size_t *length, zend_serialize_data *data)
{
    Stack *stack = Z_STACK_P(object);

    php_serialize_data_t serialize_data = (php_serialize_data_t) data;
    PHP_VAR_SERIALIZE_INIT(serialize_data);

    if (STACK_SIZE(stack) == 0) {
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

int stack_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t length, zend_unserialize_data *data)
{
    Stack *stack = stack_init();

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

        stack_push_one(stack, value);
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
