#include "../common.h"

#include "../php/iterators/php_queue_iterator.h"
#include "../php/handlers/php_queue_handlers.h"
#include "../php/classes/php_queue_ce.h"

#include "ds_deque.h"
#include "ds_queue.h"

static Queue *queue_init_ex(ds_deque_t *deque)
{
    Queue *queue = ecalloc(1, sizeof(Queue));
    zend_object_std_init(&queue->std, queue_ce);
    queue->std.handlers = &queue_handlers;
    queue->deque = deque;
    return queue;
}

Queue *queue_init()
{
    return queue_init_ex(ds_deque());
}

zend_object *queue_create_object(zend_class_entry *ce)
{
    return &queue_init()->std;
}

zend_object *queue_create_clone(Queue *queue)
{
    ds_deque_t *copy = ds_deque_create_copy(queue->deque);
    return &queue_init_ex(copy)->std;
}

void queue_user_allocate(Queue *queue, zend_long capacity)
{
    ds_deque_user_allocate(queue->deque, capacity);
}

zend_long queue_capacity(Queue *queue)
{
    return queue->deque->capacity;
}

void queue_push(Queue *queue, VA_PARAMS)
{
    ds_deque_push_va(queue->deque, argc, argv);
}

void queue_push_one(Queue *queue, zval *value)
{
    ds_deque_push(queue->deque, value);
}

void queue_clear(Queue *queue)
{
    ds_deque_clear(queue->deque);
}

void queue_push_all(Queue *queue, zval *value)
{
    ds_deque_push_all(queue->deque, value);
}

void queue_to_array(Queue *queue, zval *return_value)
{
    zend_ulong size = QUEUE_SIZE(queue);

    if (size == 0) {
        array_init(return_value);

    } else {
        zval *value;
        array_init_size(return_value, size);

        DS_DEQUE_FOREACH(queue->deque, value) {
            add_next_index_zval(return_value, value);
            Z_TRY_ADDREF_P(value);
        }
        DS_DEQUE_FOREACH_END();
    }
}

void queue_pop(Queue *queue, zval *return_value)
{
    ds_deque_shift(queue->deque, return_value);
}

void queue_destroy(Queue *queue)
{
    ds_deque_destroy(queue->deque);
}

zval *queue_peek(Queue *queue)
{
    return ds_deque_get_first(queue->deque);
}

int queue_serialize(zval *object, unsigned char **buffer, size_t *length, zend_serialize_data *data)
{
    Queue *queue = Z_QUEUE_P(object);

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

int queue_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t length, zend_unserialize_data *data)
{
    Queue *queue = queue_init();

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

        queue_push_one(queue, value);
    }

    if (*(++pos) != '\0') {
        goto error;
    }

    ZVAL_OBJ(object, &queue->std);
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    return SUCCESS;

error:
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    UNSERIALIZE_ERROR();
    return FAILURE;
}
