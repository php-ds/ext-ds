#include "../../common.h"

#include "../../ds/ds_heap.h"
#include "../objects/php_heap.h"
#include "php_heap_iterator.h"

static void php_ds_heap_iterator_dtor(zend_object_iterator *iter)
{
    php_ds_heap_iterator_t *iterator = (php_ds_heap_iterator_t *) iter;

    if (iterator->heap) {
        ds_heap_free(iterator->heap);
    }
    DTOR_AND_UNDEF(&iterator->intern.data);
    ds_heap_release(iterator->snapshot);
    OBJ_RELEASE(iterator->object);
}

static int php_ds_heap_iterator_valid(zend_object_iterator *iter)
{
    return Z_ISUNDEF(iter->data) ? FAILURE : SUCCESS;
}

static zval *php_ds_heap_iterator_get_current_data(zend_object_iterator *iter)
{
    return Z_ISUNDEF(iter->data) ? NULL : &iter->data;
}

static void php_ds_heap_iterator_get_current_key(zend_object_iterator *iter, zval *key)
{
    if (Z_ISUNDEF(iter->data)) {
        ZVAL_NULL(key);
    } else {
        ZVAL_LONG(key, ((php_ds_heap_iterator_t *) iter)->position);
    }
}

static void php_ds_heap_iterator_set_current(php_ds_heap_iterator_t *iterator)
{
    DTOR_AND_UNDEF(&iterator->intern.data);

    if (!DS_HEAP_IS_EMPTY(iterator->heap)) {
        ds_heap_pop(iterator->heap, &iterator->intern.data, iterator->compare);
    }
}

static void php_ds_heap_iterator_move_forward(zend_object_iterator *iter)
{
    php_ds_heap_iterator_t *iterator = (php_ds_heap_iterator_t *) iter;

    if (iterator->running) {
        ds_throw_exception(spl_ce_RuntimeException,
            "Cannot modify a heap iterator while it is executing");
        return;
    }

    iterator->running = true;
    php_ds_heap_iterator_set_current(iterator);
    iterator->position++;
    iterator->running = false;
}

static void php_ds_heap_iterator_rewind(zend_object_iterator *iter)
{
    php_ds_heap_iterator_t *iterator = (php_ds_heap_iterator_t *) iter;

    if (iterator->running) {
        ds_throw_exception(spl_ce_RuntimeException,
            "Cannot modify a heap iterator while it is executing");
        return;
    }

    iterator->running = true;
    if (iterator->heap) {
        ds_heap_free(iterator->heap);
    }
    iterator->heap = ds_heap_clone(iterator->snapshot);
    php_ds_heap_iterator_set_current(iterator);
    iterator->position = 0;
    iterator->running = false;
}

static zend_object_iterator_funcs iterator_funcs = {
    php_ds_heap_iterator_dtor,
    php_ds_heap_iterator_valid,
    php_ds_heap_iterator_get_current_data,
    php_ds_heap_iterator_get_current_key,
    php_ds_heap_iterator_move_forward,
    php_ds_heap_iterator_rewind
};

zend_object_iterator *php_ds_heap_get_iterator(zend_class_entry *ce, zval *object, int by_ref)
{
    php_ds_heap_iterator_t *iterator;
    php_ds_heap_t *obj;

    if (by_ref) {
        ITERATION_BY_REF_NOT_SUPPORTED();
        return NULL;
    }

    obj = Z_DS_HEAP_P(object);

    iterator = ecalloc(1, sizeof(php_ds_heap_iterator_t));
    zend_iterator_init((zend_object_iterator *) iterator);

    ZVAL_UNDEF(&iterator->intern.data);

    iterator->intern.funcs = &iterator_funcs;
    iterator->snapshot     = obj->heap;
    iterator->snapshot->refs++;
    iterator->compare      = php_ds_heap_get_compare(obj);
    iterator->object       = Z_OBJ_P(object);
    iterator->position     = 0;

    GC_ADDREF(iterator->object);

    return (zend_object_iterator *) iterator;
}
