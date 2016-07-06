#include "../common.h"

#include "../php/iterators/php_queue_iterator.h"
#include "../php/handlers/php_queue_handlers.h"
#include "../php/classes/php_queue_ce.h"

#include "ds_deque.h"
#include "ds_queue.h"

ds_queue_t *ds_queue_ex(ds_deque_t *deque)
{
    ds_queue_t *queue = ecalloc(1, sizeof(ds_queue_t));
    queue->deque = deque;
    return queue;
}

ds_queue_t *ds_queue()
{
    return ds_queue_ex(ds_deque());
}

ds_queue_t *ds_queue_clone(ds_queue_t *queue)
{
    return ds_queue_ex(ds_deque_clone(queue->deque));
}

void ds_queue_user_allocate(ds_queue_t *queue, zend_long capacity)
{
    ds_deque_user_allocate(queue->deque, capacity);
}

zend_long ds_queue_capacity(ds_queue_t *queue)
{
    return queue->deque->capacity;
}

void ds_queue_push(ds_queue_t *queue, VA_PARAMS)
{
    ds_deque_push_va(queue->deque, argc, argv);
}

void ds_queue_push_one(ds_queue_t *queue, zval *value)
{
    ds_deque_push(queue->deque, value);
}

void ds_queue_clear(ds_queue_t *queue)
{
    ds_deque_clear(queue->deque);
}

void ds_queue_push_all(ds_queue_t *queue, zval *value)
{
    ds_deque_push_all(queue->deque, value);
}

void ds_queue_to_array(ds_queue_t *queue, zval *return_value)
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

void ds_queue_pop(ds_queue_t *queue, zval *return_value)
{
    ds_deque_shift(queue->deque, return_value);
}

void ds_queue_destroy(ds_queue_t *queue)
{
    ds_deque_destroy(queue->deque);
}

zval *ds_queue_peek(ds_queue_t *queue)
{
    return ds_deque_get_first(queue->deque);
}
