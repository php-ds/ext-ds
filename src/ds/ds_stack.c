#include "../ds_common.h"

// #include "../php/iterators/php_ds_stack_iterator.h"
// #include "../php/handlers/php_ds_stack_handlers.h"
// #include "../php/classes/php_ds_stack_ce.h"

#include "ds_stack.h"

ds_stack_t *ds_stack_ex(ds_vector_t *vector)
{
    ds_stack_t *stack = ecalloc(1, sizeof(ds_stack_t));
    stack->vector = vector;
    return stack;
}

ds_stack_t *ds_stack()
{
    return ds_stack_ex(ds_vector());
}

ds_stack_t *ds_stack_clone(ds_stack_t *stack)
{
    return ds_stack_ex(ds_vector_clone(stack->vector));
}

void ds_stack_free(ds_stack_t *stack)
{
    ds_vector_free(stack->vector);
    efree(stack);
}

void ds_stack_allocate(ds_stack_t *stack, zend_long capacity)
{
    ds_vector_allocate(stack->vector, capacity);
}

void ds_stack_push_va(ds_stack_t *stack, VA_PARAMS)
{
    ds_vector_push_va(stack->vector, argc, argv);
}

void ds_stack_push(ds_stack_t *stack, zval *value)
{
    ds_vector_push(stack->vector, value);
}

void ds_stack_clear(ds_stack_t *stack)
{
    ds_vector_clear(stack->vector);
}

void ds_stack_push_all(ds_stack_t *stack, zval *value)
{
    ds_vector_push_all(stack->vector, value);
}

void ds_stack_to_array(ds_stack_t *stack, zval *return_value)
{
    zend_long size = DS_STACK_SIZE(stack);

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

void ds_stack_pop_throw(ds_stack_t *stack, zval *return_value)
{
    ds_vector_pop_throw(stack->vector, return_value);
}

void ds_stack_pop(ds_stack_t *stack, zval *return_value)
{
    ds_vector_pop(stack->vector, return_value);
}

zval *ds_stack_peek(ds_stack_t *stack)
{
    return ds_vector_get_last(stack->vector);
}

zval *ds_stack_peek_throw(ds_stack_t *stack)
{
    return ds_vector_get_last_throw(stack->vector);
}
