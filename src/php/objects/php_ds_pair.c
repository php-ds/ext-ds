// #include "../iterators/php_pair_iterator.h"
#include "../handlers/php_pair_handlers.h"
#include "../classes/php_ce_pair.h"

#include "php_ds_pair.h"


zend_object *php_ds_pair_create_object_ex(ds_pair_t *pair)
{
    php_ds_pair_t *obj = ecalloc(1, sizeof(php_ds_pair_t));
    zend_object_std_init(&obj->std, php_ds_pair_ce);
    obj->std.handlers = &php_ds_pair_handlers;
    obj->pair = pair;

    return &obj->std;
}

zend_object *php_ds_pair_create_object(zend_class_entry *ce)
{
    return php_ds_pair_create_object_ex(ds_pair());
}

zend_object *php_ds_pair_create_clone(ds_pair_t *pair)
{
    return php_ds_pair_create_object_ex(ds_pair_clone(pair));
}

int php_ds_pair_serialize(zval *object, unsigned char **buffer, size_t *length, zend_serialize_data *data)
{
    smart_str buf = {0};

    ds_pair_t *pair = Z_DS_PAIR_P(object);

    php_serialize_data_t serialize_data = (php_serialize_data_t) data;
    PHP_VAR_SERIALIZE_INIT(serialize_data);

    php_var_serialize(&buf, &pair->key, &serialize_data);
    php_var_serialize(&buf, &pair->value, &serialize_data);

    smart_str_0(&buf);
    SERIALIZE_SET_ZSTR(buf.s);
    zend_string_release(buf.s);

    PHP_VAR_SERIALIZE_DESTROY(serialize_data);
    return SUCCESS;
}

int php_ds_pair_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t length, zend_unserialize_data *data)
{
    zval *key, *value;
    php_unserialize_data_t unserialize_data = (php_unserialize_data_t) data;

    const unsigned char *pos = buffer;
    const unsigned char *max = buffer + length;

    PHP_VAR_UNSERIALIZE_INIT(unserialize_data);

    key = var_tmp_var(&unserialize_data);
    if (php_var_unserialize(key, &pos, max, &unserialize_data)) {
        var_push_dtor(&unserialize_data, key);
    } else {
        goto error;
    }

    value = var_tmp_var(&unserialize_data);
    if (php_var_unserialize(value, &pos, max, &unserialize_data)) {
        var_push_dtor(&unserialize_data, value);
    } else {
        goto error;
    }

    if (*(++pos) != '\0') {
        goto error;
    }

    ZVAL_DS_PAIR(object, ds_pair_ex(key, value));
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    return SUCCESS;

error:
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    UNSERIALIZE_ERROR();
    return FAILURE;
}
