#include "../../common.h"

#include "../../ds/ds_priority_queue.h"
#include "../objects/php_priority_queue.h"
#include "php_priority_queue_iterator.h"

static void php_ds_priority_queue_iterator_dtor(zend_object_iterator *iter)
{
    DTOR_AND_UNDEF(&iter->data);
}

static int php_ds_priority_queue_iterator_valid(zend_object_iterator *iter)
{
    return Z_ISUNDEF(iter->data) ? FAILURE : SUCCESS;
}

static zval *php_ds_priority_queue_iterator_get_current_data(zend_object_iterator *iter)
{
    return &iter->data;
}

static void php_ds_priority_queue_iterator_get_current_key(zend_object_iterator *iter, zval *key) {
    ZVAL_LONG(key, ((php_ds_priority_queue_iterator *) iter)->position);
}

static void php_ds_priority_queue_iterator_move_forward(zend_object_iterator *iter)
{
    php_ds_priority_queue_iterator *iterator = (php_ds_priority_queue_iterator *) iter;

    if ( ! DS_PRIORITY_QUEUE_IS_EMPTY(iterator->queue)) {
        ds_priority_queue_pop(iterator->queue, &iter->data);
        iterator->position++;
    } else {
        ZVAL_UNDEF(&iter->data);
    }
}

static void php_ds_priority_queue_iterator_rewind(zend_object_iterator *iter)
{
    php_ds_priority_queue_iterator *iterator = (php_ds_priority_queue_iterator *) iter;

    if ( ! DS_PRIORITY_QUEUE_IS_EMPTY(iterator->queue)) {
        ds_priority_queue_pop(iterator->queue, &iter->data);
    }

    iterator->position = 0;
}

static zend_object_iterator_funcs iterator_funcs = {
    php_ds_priority_queue_iterator_dtor,
    php_ds_priority_queue_iterator_valid,
    php_ds_priority_queue_iterator_get_current_data,
    php_ds_priority_queue_iterator_get_current_key,
    php_ds_priority_queue_iterator_move_forward,
    php_ds_priority_queue_iterator_rewind
};

zend_object_iterator *php_ds_priority_queue_get_iterator(zend_class_entry *ce, zval *object, int by_ref)
{
    php_ds_priority_queue_iterator *iterator;

    if (by_ref) {
        ITERATION_BY_REF_NOT_SUPPORTED();
        return NULL;
    }

    iterator = ecalloc(1, sizeof(php_ds_priority_queue_iterator));
    zend_iterator_init((zend_object_iterator*) iterator);

    ZVAL_UNDEF(&iterator->intern.data);

    iterator->intern.funcs = &iterator_funcs;
    iterator->queue        = Z_DS_PRIORITY_QUEUE_P(object);
    iterator->position     = 0;

    return (zend_object_iterator *) iterator;
}
