#include "../iterators/php_stack_iterator.h"
#include "../handlers/php_stack_handlers.h"
#include "../classes/php_stack_ce.h"

#include "php_stack.h"

zend_object *php_ds_stack_create_object_ex(ds_stack_t *stack)
{
    php_ds_stack_t *obj = ecalloc(1, sizeof(php_ds_stack_t));
    zend_object_std_init(&obj->std, php_ds_stack_ce);
    obj->std.handlers = &php_ds_stack_handlers;
    obj->stack = stack;
    return &obj->std;
}

zend_object *php_ds_stack_create_object(zend_class_entry *ce)
{
    return php_ds_stack_create_object_ex(ds_stack());
}

zend_object *php_ds_stack_create_clone(ds_stack_t *stack)
{
    return php_ds_stack_create_object_ex(ds_stack_clone(stack));
}

int php_ds_stack_serialize(zval *object, unsigned char **buffer, size_t *length, zend_serialize_data *data)
{
    ds_stack_t *stack = Z_DS_STACK_P(object);

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
    ds_stack_t *stack = ds_stack();

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

        ds_stack_push_one(stack, value);
    }

    if (*(++pos) != '\0') {
        goto error;
    }

    ZVAL_DS_STACK(object, stack);
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    return SUCCESS;

error:
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    UNSERIALIZE_ERROR();
    return FAILURE;
}
