#include "php.h"
#include "zend_exceptions.h"

#include "../common.h"
#include "../internal/php_queue.h"
#include "php_queue_iterator.h"

static void queue_iterator_dtor(zend_object_iterator *iter)
{
    DTOR_AND_UNDEF(&iter->data);
}

static int queue_iterator_valid(zend_object_iterator *iter)
{
    return Z_ISUNDEF(iter->data) ? FAILURE : SUCCESS;
}

static zval *queue_iterator_get_current_data(zend_object_iterator *iter)
{
    return &iter->data;
}

static void queue_iterator_get_current_key(zend_object_iterator *iter, zval *key) {
    ZVAL_LONG(key, ((QueueIterator *) iter)->position);
}

static void queue_iterator_move_forward(zend_object_iterator *iter)
{
    QueueIterator *iterator = (QueueIterator *) iter;

    if ( ! QUEUE_IS_EMPTY(iterator->queue)) {
        queue_pop(iterator->queue, &iter->data);
        iterator->position++;
    } else {
        ZVAL_UNDEF(&iter->data);
    }
}

static void queue_iterator_rewind(zend_object_iterator *iter)
{
    QueueIterator *iterator = (QueueIterator *) iter;

    if ( ! QUEUE_IS_EMPTY(iterator->queue)) {
        queue_pop(iterator->queue, &iter->data);
    }

    iterator->position = 0;
}

static zend_object_iterator_funcs iterator_funcs = {
    queue_iterator_dtor,
    queue_iterator_valid,
    queue_iterator_get_current_data,
    queue_iterator_get_current_key,
    queue_iterator_move_forward,
    queue_iterator_rewind
};

zend_object_iterator *queue_get_iterator(zend_class_entry *ce, zval *object, int by_ref)
{
    QueueIterator *iterator;

    if (by_ref) {
        ITERATION_BY_REF_NOT_SUPPORTED();
        return NULL;
    }

    iterator = ecalloc(1, sizeof(QueueIterator));
    zend_iterator_init((zend_object_iterator*) iterator);

    ZVAL_UNDEF(&iterator->intern.data);

    iterator->intern.funcs = &iterator_funcs;
    iterator->queue        = Z_QUEUE_P(object);
    iterator->position     = 0;

    return (zend_object_iterator *) iterator;
}
