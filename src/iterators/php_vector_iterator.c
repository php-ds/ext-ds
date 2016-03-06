#include "php.h"
#include "../common.h"
#include "../internal/php_vector.h"
#include "php_vector_iterator.h"

static void iterator_dtor(zend_object_iterator *i)
{

}

static int iterator_valid(zend_object_iterator *i)
{
    php_ds_vector_iterator_t *iterator = (php_ds_vector_iterator_t *) i;
    return iterator->position < iterator->vector->size ? SUCCESS : FAILURE;
}

static zval *iterator_get_current_data(zend_object_iterator *i)
{
    php_ds_vector_iterator_t *iterator = (php_ds_vector_iterator_t *) i;
    return &iterator->vector->buffer[iterator->position];
}

static void iterator_get_current_key(zend_object_iterator *i, zval *key)
{
    php_ds_vector_iterator_t *iterator = (php_ds_vector_iterator_t *) i;
    ZVAL_LONG(key, iterator->position);
}

static void iterator_move_forward(zend_object_iterator *i)
{
    php_ds_vector_iterator_t *iterator = (php_ds_vector_iterator_t *) i;
    iterator->position++;
}

static void iterator_rewind(zend_object_iterator *i)
{
    php_ds_vector_iterator_t *iterator = (php_ds_vector_iterator_t *) i;
    iterator->position = 0;
}

static zval *iterator_reversed_get_current_data(zend_object_iterator *i)
{
    php_ds_vector_iterator_t *iterator = (php_ds_vector_iterator_t *) i;
    return &iterator->vector->buffer[iterator->vector->size - iterator->position - 1];
}

static zend_object_iterator_funcs iterator_funcs = {
    iterator_dtor,
    iterator_valid,
    iterator_get_current_data,
    iterator_get_current_key,
    iterator_move_forward,
    iterator_rewind
};

static zend_object_iterator *ds_vector_create_iterator(
    ds_vector_t *vector,
    zend_object_iterator_funcs *funcs,
    int by_ref
) {

    php_ds_vector_iterator_t *iterator;

    if (by_ref) {
        ITERATION_BY_REF_NOT_SUPPORTED();
        return NULL;
    }

    iterator = ecalloc(1, sizeof(php_ds_vector_iterator_t));

    zend_iterator_init((zend_object_iterator*) iterator);

    iterator->intern.funcs  = funcs;
    iterator->position      = 0;
    iterator->vector        = vector;

    return (zend_object_iterator *) iterator;
}

zend_object_iterator *php_ds_vector_get_iterator_ex(zend_class_entry *ce, zval *obj, int by_ref, ds_vector_t *vector)
{
    return ds_vector_create_iterator(vector, &iterator_funcs, by_ref);
}

zend_object_iterator *php_ds_vector_get_iterator(zend_class_entry *ce, zval *obj, int by_ref)
{
    ds_vector_t *vector = Z_DS_VECTOR_P(obj);
    return ds_vector_create_iterator(vector, &iterator_funcs, by_ref);
}
