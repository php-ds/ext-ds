#include "../../common.h"

#include "../../ds/ds_vector.h"
#include "../objects/php_vector.h"
#include "php_vector_iterator.h"

static void php_ds_vector_iterator_dtor(zend_object_iterator *iter)
{
    php_ds_vector_iterator_t *iterator = (php_ds_vector_iterator_t *) iter;

    OBJ_RELEASE((zend_object*) iterator->vector_obj);
}

static int php_ds_vector_iterator_valid(zend_object_iterator *iter)
{
    php_ds_vector_iterator_t *iterator = (php_ds_vector_iterator_t *) iter;

    return iterator->position < iterator->vector_obj->vector->size ? SUCCESS : FAILURE;
}

static zval *php_ds_vector_iterator_get_current_data(zend_object_iterator *iter)
{
    php_ds_vector_iterator_t *iterator = (php_ds_vector_iterator_t *) iter;

    return &iterator->vector_obj->vector->buffer[iterator->position];
}

static void php_ds_vector_iterator_get_current_key(zend_object_iterator *iter, zval *key)
{
    ZVAL_LONG(key, ((php_ds_vector_iterator_t *) iter)->position);
}

static void php_ds_vector_iterator_move_forward(zend_object_iterator *iter)
{
    ((php_ds_vector_iterator_t *) iter)->position++;
}

static void php_ds_vector_iterator_rewind(zend_object_iterator *iter)
{
    ((php_ds_vector_iterator_t *) iter)->position = 0;
}

static zend_object_iterator_funcs php_ds_vector_iterator_funcs = {
    php_ds_vector_iterator_dtor,
    php_ds_vector_iterator_valid,
    php_ds_vector_iterator_get_current_data,
    php_ds_vector_iterator_get_current_key,
    php_ds_vector_iterator_move_forward,
    php_ds_vector_iterator_rewind
};

static zend_object_iterator *php_ds_vector_create_iterator(zval *obj, int by_ref)
{
    php_ds_vector_iterator_t *iterator;

    if (by_ref) {
        ITERATION_BY_REF_NOT_SUPPORTED();
        return NULL;
    }

    iterator = ecalloc(1, sizeof(php_ds_vector_iterator_t));

    zend_iterator_init((zend_object_iterator*) iterator);

    iterator->intern.funcs  = &php_ds_vector_iterator_funcs;
    iterator->vector_obj    = (php_ds_vector_t*)(Z_OBJ_P(obj));
    iterator->position      = 0;

    // Add a reference to the object so that it doesn't get collected when
    // the iterated object is implict, eg. foreach ($obj->getInstance() as $value){ ... }
    ++GC_REFCOUNT((zend_object*) iterator->vector_obj);

    return (zend_object_iterator *) iterator;
}

zend_object_iterator *php_ds_vector_get_iterator(zend_class_entry *ce, zval *obj, int by_ref)
{
    return php_ds_vector_create_iterator(obj, by_ref);
}
