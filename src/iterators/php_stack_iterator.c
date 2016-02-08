#include "php.h"
#include "zend_exceptions.h"

#include "../common.h"
#include "../internal/php_stack.h"
#include "php_stack_iterator.h"

static void stack_iterator_dtor(zend_object_iterator *iter)
{
    DTOR_AND_UNDEF(&iter->data);
}

static int stack_iterator_valid(zend_object_iterator *iter)
{
    return Z_ISUNDEF(iter->data) ? FAILURE : SUCCESS;
}

static zval *stack_iterator_get_current_data(zend_object_iterator *iter)
{
    return &iter->data;
}

static void stack_iterator_get_current_key(zend_object_iterator *iter, zval *key) {
    ZVAL_LONG(key, ((StackIterator *) iter)->position);
}

static void stack_iterator_move_forward(zend_object_iterator *iter)
{
    StackIterator *iterator = (StackIterator *) iter;

    if ( ! STACK_IS_EMPTY(iterator->stack)) {
        stack_pop(iterator->stack, &iter->data);
        iterator->position++;
    } else {
        ZVAL_UNDEF(&iter->data);
    }
}

static void stack_iterator_rewind(zend_object_iterator *iter)
{
    StackIterator *iterator = (StackIterator *) iter;

    if ( ! STACK_IS_EMPTY(iterator->stack)) {
        stack_pop(iterator->stack, &iter->data);
    }

    iterator->position = 0;
}

static zend_object_iterator_funcs iterator_funcs = {
    stack_iterator_dtor,
    stack_iterator_valid,
    stack_iterator_get_current_data,
    stack_iterator_get_current_key,
    stack_iterator_move_forward,
    stack_iterator_rewind
};

zend_object_iterator *stack_get_iterator(zend_class_entry *ce, zval *object, int by_ref)
{
    StackIterator *iterator;

    if (by_ref) {
        ITERATION_BY_REF_NOT_SUPPORTED();
        return NULL;
    }

    iterator = ecalloc(1, sizeof(StackIterator));
    zend_iterator_init((zend_object_iterator*) iterator);

    ZVAL_UNDEF(&iterator->intern.data);

    iterator->intern.funcs = &iterator_funcs;
    iterator->stack        = Z_STACK_P(object);
    iterator->position     = 0;

    return (zend_object_iterator *) iterator;
}
