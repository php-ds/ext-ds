#include "../../common.h"
#include "../../iterators/php_deque_iterator.h"
#include "../../handlers/php_deque_handlers.h"
#include "../../classes/php_ce_deque.h"
#include "php_ds_deque.h"

zend_object *deque_create_object_ex(ds_deque_t *deque)
{
    php_ds_deque_t *obj = ecalloc(1, sizeof(php_ds_deque_t));
    zend_object_std_init(&obj->std, php_ds_deque_ce);
    obj->std.handlers = &deque_handlers;
    obj->deque = deque;
    return &obj->std;
}

zend_object *deque_create_object(zend_class_entry *ce)
{
    return deque_create_object_ex(ds_deque());
}

zend_object *deque_create_clone(ds_deque_t *deque)
{
    return deque_create_object_ex(deque_create_copy(deque));
}

int deque_serialize(zval *object, unsigned char **buffer, size_t *length, zend_serialize_data *data)
{
    ds_deque_t *deque = Z_DEQUE_P(object);

    php_serialize_data_t serialize_data = (php_serialize_data_t) data;
    PHP_VAR_SERIALIZE_INIT(serialize_data);

    if (DEQUE_IS_EMPTY(deque)) {
        SERIALIZE_SET_ZSTR(ZSTR_EMPTY_ALLOC());

    } else {

        zval *value;
        smart_str buf = {0};

        DEQUE_FOREACH(deque, value) {
            php_var_serialize(&buf, value, &serialize_data);
        }
        DEQUE_FOREACH_END();

        smart_str_0(&buf);
        SERIALIZE_SET_ZSTR(buf.s);
        zend_string_release(buf.s);
    }

    PHP_VAR_SERIALIZE_DESTROY(serialize_data);
    return SUCCESS;
}

int deque_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t length, zend_unserialize_data *data)
{
    ds_deque_t *deque = ds_deque();

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

        deque_push(deque, value);
    }

    if (*(++pos) != '\0') {
        goto error;
    }

    ZVAL_DEQUE(object, deque);
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    return SUCCESS;

error:
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    UNSERIALIZE_ERROR();
    return FAILURE;
}
