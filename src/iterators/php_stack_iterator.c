#include "php.h"
#include "zend_exceptions.h"

#include "../common.h"
#include "../internal/php_stack.h"
#include "php_stack_iterator.h"

static void php_ds_stack_iterator_dtor(zend_object_iterator *iter)
{
    DTOR_AND_UNDEF(&iter->data);
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

static void php_ds_stack_iterator_move_forward(zend_object_iterator *iter)
{
    php_ds_stack_iterator_t *iterator = (php_ds_stack_iterator_t *) iter;

    if ( ! DS_STACK_IS_EMPTY(iterator->stack)) {
        php_ds_stack_pop(iterator->stack, &iter->data);
        iterator->position++;
    } else {
        ZVAL_UNDEF(&iter->data);
    }
}

static void php_ds_stack_iterator_rewind(zend_object_iterator *iter)
{
    php_ds_stack_iterator_t *iterator = (php_ds_stack_iterator_t *) iter;

    if ( ! DS_STACK_IS_EMPTY(iterator->stack)) {
        php_ds_stack_pop(iterator->stack, &iter->data);
    }

    iterator->position = 0;
}

static zend_object_iterator_funcs iterator_funcs = {
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

    ZVAL_UNDEF(&iterator->intern.data);

    iterator->intern.funcs = &iterator_funcs;
    iterator->stack        = Z_DS_STACK_P(object);
    iterator->position     = 0;

    return (zend_object_iterator *) iterator;
}
