#include "../iterators/php_queue_iterator.h"
#include "../handlers/php_queue_handlers.h"
#include "../classes/php_queue_ce.h"

#include "php_queue.h"

zend_object *php_ds_queue_create_object_ex(ds_queue_t *queue)
{
    php_ds_queue_t *obj = ecalloc(1, sizeof(php_ds_queue_t));
    zend_object_std_init(&obj->std, php_ds_queue_ce);
    obj->std.handlers = &php_queue_handlers;
    obj->queue = queue;

    return &obj->std;
}

zend_object *php_ds_queue_create_object(zend_class_entry *ce)
{
    return php_ds_queue_create_object_ex(ds_queue());
}

zend_object *php_ds_queue_create_clone(ds_queue_t *queue)
{
    return php_ds_queue_create_object_ex(ds_queue_clone(queue));
}

int php_ds_queue_serialize(zval *object, unsigned char **buffer, size_t *length, zend_serialize_data *data)
{
    ds_queue_t *queue = Z_DS_QUEUE_P(object);

    php_serialize_data_t serialize_data = (php_serialize_data_t) data;
    PHP_VAR_SERIALIZE_INIT(serialize_data);

    if (QUEUE_SIZE(queue) == 0) {
        SERIALIZE_SET_ZSTR(ZSTR_EMPTY_ALLOC());

    } else {

        zval *value;
        smart_str buf = {0};

        DS_DEQUE_FOREACH(queue->deque, value) {
            php_var_serialize(&buf, value, &serialize_data);
        }
        DS_DEQUE_FOREACH_END();

        smart_str_0(&buf);
        SERIALIZE_SET_ZSTR(buf.s);
        zend_string_release(buf.s);
    }

    PHP_VAR_SERIALIZE_DESTROY(serialize_data);
    return SUCCESS;
}

int php_ds_queue_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t length, zend_unserialize_data *data)
{
    ds_queue_t *queue = ds_queue();

    php_unserialize_data_t unserialize_data = (php_unserialize_data_t) data;

    const unsigned char *pos = buffer;
    const unsigned char *end = buffer + length;

    PHP_VAR_UNSERIALIZE_INIT(unserialize_data);

    while (pos != end) {
        zval *value = var_tmp_var(&unserialize_data);

        if ( ! php_var_unserialize(value, &pos, end, &unserialize_data)) {
            goto error;
        }

        ds_queue_push_one(queue, value);
    }

    ZVAL_DS_QUEUE(object, queue);
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    return SUCCESS;

error:
    ds_queue_free(queue);
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    UNSERIALIZE_ERROR();
    return FAILURE;
}
