#include "php.h"
#include "../common.h"
#include "../internal/php_vector.h"
#include "php_vector_iterator.h"

static void iterator_dtor(zend_object_iterator *i)
{

}

static int iterator_valid(zend_object_iterator *i)
{
    VectorIterator *iterator = (VectorIterator *) i;
    return iterator->position < iterator->vector->size ? SUCCESS : FAILURE;
}

static zval *iterator_get_current_data(zend_object_iterator *i)
{
    VectorIterator *iterator = (VectorIterator *) i;
    return &iterator->vector->buffer[iterator->position];
}

static void iterator_get_current_key(zend_object_iterator *i, zval *key)
{
    VectorIterator *iterator = (VectorIterator *) i;
    ZVAL_LONG(key, iterator->position);
}

static void iterator_move_forward(zend_object_iterator *i)
{
    VectorIterator *iterator = (VectorIterator *) i;
    iterator->position++;
}

static void iterator_rewind(zend_object_iterator *i)
{
    VectorIterator *iterator = (VectorIterator *) i;
    iterator->position = 0;
}

static zval *iterator_reversed_get_current_data(zend_object_iterator *i)
{
    VectorIterator *iterator = (VectorIterator *) i;
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

static zend_object_iterator *vector_create_iterator(
    Vector *vector,
    zend_object_iterator_funcs *funcs,
    int by_ref
) {

    VectorIterator *iterator;

    if (by_ref) {
        ITERATION_BY_REF_NOT_SUPPORTED();
        return NULL;
    }

    iterator = ecalloc(1, sizeof(VectorIterator));

    zend_iterator_init((zend_object_iterator*) iterator);

    iterator->intern.funcs  = funcs;
    iterator->position      = 0;
    iterator->vector        = vector;

    return (zend_object_iterator *) iterator;
}

zend_object_iterator *vector_get_iterator_ex(zend_class_entry *ce, zval *obj, int by_ref, Vector *vector)
{
    return vector_create_iterator(vector, &iterator_funcs, by_ref);
}

zend_object_iterator *vector_get_iterator(zend_class_entry *ce, zval *obj, int by_ref)
{
    Vector *vector = Z_VECTOR_P(obj);
    return vector_create_iterator(vector, &iterator_funcs, by_ref);
}
