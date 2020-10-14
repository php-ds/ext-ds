#ifndef PHP_DS_SEQUENCE_CE_H
#define PHP_DS_SEQUENCE_CE_H

#include "php.h"

extern zend_class_entry *sequence_ce;

#define PHP_DS_SEQUENCE_ME(cls, name) \
    PHP_ME(cls, name, arginfo_Sequence_##name, ZEND_ACC_PUBLIC)

#define PHP_DS_SEQUENCE_ME_LIST(cls) \
PHP_DS_SEQUENCE_ME(cls, allocate) \
PHP_DS_SEQUENCE_ME(cls, apply) \
PHP_DS_SEQUENCE_ME(cls, capacity) \
PHP_DS_SEQUENCE_ME(cls, contains) \
PHP_DS_SEQUENCE_ME(cls, filter) \
PHP_DS_SEQUENCE_ME(cls, find) \
PHP_DS_SEQUENCE_ME(cls, first) \
PHP_DS_SEQUENCE_ME(cls, get) \
PHP_DS_SEQUENCE_ME(cls, insert) \
PHP_DS_SEQUENCE_ME(cls, join) \
PHP_DS_SEQUENCE_ME(cls, last) \
PHP_DS_SEQUENCE_ME(cls, map) \
PHP_DS_SEQUENCE_ME(cls, merge) \
PHP_DS_SEQUENCE_ME(cls, offsetExists) \
PHP_DS_SEQUENCE_ME(cls, offsetGet) \
PHP_DS_SEQUENCE_ME(cls, offsetSet) \
PHP_DS_SEQUENCE_ME(cls, offsetUnset) \
PHP_DS_SEQUENCE_ME(cls, pop) \
PHP_DS_SEQUENCE_ME(cls, push) \
PHP_DS_SEQUENCE_ME(cls, reduce) \
PHP_DS_SEQUENCE_ME(cls, remove) \
PHP_DS_SEQUENCE_ME(cls, reverse) \
PHP_DS_SEQUENCE_ME(cls, reversed) \
PHP_DS_SEQUENCE_ME(cls, rotate) \
PHP_DS_SEQUENCE_ME(cls, set) \
PHP_DS_SEQUENCE_ME(cls, shift) \
PHP_DS_SEQUENCE_ME(cls, slice) \
PHP_DS_SEQUENCE_ME(cls, sort) \
PHP_DS_SEQUENCE_ME(cls, sorted) \
PHP_DS_SEQUENCE_ME(cls, sum) \
PHP_DS_SEQUENCE_ME(cls, unshift) \

ARGINFO_LONG(                           Sequence_allocate, capacity);
ARGINFO_CALLABLE(                       Sequence_apply, callback);
ARGINFO_NONE_RETURN_LONG(               Sequence_capacity);
ARGINFO_VARIADIC_ZVAL_RETURN_BOOL(      Sequence_contains, values);
ARGINFO_OPTIONAL_CALLABLE_RETURN_DS(    Sequence_filter, callback, Sequence);
ARGINFO_ZVAL(                           Sequence_find, value);
ARGINFO_NONE(                           Sequence_first);
ARGINFO_OPTIONAL_STRING_RETURN_STRING(  Sequence_join, glue);
ARGINFO_LONG(                           Sequence_get, index);
ARGINFO_LONG_VARIADIC_ZVAL(             Sequence_insert, index, values);
ARGINFO_NONE(                           Sequence_last);
ARGINFO_CALLABLE_RETURN_DS(             Sequence_map, callback, Sequence);
ARGINFO_ZVAL_RETURN_DS(                 Sequence_merge, values, Sequence);
ARGINFO_ZVAL_RETURN_BOOL(              	Sequence_offsetExists, offset);
ARGINFO_ZVAL(    						Sequence_offsetGet, offset);
ARGINFO_ZVAL_ZVAL(                      Sequence_offsetSet, offset, value);
ARGINFO_ZVAL(                  		    Sequence_offsetUnset, offset);
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
