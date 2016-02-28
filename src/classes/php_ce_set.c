#include "../common.h"
#include "../internal/php_set.h"
#include "../iterators/php_set_iterator.h"
#include "../handlers/php_set_handlers.h"
#include "php_ce_collection.h"
#include "php_ce_set.h"

#define METHOD(name) PHP_METHOD(Set, name)

zend_class_entry *set_ce;

ARGINFO_OPTIONAL_ZVAL(__construct, values)
METHOD(__construct)
{
    PARSE_OPTIONAL_ZVAL(values);

    if (values && Z_TYPE_P(values) == IS_LONG) {
        set_user_allocate(THIS_SET(), Z_LVAL_P(values));
    } else {
        set_add_all(THIS_SET(), values);
    }
}

ARGINFO_OPTIONAL_STRING(join, glue)
METHOD(join)
{
    if (ZEND_NUM_ARGS()) {
        PARSE_STRING();
        set_join(THIS_SET(), str, len, return_value);
    } else {
        set_join(THIS_SET(), NULL, 0, return_value);
    }
}

ARGINFO_LONG(allocate, capacity)
METHOD(allocate)
{
    PARSE_LONG(capacity);
    set_user_allocate(THIS_SET(), capacity);
}

ARGINFO_NONE_RETURN_LONG(capacity)
METHOD(capacity)
{
    PARSE_NONE;
    RETURN_LONG(set_capacity(THIS_SET()));
}

ARGINFO_VARIADIC_ZVAL(add, values)
METHOD(add)
{
    PARSE_VARIADIC_ZVAL();
    set_add_va(THIS_SET(), argc, argv);
}

ARGINFO_ZVAL(addAll, values)
METHOD(addAll)
{
    PARSE_ZVAL(values);
    set_add_all(THIS_SET(), values);
}

ARGINFO_VARIADIC_ZVAL(remove, values)
METHOD(remove)
{
    PARSE_VARIADIC_ZVAL();
    set_remove_va(THIS_SET(), argc, argv);
}

ARGINFO_LONG(get, index)
METHOD(get)
{
    PARSE_LONG(index);
    RETURN_ZVAL_COPY(set_get(THIS_SET(), index));
}

ARGINFO_VARIADIC_ZVAL_RETURN_BOOL(contains, values)
METHOD(contains)
{
    PARSE_VARIADIC_ZVAL();
    RETURN_BOOL(set_contains_all(THIS_SET(), argc, argv));
}

ARGINFO_DS_RETURN_DS(diff, set, Set, Set)
METHOD(diff)
{
    PARSE_OBJ(obj, set_ce);
    set_diff(THIS_SET(), Z_SET_P(obj), return_value);
}

ARGINFO_DS_RETURN_DS(intersect, set, Set, Set)
METHOD(intersect)
{
    PARSE_OBJ(obj, set_ce);
    set_intersect(THIS_SET(), Z_SET_P(obj), return_value);
}

ARGINFO_DS_RETURN_DS(xor, set, Set, Set)
METHOD(xor)
{
    PARSE_OBJ(obj, set_ce);
    set_xor(THIS_SET(), Z_SET_P(obj), return_value);
}

ARGINFO_NONE(first)
METHOD(first)
{
    PARSE_NONE;
    RETURN_ZVAL_COPY(set_get_first(THIS_SET()));
}

ARGINFO_NONE(last)
METHOD(last)
{
    PARSE_NONE;
    RETURN_ZVAL_COPY(set_get_last(THIS_SET()));
}

ARGINFO_DS_RETURN_DS(union, set, Set, Set)
METHOD(union)
{
    PARSE_OBJ(obj, set_ce);
    set_union(THIS_SET(), Z_SET_P(obj), return_value);
}

ARGINFO_NONE(clear)
METHOD(clear)
{
    PARSE_NONE;
    set_clear(THIS_SET());
}

ARGINFO_NONE_RETURN_ARRAY(toArray)
METHOD(toArray)
{
    PARSE_NONE;
    set_to_array(THIS_SET(), return_value);
}

ARGINFO_NONE_RETURN_LONG(count)
METHOD(count)
{
    PARSE_NONE;
    RETURN_LONG(SET_SIZE(THIS_SET()));
}

ARGINFO_NONE_RETURN_BOOL(isEmpty)
METHOD(isEmpty)
{
    PARSE_NONE;
    RETURN_BOOL(SET_IS_EMPTY(THIS_SET()));
}

ARGINFO_OPTIONAL_CALLABLE_RETURN_DS(sort, comparator, Set)
METHOD(sort)
{
    if (ZEND_NUM_ARGS()) {
        PARSE_COMPARE_CALLABLE();
        set_sorted_callback(THIS_SET(), return_value);
    } else {
        set_sorted(THIS_SET(), return_value);
    }
}

ARGINFO_NONE_RETURN_DS(copy, Set)
METHOD(copy)
{
    PARSE_NONE;
    RETURN_OBJ(set_create_clone(THIS_SET()));
}

ARGINFO_CALLABLE_OPTIONAL_ZVAL(reduce, callback, initial)
METHOD(reduce)
{
    PARSE_CALLABLE_AND_OPTIONAL_ZVAL(initial);
    set_reduce(THIS_SET(), FCI_ARGS, initial, return_value);
}

ARGINFO_LONG_OPTIONAL_LONG_RETURN_DS(slice, index, length, Set)
METHOD(slice)
{
    Set *set = THIS_SET();

    if (ZEND_NUM_ARGS() > 1) {
        PARSE_LONG_AND_LONG(index, length);
        set_slice(set, index, length, return_value);
    } else {
        PARSE_LONG(index);
        set_slice(set, index, SET_SIZE(set), return_value);
    }
}

ARGINFO_OPTIONAL_CALLABLE_RETURN_DS(filter, callback, Set)
METHOD(filter)
{
    if (ZEND_NUM_ARGS()) {
        PARSE_CALLABLE();
        set_filter_callback(THIS_SET(), FCI_ARGS, return_value);
    } else {
        set_filter(THIS_SET(), return_value);
    }
}

ARGINFO_NONE_RETURN_DS(reverse, Set)
METHOD(reverse)
{
    PARSE_NONE;
    set_reversed(THIS_SET(), return_value);
}

ARGINFO_NONE(jsonSerialize)
METHOD(jsonSerialize)
{
    PARSE_NONE;
    set_to_array(THIS_SET(), return_value);
}

void register_set()
{
    zend_class_entry ce;

    zend_function_entry methods[] = {
        COLLECTION_ME(Set, __construct)
        COLLECTION_ME(Set, add)
        COLLECTION_ME(Set, addAll)
        COLLECTION_ME(Set, allocate)
        COLLECTION_ME(Set, capacity)
        COLLECTION_ME(Set, contains)
        COLLECTION_ME(Set, diff)
        COLLECTION_ME(Set, filter)
        COLLECTION_ME(Set, first)
        COLLECTION_ME(Set, get)
        COLLECTION_ME(Set, intersect)
        COLLECTION_ME(Set, join)
        COLLECTION_ME(Set, last)
        COLLECTION_ME(Set, reduce)
        COLLECTION_ME(Set, remove)
        COLLECTION_ME(Set, reverse)
        COLLECTION_ME(Set, slice)
        COLLECTION_ME(Set, sort)
        COLLECTION_ME(Set, union)
        COLLECTION_ME(Set, xor)

        COLLECTION_ME_LIST(Set)
        PHP_FE_END
    };

    INIT_CLASS_ENTRY(ce, DS_NS(Set), methods);

    set_ce = zend_register_internal_class(&ce);
    set_ce->ce_flags      |= ZEND_ACC_FINAL;
    set_ce->create_object  = set_create_object;
    set_ce->get_iterator   = set_get_iterator;
    set_ce->serialize      = set_serialize;
    set_ce->unserialize    = set_unserialize;

    zend_class_implements(set_ce, 1, collection_ce);
    register_set_handlers();
}
