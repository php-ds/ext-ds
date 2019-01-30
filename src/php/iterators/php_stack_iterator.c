#include "../../common.h"
#include "../objects/php_stack.h"
#include "php_stack_iterator.h"

static void php_ds_stack_iterator_dtor(zend_object_iterator *iter)
{
    PHP_DS_DECR_ITERATOR((php_ds_stack_t *) (&iter->std));
}

static int php_ds_stack_iterator_valid(zend_object_iterator *iter)
{
    return Z_ISUNDEF(iter->data) ? FAILURE : SUCCESS;
}

static zval *php_ds_stack_iterator_get_current_data(zend_object_iterator *iter)
{
    return &iter->data;
}

static void php_ds_stack_iterator_get_current_key(zend_object_iterator *iter, zval *key) {
    ZVAL_LONG(key, ((php_ds_stack_iterator_t *) iter)->position);
}

static void php_ds_stack_iterator_set_current(ds_stack_t *stack, zval *data)
{
    if (DS_STACK_IS_EMPTY(stack)) {
        ZVAL_UNDEF(data);
    } else {
        ds_stack_pop(stack, data);
        Z_TRY_DELREF_P(data);
    }
}

static void php_ds_stack_iterator_move_forward(zend_object_iterator *iter)
{
    php_ds_stack_iterator_t *iterator = (php_ds_stack_iterator_t *) iter;
    php_ds_stack_iterator_set_current(iterator->stack, &iter->data);
    iterator->position++;
}

static void php_ds_stack_iterator_rewind(zend_object_iterator *iter)
{
    php_ds_stack_iterator_t *iterator = (php_ds_stack_iterator_t *) iter;
    php_ds_stack_iterator_set_current(iterator->stack, &iter->data);
    iterator->position = 0;
}

static zend_object_iterator_funcs php_ds_stack_iterator_funcs = {
    php_ds_stack_iterator_dtor,
    php_ds_stack_iterator_valid,
    php_ds_stack_iterator_get_current_data,
    php_ds_stack_iterator_get_current_key,
    php_ds_stack_iterator_move_forward,
    php_ds_stack_iterator_rewind
};

zend_object_iterator *php_ds_stack_get_iterator(zend_class_entry *ce, zval *object, int by_ref)
{
    php_ds_stack_iterator_t *iterator;

    if (by_ref) {
        ITERATION_BY_REF_NOT_SUPPORTED();
        return NULL;
    }


    iterator = ecalloc(1, sizeof(php_ds_stack_iterator_t));
    zend_iterator_init((zend_object_iterator*) iterator);

    iterator->intern.funcs = &php_ds_stack_iterator_funcs;
    iterator->stack        = Z_DS_STACK_P(object);
    iterator->position     = 0;

    PHP_DS_INCR_ITERATOR(Z_DS_STACK_OBJ_P(object));

    return (zend_object_iterator *) iterator;
}
