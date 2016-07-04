#include "../../common.h"

#include "../parameters.h"
#include "../arginfo.h"

#include "../../ds/ds_map.h"

#include "../iterators/php_map_iterator.h"
#include "../handlers/php_map_handlers.h"
#include "../objects/php_ds_vector.h"

#include "php_ce_collection.h"
#include "php_ce_map.h"

#define METHOD(name) PHP_METHOD(Map, name)

zend_class_entry *map_ce;

ARGINFO_OPTIONAL_ZVAL(__construct, values)
METHOD(__construct)
{
    PARSE_OPTIONAL_ZVAL(values);

    if (values) {
        if (Z_TYPE_P(values) == IS_LONG) {
            map_user_allocate(THIS_MAP(), Z_LVAL_P(values));
        } else {
            map_put_all(THIS_MAP(), values);
        }
    }
}

ARGINFO_LONG(allocate, capacity)
METHOD(allocate)
{
    PARSE_LONG(capacity);
    map_user_allocate(THIS_MAP(), capacity);
}

ARGINFO_NONE_RETURN_LONG(capacity)
METHOD(capacity)
{
    PARSE_NONE;
    RETURN_LONG(map_capacity(THIS_MAP()));
}

ARGINFO_ZVAL(putAll, values)
METHOD(putAll)
{
    PARSE_ZVAL(values);
    map_put_all(THIS_MAP(), values);
}

ARGINFO_ZVAL_ZVAL(put, key, value)
METHOD(put)
{
    PARSE_ZVAL_ZVAL(key, value);
    map_put(THIS_MAP(), key, value);
}

ARGINFO_ZVAL_OPTIONAL_ZVAL(get, key, default)
METHOD(get)
{
    PARSE_ZVAL_OPTIONAL_ZVAL(key, def);
    RETURN_ZVAL_COPY(map_get(THIS_MAP(), key, def));
}

ARGINFO_DS_RETURN_DS(intersect, map, Map, Map)
METHOD(intersect)
{
    PARSE_OBJ(obj, map_ce);
    map_intersect(THIS_MAP(), Z_MAP_P(obj), return_value);
}

ARGINFO_ZVAL_OPTIONAL_ZVAL(remove, key, default)
METHOD(remove)
{
    PARSE_ZVAL_OPTIONAL_ZVAL(key, def);
    map_remove(THIS_MAP(), key, def, return_value);
}

ARGINFO_VARIADIC_ZVAL_RETURN_BOOL(hasKey, keys)
METHOD(hasKey)
{
    PARSE_VARIADIC_ZVAL();
    RETURN_BOOL(map_has_keys(THIS_MAP(), argc, argv));
}

ARGINFO_VARIADIC_ZVAL_RETURN_BOOL(hasValue, values)
METHOD(hasValue)
{
    PARSE_VARIADIC_ZVAL();
    RETURN_BOOL(map_has_values(THIS_MAP(), argc, argv));
}

ARGINFO_DS_RETURN_DS(diff, map, Map, Map)
METHOD(diff)
{
    PARSE_OBJ(obj, map_ce);
    map_diff(THIS_MAP(), Z_MAP_P(obj), return_value);
}

ARGINFO_NONE(clear)
METHOD(clear)
{
    PARSE_NONE;
    map_clear(THIS_MAP());
}

ARGINFO_OPTIONAL_CALLABLE_RETURN_DS(sort, comparator, Map)
METHOD(sort)
{
    if (ZEND_NUM_ARGS()) {
        PARSE_COMPARE_CALLABLE();
        map_sorted_by_value_callback(THIS_MAP(), return_value);
    } else {
        map_sorted_by_value(THIS_MAP(), return_value);
    }
}

ARGINFO_OPTIONAL_CALLABLE_RETURN_DS(ksort, comparator, Map)
METHOD(ksort)
{
    if (ZEND_NUM_ARGS()) {
        PARSE_COMPARE_CALLABLE();
        map_sorted_by_key_callback(THIS_MAP(), return_value);
    } else {
        map_sorted_by_key(THIS_MAP(), return_value);
    }
}

ARGINFO_NONE_RETURN_DS(keys, Set)
METHOD(keys)
{
    PARSE_NONE;
    map_create_key_set(THIS_MAP(), return_value);
}

ARGINFO_NONE_RETURN_DS(last, Pair)
METHOD(last)
{
    PARSE_NONE;
    map_last(THIS_MAP(), return_value);
}

ARGINFO_ZVAL_RETURN_DS(merge, values, Map)
METHOD(merge)
{
    PARSE_ZVAL(values);
    map_merge(THIS_MAP(), values, return_value);
}

ARGINFO_NONE_RETURN_DS(pairs, Sequence)
METHOD(pairs)
{
    PARSE_NONE;
    RETURN_DS_VECTOR(map_pairs_to_vector(THIS_MAP()));
}

ARGINFO_NONE_RETURN_ARRAY(toArray)
METHOD(toArray)
{
    PARSE_NONE;
    map_to_array(THIS_MAP(), return_value);
}

ARGINFO_NONE_RETURN_LONG(count)
METHOD(count)
{
    PARSE_NONE;
    RETURN_LONG(MAP_SIZE(THIS_MAP()));
}

ARGINFO_NONE_RETURN_BOOL(isEmpty)
METHOD(isEmpty)
{
    PARSE_NONE;
    RETURN_BOOL(MAP_IS_EMPTY(THIS_MAP()));
}

ARGINFO_NONE_RETURN_DS(copy, Map)
METHOD(copy)
{
    PARSE_NONE;
    RETURN_OBJ(map_create_clone(THIS_MAP()));
}

ARGINFO_NONE(jsonSerialize)
METHOD(jsonSerialize)
{
    PARSE_NONE;
    map_to_array(THIS_MAP(), return_value);
}

ARGINFO_CALLABLE_RETURN_DS(filter, callback, Map)
METHOD(filter)
{
    PARSE_CALLABLE();
    map_filter_callback(THIS_MAP(), FCI_ARGS, return_value);
}

ARGINFO_NONE_RETURN_DS(first, Pair)
METHOD(first)
{
    PARSE_NONE;
    map_first(THIS_MAP(), return_value);
}

ARGINFO_CALLABLE_OPTIONAL_ZVAL(reduce, callback, initial)
METHOD(reduce)
{
    PARSE_CALLABLE_AND_OPTIONAL_ZVAL(initial);
    map_reduce(THIS_MAP(), FCI_ARGS, initial, return_value);
}

ARGINFO_NONE_RETURN_DS(reverse, Map)
METHOD(reverse)
{
    PARSE_NONE;
    map_reversed(THIS_MAP(), return_value);
}

ARGINFO_LONG_RETURN_DS(skip, position, Pair)
METHOD(skip)
{
    PARSE_LONG(position);
    map_skip(THIS_MAP(), position, return_value);
}

ARGINFO_CALLABLE_RETURN_DS(map, callback, Map)
METHOD(map)
{
    PARSE_CALLABLE();
    map_map(THIS_MAP(), FCI_ARGS, return_value);
}

ARGINFO_LONG_OPTIONAL_LONG_RETURN_DS(slice, index, length, Map)
METHOD(slice)
{
    Map *map = THIS_MAP();

    if (ZEND_NUM_ARGS() > 1) {
        PARSE_LONG_AND_LONG(index, length);
        map_slice(map, index, length, return_value);
    } else {
        PARSE_LONG(index);
        map_slice(map, index, MAP_SIZE(map), return_value);
    }
}

ARGINFO_NONE_RETURN_DS(values, Sequence)
METHOD(values)
{
    PARSE_NONE;
    RETURN_DS_VECTOR(map_values_to_vector(THIS_MAP()));
}

ARGINFO_DS_RETURN_DS(xor, map, Map, Map)
METHOD(xor)
{
    PARSE_OBJ(obj, map_ce);
    map_xor(THIS_MAP(), Z_MAP_P(obj), return_value);
}

void register_map()
{
    zend_class_entry ce;

    zend_function_entry methods[] = {
        COLLECTION_ME(Map, __construct)
        COLLECTION_ME(Map, allocate)
        COLLECTION_ME(Map, capacity)
        COLLECTION_ME(Map, hasKey)
        COLLECTION_ME(Map, hasValue)
        COLLECTION_ME(Map, diff)
        COLLECTION_ME(Map, filter)
        COLLECTION_ME(Map, first)
        COLLECTION_ME(Map, get)
        COLLECTION_ME(Map, intersect)
        COLLECTION_ME(Map, keys)
        COLLECTION_ME(Map, ksort)
        COLLECTION_ME(Map, last)
        COLLECTION_ME(Map, map)
        COLLECTION_ME(Map, merge)
        COLLECTION_ME(Map, pairs)
        COLLECTION_ME(Map, put)
        COLLECTION_ME(Map, putAll)
        COLLECTION_ME(Map, reduce)
        COLLECTION_ME(Map, remove)
        COLLECTION_ME(Map, reverse)
        COLLECTION_ME(Map, skip)
        COLLECTION_ME(Map, slice)
        COLLECTION_ME(Map, sort)
        COLLECTION_ME(Map, values)
        COLLECTION_ME(Map, xor)

        COLLECTION_ME_LIST(Map)
        PHP_FE_END
    };

    INIT_CLASS_ENTRY(ce, DS_NS(Map), methods);

    map_ce = zend_register_internal_class(&ce);
    map_ce->ce_flags      |= ZEND_ACC_FINAL;
    map_ce->create_object  = map_create_object;
    map_ce->get_iterator   = map_get_iterator;
    map_ce->serialize      = map_serialize;
    map_ce->unserialize    = map_unserialize;

    zend_declare_class_constant_long(
        map_ce,
        STR_AND_LEN("MIN_CAPACITY"),
        DS_HTABLE_MIN_CAPACITY
    );

    zend_class_implements(map_ce, 1, collection_ce);
    register_map_handlers();
}
