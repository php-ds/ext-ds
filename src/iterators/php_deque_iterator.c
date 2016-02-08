#include "php.h"
#include "../common.h"
#include "../internal/php_deque.h"
#include "php_deque_iterator.h"

static void deque_iterator_dtor(zend_object_iterator *intern)
{

}

static int deque_iterator_valid(zend_object_iterator *iter)
{
    DequeIterator *iterator = (DequeIterator *) iter;
    Deque *deque             = iterator->deque;
    zend_long position       = iterator->position;

    return position < DEQUE_SIZE(deque) ? SUCCESS : FAILURE;
}

static zval *deque_iterator_get_current_data(zend_object_iterator *iter)
{
    DequeIterator *iterator = (DequeIterator *) iter;
    Deque *deque            = iterator->deque;
    zend_long position      = iterator->position;

    return deque_get(deque, position);
}

static void deque_iterator_get_current_key(zend_object_iterator *iter, zval *key) {
    ZVAL_LONG(key, ((DequeIterator *) iter)->position);
}

static void deque_iterator_move_forward(zend_object_iterator *iter)
{
    ((DequeIterator *) iter)->position++;
}

static void deque_iterator_rewind(zend_object_iterator *iter)
{
    ((DequeIterator *) iter)->position = 0;
}

static zend_object_iterator_funcs iterator_funcs = {
    deque_iterator_dtor,
    deque_iterator_valid,
    deque_iterator_get_current_data,
    deque_iterator_get_current_key,
    deque_iterator_move_forward,
    deque_iterator_rewind
};

static zend_object_iterator *create_iterator(Deque *deque, int by_ref)
{
    DequeIterator *iterator;

    if (by_ref) {
        ITERATION_BY_REF_NOT_SUPPORTED();
        return NULL;
    }

    iterator = ecalloc(1, sizeof(DequeIterator));
    zend_iterator_init((zend_object_iterator*) iterator);

    iterator->intern.funcs  = &iterator_funcs;
    iterator->deque         = deque;
    iterator->position      = 0;

    return (zend_object_iterator *) iterator;
}

zend_object_iterator *deque_get_iterator_ex(zend_class_entry *ce, zval *object, int by_ref, Deque *deque)
{
    return create_iterator(deque, by_ref);
}

zend_object_iterator *deque_get_iterator(zend_class_entry *ce, zval *object, int by_ref)
{
    return create_iterator(Z_DEQUE_P(object), by_ref);
}
