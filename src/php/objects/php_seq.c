#include "../handlers/php_seq_handlers.h"
#include "../classes/php_seq_ce.h"

#include "php_seq.h"

zend_object *php_ds_seq_create_object_ex(ds_seq_t *seq)
{
    php_ds_seq_t *obj = zend_object_alloc(sizeof(php_ds_seq_t), php_ds_seq_ce);
    zend_object_std_init(&obj->std, php_ds_seq_ce);
    obj->std.handlers = &php_seq_handlers;
    obj->seq = seq;
    return &obj->std;
}

zend_object *php_ds_seq_create_object(zend_class_entry *ce)
{
    return php_ds_seq_create_object_ex(ds_seq());
}

zend_object *php_ds_seq_create_clone(ds_seq_t *seq)
{
    seq->refs++;
    return php_ds_seq_create_object_ex(seq);
}

int php_ds_seq_serialize(zval *object, unsigned char **buffer, size_t *length, zend_serialize_data *data)
{
    ds_seq_t *seq = Z_DS_SEQ_P(object);

    php_serialize_data_t serialize_data = (php_serialize_data_t) data;
    PHP_VAR_SERIALIZE_INIT(serialize_data);

    if (seq->size == 0) {
        SERIALIZE_SET_ZSTR(ZSTR_EMPTY_ALLOC());

    } else {

        zval *value;
        smart_str buf = {0};

        DS_SEQ_FOREACH(seq, value) {
            php_var_serialize(&buf, value, &serialize_data);
        }
        DS_SEQ_FOREACH_END();

        smart_str_0(&buf);
        SERIALIZE_SET_ZSTR(buf.s);
        zend_string_release(buf.s);
    }

    PHP_VAR_SERIALIZE_DESTROY(serialize_data);
    return SUCCESS;
}

int php_ds_seq_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t length, zend_unserialize_data *data)
{
    ds_seq_t *seq = ds_seq();

    php_unserialize_data_t unserialize_data = (php_unserialize_data_t) data;

    const unsigned char *pos = buffer;
    const unsigned char *end = buffer + length;

    PHP_VAR_UNSERIALIZE_INIT(unserialize_data);

    while (pos != end) {
        zval *value = var_tmp_var(&unserialize_data);

        if ( ! php_var_unserialize(value, &pos, end, &unserialize_data)) {
            goto error;
        }

        ds_seq_push(seq, value);
    }

    ZVAL_DS_SEQ(object, seq);
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    return SUCCESS;

error:
    ds_seq_free(seq);
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    UNSERIALIZE_ERROR();
    return FAILURE;
}
