#include "../common.h"
#include "../classes/php_ce_pair.h"
#include "../handlers/php_pair_handlers.h"

#include "php_pair.h"

void pair_to_array(Pair *pair, zval *array)
{
    array_init_size(array, 2);
    add_next_index_zval(array, &pair->key);
    add_next_index_zval(array, &pair->value);
    Z_TRY_ADDREF_P(&pair->key);
    Z_TRY_ADDREF_P(&pair->value);
}

void pair_destroy(Pair *pair)
{
    DTOR_AND_UNDEF(&pair->key);
    DTOR_AND_UNDEF(&pair->value);
}

static Pair *pair_init()
{
    Pair *pair = ecalloc(1, sizeof(Pair));
    zend_object_std_init(&pair->std, pair_ce);
    pair->std.handlers = &pair_handlers;
    return pair;
}

void pair_create_as_zval(zval *key, zval *value, zval *return_value)
{
    Pair *pair = pair_init();

    ZVAL_COPY(&pair->key, key);
    ZVAL_COPY(&pair->value, value);

    ZVAL_OBJ(return_value, &pair->std);
}

static inline zend_object *pair_create_object_ex(zval *key, zval *value)
{
    Pair *pair = pair_init();

    ZVAL_COPY(&pair->key, key);
    ZVAL_COPY(&pair->value, value);
    return &pair->std;
}

zend_object *pair_create_object(zend_class_entry *ce)
{
    Pair *pair = pair_init();

    ZVAL_UNDEF(&pair->key);
    ZVAL_UNDEF(&pair->value);
    return &pair->std;
}

zend_object *pair_create_clone(Pair *pair)
{
    return pair_create_object_ex(&pair->key, &pair->value);
}

int pair_serialize(zval *object, unsigned char **buffer, size_t *length, zend_serialize_data *data)
{
    smart_str buf = {0};

    Pair *pair = Z_PAIR_P(object);

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

int pair_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t length, zend_unserialize_data *data)
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

    pair_create_as_zval(key, value, object);
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    return SUCCESS;

error:
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    UNSERIALIZE_ERROR();
    return FAILURE;
}
