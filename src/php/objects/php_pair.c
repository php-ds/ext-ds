#include "../handlers/php_pair_handlers.h"
#include "../classes/php_pair_ce.h"
#include "php_pair.h"

zend_object *php_ds_pair_create_object(zend_class_entry *ce)
{
    php_ds_pair_t *obj = ecalloc(1, sizeof(php_ds_pair_t) + zend_object_properties_size(ce));
    zend_object_std_init(&obj->std, php_ds_pair_ce);
    object_properties_init(&obj->std, php_ds_pair_ce);
    obj->std.handlers = &php_pair_handlers;

    return &obj->std;
}

php_ds_pair_t *php_ds_pair()
{
    return (php_ds_pair_t*) php_ds_pair_create_object(php_ds_pair_ce);
}

php_ds_pair_t *php_ds_pair_ex(zval *key, zval *value)
{
    php_ds_pair_t *obj = php_ds_pair();

    php_ds_pair_set_key(obj, key);
    php_ds_pair_set_value(obj, value);

    return obj;
}

zend_object *php_ds_pair_create_clone(php_ds_pair_t *obj)
{
    zval *key = php_ds_pair_get_key(obj);
    zval *val = php_ds_pair_get_value(obj);

    return (zend_object *) php_ds_pair_ex(key, val);
}

void php_ds_pair_set_key(php_ds_pair_t *pair, zval *key)
{
    zval tmp;
    ZVAL_DS_PAIR(&tmp, pair);
    zend_update_property(php_ds_pair_ce, &tmp, STR_AND_LEN("key"), key);
}

void php_ds_pair_set_value(php_ds_pair_t *pair, zval *value)
{
    zval tmp;
    ZVAL_DS_PAIR(&tmp, pair);
    zend_update_property(php_ds_pair_ce, &tmp, STR_AND_LEN("value"), value);
}

zval *php_ds_pair_get_key(php_ds_pair_t *pair)
{
    zval tmp;
    zval rv;
    ZVAL_DS_PAIR(&tmp, pair);
    return zend_read_property(php_ds_pair_ce, &tmp, STR_AND_LEN("key"), false, &rv);
}

zval *php_ds_pair_get_value(php_ds_pair_t *pair)
{
    zval tmp;
    zval rv;
    ZVAL_DS_PAIR(&tmp, pair);
    return zend_read_property(php_ds_pair_ce, &tmp, STR_AND_LEN("value"), false, &rv);
}

void php_ds_pair_to_array(php_ds_pair_t *obj, zval *result)
{
    zval *key = php_ds_pair_get_key(obj);
    zval *val = php_ds_pair_get_value(obj);

    array_init_size(result, 2);

    add_assoc_zval(result, "key", key);
    add_assoc_zval(result, "value", val);

    Z_TRY_ADDREF_P(key);
    Z_TRY_ADDREF_P(val);
}

int php_ds_pair_serialize(zval *object, unsigned char **buffer, size_t *length, zend_serialize_data *data)
{
    zval *key, *val;
    smart_str buf = {0};

    php_ds_pair_t *pair = Z_DS_PAIR_P(object);

    php_serialize_data_t serialize_data = (php_serialize_data_t) data;
    PHP_VAR_SERIALIZE_INIT(serialize_data);

    key = php_ds_pair_get_key(pair);
    val = php_ds_pair_get_value(pair);

    php_var_serialize(&buf, key, &serialize_data);
    php_var_serialize(&buf, val, &serialize_data);

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
    if ( ! php_var_unserialize(key, &pos, max, &unserialize_data)) {
        goto error;
    }

    value = var_tmp_var(&unserialize_data);
    if ( ! php_var_unserialize(value, &pos, max, &unserialize_data)) {
        goto error;
    }

    ZVAL_DS_PAIR(object, php_ds_pair_ex(key, value));
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    return SUCCESS;

error:
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    UNSERIALIZE_ERROR();
    return FAILURE;
}
