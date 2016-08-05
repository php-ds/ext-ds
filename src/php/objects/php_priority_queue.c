#include "../iterators/php_priority_queue_iterator.h"
#include "../handlers/php_priority_queue_handlers.h"
#include "../classes/php_priority_queue_ce.h"

#include "php_priority_queue.h"

zend_object *php_ds_priority_queue_create_object_ex(ds_priority_queue_t *queue)
{
    php_ds_priority_queue_t *obj = ecalloc(1, sizeof(php_ds_priority_queue_t));
    zend_object_std_init(&obj->std, php_ds_priority_queue_ce);
    obj->std.handlers = &php_priority_queue_handlers;

    obj->queue   = queue;
    obj->gc_data = NULL;
    obj->gc_size = 0;

    return &obj->std;
}

zend_object *php_ds_priority_queue_create_object(zend_class_entry *ce)
{
    return php_ds_priority_queue_create_object_ex(ds_priority_queue());
}


zend_object *php_ds_priority_queue_create_clone(ds_priority_queue_t *queue)
{
    return php_ds_priority_queue_create_object_ex(ds_priority_queue_clone(queue));
}


int php_ds_priority_queue_serialize(zval *object, unsigned char **buffer, size_t *length, zend_serialize_data *data)
{
    ds_priority_queue_t *queue = Z_DS_PRIORITY_QUEUE_P(object);

    php_serialize_data_t serialize_data = (php_serialize_data_t) data;
    PHP_VAR_SERIALIZE_INIT(serialize_data);

    if (queue->size == 0) {
        SERIALIZE_SET_ZSTR(ZSTR_EMPTY_ALLOC());

    } else {
        ds_priority_queue_node_t *nodes = ds_priority_queue_create_sorted_buffer(queue);
        ds_priority_queue_node_t *pos   = nodes;
        ds_priority_queue_node_t *end   = nodes + queue->size;

        smart_str buf = {0};

        for (; pos < end; ++pos) {

            zval priority;
            ZVAL_LONG(&priority, pos->priority);

            php_var_serialize(&buf, &pos->value, &serialize_data);
            php_var_serialize(&buf, &priority, &serialize_data);
        }

        smart_str_0(&buf);
        SERIALIZE_SET_ZSTR(buf.s);
        zend_string_release(buf.s);

        efree(nodes);
    }

    PHP_VAR_SERIALIZE_DESTROY(serialize_data);
    return SUCCESS;
}

int php_ds_priority_queue_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t length, zend_unserialize_data *data)
{
    ds_priority_queue_t *queue = ds_priority_queue();

    php_unserialize_data_t unserialize_data = (php_unserialize_data_t) data;

    const unsigned char *pos = buffer;
    const unsigned char *max = buffer + length;

    PHP_VAR_UNSERIALIZE_INIT(unserialize_data);
    ZVAL_DS_PRIORITY_QUEUE(object, queue);

    while (*pos != '}') {
        zval *value, *priority;

        value = var_tmp_var(&unserialize_data);
        if ( ! php_var_unserialize(value, &pos, max, &unserialize_data)) {
            goto error;
        }

        priority = var_tmp_var(&unserialize_data);
        if ( ! php_var_unserialize(priority, &pos, max, &unserialize_data)) {
            goto error;
        }

        if (Z_TYPE_P(priority) != IS_LONG) {
            goto error;
        }

        ds_priority_queue_push(queue, value, Z_LVAL_P(priority));
    }

    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    return SUCCESS;

error:
    ds_priority_queue_free(queue);
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    UNSERIALIZE_ERROR();
    return FAILURE;
}
