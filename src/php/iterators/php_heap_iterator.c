#include "../../common.h"

#include "../../ds/ds_heap.h"
#include "../objects/php_heap.h"
#include "php_heap_iterator.h"

static void php_ds_heap_iterator_dtor(zend_object_iterator *iter)
{
    php_ds_heap_iterator_t *iterator = (php_ds_heap_iterator_t *) iter;

    ds_heap_free(iterator->heap);
    DTOR_AND_UNDEF(&iterator->intern.data);
    OBJ_RELEASE(iterator->object);
}

static int php_ds_heap_iterator_valid(zend_object_iterator *iter)
{
    return Z_ISUNDEF(iter->data) ? FAILURE : SUCCESS;
}

static zval *php_ds_heap_iterator_get_current_data(zend_object_iterator *iter)
{
    return &iter->data;
}

static void php_ds_heap_iterator_get_current_key(zend_object_iterator *iter, zval *key)
{
    ZVAL_LONG(key, ((php_ds_heap_iterator_t *) iter)->position);
}

static void php_ds_heap_iterator_set_current(php_ds_heap_iterator_t *iterator)
{
    if (DS_HEAP_IS_EMPTY(iterator->heap)) {
        ZVAL_UNDEF(&iterator->intern.data);
    } else {
        ds_heap_pop(iterator->heap, &iterator->intern.data, iterator->compare);
        Z_TRY_DELREF_P(&iterator->intern.data);
    }
}

static void php_ds_heap_iterator_move_forward(zend_object_iterator *iter)
{
    php_ds_heap_iterator_t *iterator = (php_ds_heap_iterator_t *) iter;
    php_ds_heap_iterator_set_current(iterator);
    iterator->position++;
}

static void php_ds_heap_iterator_rewind(zend_object_iterator *iter)
{
    php_ds_heap_iterator_t *iterator = (php_ds_heap_iterator_t *) iter;
    php_ds_heap_iterator_set_current(iterator);
    iterator->position = 0;
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
    iterator->heap         = ds_heap_clone(obj->heap);
    iterator->compare      = php_ds_heap_get_compare(obj);
    iterator->object       = Z_OBJ_P(object);
    iterator->position     = 0;

    GC_ADDREF(iterator->object);

    return (zend_object_iterator *) iterator;
}
