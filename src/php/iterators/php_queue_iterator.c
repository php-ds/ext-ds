#include "../../common.h"

// #include "../../ds/ds_queue.h"
#include "../objects/php_queue.h"

#include "php_queue_iterator.h"
// #include "php_htable_iterator.h"

static void ds_queue_iterator_dtor(zend_object_iterator *iter)
{
    DTOR_AND_UNDEF(&iter->data);
}

static int ds_queue_iterator_valid(zend_object_iterator *iter)
{
    return Z_ISUNDEF(iter->data) ? FAILURE : SUCCESS;
}

static zval *ds_queue_iterator_get_current_data(zend_object_iterator *iter)
{
    return &iter->data;
}

static void ds_queue_iterator_get_current_key(zend_object_iterator *iter, zval *key) {
    ZVAL_LONG(key, ((ds_queue_tIterator *) iter)->position);
}

static void ds_queue_iterator_move_forward(zend_object_iterator *iter)
{
    ds_queue_tIterator *iterator = (ds_queue_tIterator *) iter;

    if ( ! QUEUE_IS_EMPTY(iterator->queue)) {
        ds_queue_pop(iterator->queue, &iter->data);
        iterator->position++;
    } else {
        ZVAL_UNDEF(&iter->data);
    }
}

static void ds_queue_iterator_rewind(zend_object_iterator *iter)
{
    ds_queue_tIterator *iterator = (ds_queue_tIterator *) iter;

    if ( ! QUEUE_IS_EMPTY(iterator->queue)) {
        ds_queue_pop(iterator->queue, &iter->data);
    }

    iterator->position = 0;
}

static zend_object_iterator_funcs iterator_funcs = {
    ds_queue_iterator_dtor,
    ds_queue_iterator_valid,
    ds_queue_iterator_get_current_data,
    ds_queue_iterator_get_current_key,
    ds_queue_iterator_move_forward,
    ds_queue_iterator_rewind
};

zend_object_iterator *php_ds_queue_get_iterator(zend_class_entry *ce, zval *object, int by_ref)
{
    ds_queue_tIterator *iterator;

    if (by_ref) {
        ITERATION_BY_REF_NOT_SUPPORTED();
        return NULL;
    }

    iterator = ecalloc(1, sizeof(ds_queue_tIterator));
    zend_iterator_init((zend_object_iterator*) iterator);

    ZVAL_UNDEF(&iterator->intern.data);

    iterator->intern.funcs = &iterator_funcs;
    iterator->queue        = Z_DS_QUEUE_P(object);
    iterator->position     = 0;

    return (zend_object_iterator *) iterator;
}
