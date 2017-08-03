#ifndef DS_DEQUE_CE_H
#define DS_DEQUE_CE_H

#include "php.h"
#include "../objects/php_sequence.h"

#define THIS_DS_DEQUE() Z_DS_DEQUE_P(getThis())

#define RETURN_DS_SEQUENCE(d)                  \
do {                                        \
    ds_deque_t *_d = d;                     \
    if (_d) {                               \
        ZVAL_DS_SEQUENCE(return_value, _d);    \
    } else {                                \
        ZVAL_NULL(return_value);            \
    }                                       \
    return;                                 \
} while(0)

extern zend_class_entry *php_ds_sequence_ce;


ARGINFO_OPTIONAL_ZVAL(                  Sequence___construct, values);
ARGINFO_LONG(                           Sequence_allocate, capacity);
ARGINFO_CALLABLE(                       Sequence_apply, callback);
ARGINFO_NONE_RETURN_LONG(               Sequence_capacity);
ARGINFO_VARIADIC_ZVAL_RETURN_BOOL(      Sequence_contains, values);
ARGINFO_CALLABLE_RETURN_BOOL(           Sequence_each, callback);
ARGINFO_OPTIONAL_CALLABLE_RETURN_DS(    Sequence_filter, callback, Sequence);
ARGINFO_ZVAL(                           Sequence_find, value);
ARGINFO_NONE(                           Sequence_first);
ARGINFO_OPTIONAL_STRING_RETURN_STRING(  Sequence_join, glue);
ARGINFO_LONG(                           Sequence_get, index);
ARGINFO_ZVAL_RETURN_DS(                 Sequence_groupBy, iteratee, Map);
ARGINFO_LONG_VARIADIC_ZVAL(             Sequence_insert, index, values);
ARGINFO_NONE(                           Sequence_last);
ARGINFO_CALLABLE_RETURN_DS(             Sequence_map, callback, Sequence);
ARGINFO_ZVAL_RETURN_DS(                 Sequence_merge, values, Sequence);
ARGINFO_OPTIONAL_CALLABLE_RETURN_LONG(  Sequence_partition, predicate);
ARGINFO_ZVAL_RETURN_DS(                 Sequence_pluck, key, Sequence);
ARGINFO_NONE(                           Sequence_pop);
ARGINFO_VARIADIC_ZVAL(                  Sequence_push, values);
ARGINFO_CALLABLE_OPTIONAL_ZVAL(         Sequence_reduce, callback, initial);
ARGINFO_LONG(                           Sequence_remove, index);
ARGINFO_NONE(                           Sequence_reverse);
ARGINFO_NONE_RETURN_DS(                 Sequence_reversed, Sequence);
ARGINFO_LONG(                           Sequence_rotate, rotations);
ARGINFO_LONG_ZVAL(                      Sequence_set, index, value);
ARGINFO_NONE(                           Sequence_shift);
ARGINFO_LONG_OPTIONAL_LONG_RETURN_DS(   Sequence_slice, index, length, Sequence);
ARGINFO_OPTIONAL_CALLABLE(              Sequence_sort, comparator);
ARGINFO_OPTIONAL_CALLABLE_RETURN_DS(    Sequence_sorted, comparator, Sequence);
ARGINFO_NONE(                           Sequence_sum);
ARGINFO_VARIADIC_ZVAL(                  Sequence_unshift, values);

void php_ds_register_sequence();

#endif
