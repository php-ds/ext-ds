#include "../iterators/php_set_iterator.h"
#include "../handlers/php_set_handlers.h"
#include "../classes/php_set_ce.h"

#include "php_set.h"

zend_object *php_ds_set_create_object_ex(ds_set_t *set)
{
#if PHP_VERSION_ID < 70300
    php_ds_set_t *obj = ecalloc(1, sizeof(php_ds_set_t) + zend_object_properties_size(php_ds_set_ce));
#else
    php_ds_set_t *obj = zend_object_alloc(sizeof(php_ds_set_t), php_ds_set_ce);
#endif
    zend_object_std_init(&obj->std, php_ds_set_ce);
    obj->std.handlers = &php_ds_set_handlers;
    obj->set = set;
    return &obj->std;
}

zend_object *php_ds_set_create_object(zend_class_entry *ce)
{
    return php_ds_set_create_object_ex(ds_set());
}

zend_object *php_ds_set_create_clone(ds_set_t *set)
{
    return php_ds_set_create_object_ex(ds_set_clone(set));
}

int php_ds_set_serialize(zval *object, unsigned char **buffer, size_t *length, zend_serialize_data *data)
{
    ds_set_t *set = Z_DS_SET_P(object);

    php_serialize_data_t serialize_data = (php_serialize_data_t) data;
    PHP_VAR_SERIALIZE_INIT(serialize_data);

    if (set->table->size == 0) {
        SERIALIZE_SET_ZSTR(ZSTR_EMPTY_ALLOC());

    } else {

        zval *key;
        smart_str buf = {0};

        DS_HTABLE_FOREACH_KEY(set->table, key) {
            php_var_serialize(&buf, key, &serialize_data);
        }
        DS_HTABLE_FOREACH_END();

        smart_str_0(&buf);
        SERIALIZE_SET_ZSTR(buf.s);
        zend_string_release(buf.s);
    }

    PHP_VAR_SERIALIZE_DESTROY(serialize_data);
    return SUCCESS;
}

int php_ds_set_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t length, zend_unserialize_data *data)
{
    ds_set_t *set = ds_set();

    php_unserialize_data_t unserialize_data = (php_unserialize_data_t) data;

    const unsigned char *pos = buffer;
    const unsigned char *end = buffer + length;

    PHP_VAR_UNSERIALIZE_INIT(unserialize_data);
    ZVAL_DS_SET(object, set);

    while (pos != end) {
        zval *value = var_tmp_var(&unserialize_data);

        if ( ! php_var_unserialize(value, &pos, end, &unserialize_data)) {
            goto error;
        }

        ds_set_add(set, value);
    }

    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    return SUCCESS;

error:
    ds_set_free(set);
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    UNSERIALIZE_ERROR();
    return FAILURE;
}
