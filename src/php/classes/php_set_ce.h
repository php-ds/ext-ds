#ifndef DS_SET_CE_H
#define DS_SET_CE_H

#include "php.h"
#include "../../common.h"
#include "../arginfo.h"

extern zend_class_entry *php_ds_set_ce;

ARGINFO_OPTIONAL_ZVAL(                      Set___construct, values);
ARGINFO_OPTIONAL_STRING(                    Set_join, glue);
ARGINFO_LONG(                               Set_allocate, capacity);
ARGINFO_NONE_RETURN_LONG(                   Set_capacity);
ARGINFO_VARIADIC_ZVAL(                      Set_add, values);
ARGINFO_VARIADIC_ZVAL(                      Set_remove, values);
ARGINFO_LONG(                               Set_get, index);
ARGINFO_VARIADIC_ZVAL_RETURN_BOOL(          Set_contains, values);
ARGINFO_DS_RETURN_DS(                       Set_diff, set, Set, Set);
ARGINFO_DS_RETURN_DS(                       Set_intersect, set, Set, Set);
ARGINFO_DS_RETURN_DS(                       Set_xor, set, Set, Set);
ARGINFO_NONE(                               Set_first);
ARGINFO_NONE(                               Set_last);
ARGINFO_ZVAL_RETURN_DS(                     Set_merge, values, Set);
ARGINFO_DS_RETURN_DS(                       Set_union, set, Set, Set);
ARGINFO_OPTIONAL_CALLABLE(                  Set_sort, comparator);
ARGINFO_OPTIONAL_CALLABLE_RETURN_DS(        Set_sorted, comparator, Set);
ARGINFO_CALLABLE_OPTIONAL_ZVAL(             Set_reduce, callback, initial);
ARGINFO_LONG_OPTIONAL_LONG_RETURN_DS(       Set_slice, index, length, Set);
ARGINFO_OPTIONAL_CALLABLE_RETURN_DS(        Set_filter, predicate, Set);
ARGINFO_CALLABLE_RETURN_DS(                 Set_map, callback, Set);
ARGINFO_NONE(                               Set_reverse);
ARGINFO_NONE_RETURN_DS(                     Set_reversed, Set);
ARGINFO_NONE(                               Set_sum);

void php_ds_register_set();

#endif
