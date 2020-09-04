#ifndef DS_MAP_CE_H
#define DS_MAP_CE_H

#include "php.h"
#include "../../common.h"
#include "../arginfo.h"

extern zend_class_entry *php_ds_map_ce;

ARGINFO_OPTIONAL_ZVAL(                      Map___construct, values);
ARGINFO_LONG(                               Map_allocate, capacity);
ARGINFO_CALLABLE(                           Map_apply, callback);
ARGINFO_NONE_RETURN_LONG(                   Map_capacity);
ARGINFO_ZVAL_ZVAL(                          Map_put, key, value);
ARGINFO_ZVAL(                               Map_putAll, values);
ARGINFO_ZVAL_OPTIONAL_ZVAL(                 Map_get, key, default);
ARGINFO_DS_RETURN_DS(                       Map_intersect, map, Map, Map);
ARGINFO_ZVAL_OPTIONAL_ZVAL(                 Map_remove, key, default);
ARGINFO_ZVAL_RETURN_BOOL(                   Map_hasKey, key);
ARGINFO_ZVAL_RETURN_BOOL(                   Map_hasValue, value);
ARGINFO_DS_RETURN_DS(                       Map_diff, map, Map, Map);
ARGINFO_OPTIONAL_CALLABLE(                  Map_sort, comparator);
ARGINFO_OPTIONAL_CALLABLE_RETURN_DS(        Map_sorted, comparator, Map);
ARGINFO_OPTIONAL_CALLABLE(                  Map_ksort, comparator);
ARGINFO_OPTIONAL_CALLABLE_RETURN_DS(        Map_ksorted, comparator, Map);
ARGINFO_NONE_RETURN_DS(                     Map_keys, Set);
ARGINFO_NONE_RETURN_DS(                     Map_last, Pair);
ARGINFO_ZVAL_RETURN_DS(                     Map_merge, values, Map);
ARGINFO_NONE_RETURN_DS(                     Map_pairs, Sequence);
ARGINFO_NONE(                               Map_jsonSerialize);
ARGINFO_OPTIONAL_CALLABLE_RETURN_DS(        Map_filter, callback, Map);
ARGINFO_NONE_RETURN_DS(                     Map_first, Pair);
ARGINFO_CALLABLE_OPTIONAL_ZVAL(             Map_reduce, callback, initial);
ARGINFO_NONE(                               Map_reverse);
ARGINFO_NONE_RETURN_DS(                     Map_reversed, Map);
ARGINFO_LONG_RETURN_DS(                     Map_skip, position, Pair);
ARGINFO_CALLABLE_RETURN_DS(                 Map_map, callback, Map);
ARGINFO_LONG_OPTIONAL_LONG_RETURN_DS(       Map_slice, index, length, Map);
ARGINFO_NONE(                               Map_sum);
ARGINFO_ZVAL_RETURN_DS(                     Map_union, map, Map);
ARGINFO_NONE_RETURN_DS(                     Map_values, Sequence);
ARGINFO_DS_RETURN_DS(                       Map_xor, map, Map, Map);
ARGINFO_NONE_RETURN_OBJ(					Map_getIterator, Traversable);

void php_ds_register_map();

#endif
