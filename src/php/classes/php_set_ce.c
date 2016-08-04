#include "../../common.h"

#include "../parameters.h"
#include "../arginfo.h"

#include "../objects/php_set.h"

#include "../iterators/php_set_iterator.h"
#include "../handlers/php_set_handlers.h"

#include "php_collection_ce.h"
#include "php_set_ce.h"

#define METHOD(name) PHP_METHOD(Set, name)

zend_class_entry *php_ds_set_ce;

ARGINFO_OPTIONAL_ZVAL(__construct, values)
METHOD(__construct)
{
    PARSE_OPTIONAL_ZVAL(values);

    if (values) {
        ds_set_add_all(THIS_DS_SET(), values);
    }
}

ARGINFO_OPTIONAL_STRING(join, glue)
METHOD(join)
{
    if (ZEND_NUM_ARGS()) {
        PARSE_STRING();
        ds_set_join(THIS_DS_SET(), str, len, return_value);
    } else {
        ds_set_join(THIS_DS_SET(), NULL, 0, return_value);
    }
}

ARGINFO_LONG(allocate, capacity)
METHOD(allocate)
{
    PARSE_LONG(capacity);
    ds_set_allocate(THIS_DS_SET(), capacity);
}

ARGINFO_NONE_RETURN_LONG(capacity)
METHOD(capacity)
{
    PARSE_NONE;
    RETURN_LONG(DS_SET_CAPACITY(THIS_DS_SET()));
}

ARGINFO_VARIADIC_ZVAL(add, values)
METHOD(add)
{
    PARSE_VARIADIC_ZVAL();
    ds_set_add_va(THIS_DS_SET(), argc, argv);
}

ARGINFO_VARIADIC_ZVAL(remove, values)
METHOD(remove)
{
    PARSE_VARIADIC_ZVAL();
    ds_set_remove_va(THIS_DS_SET(), argc, argv);
}

ARGINFO_LONG(get, index)
METHOD(get)
{
    PARSE_LONG(index);
    RETURN_ZVAL_COPY(ds_set_get(THIS_DS_SET(), index));
}

ARGINFO_VARIADIC_ZVAL_RETURN_BOOL(contains, values)
METHOD(contains)
{
    PARSE_VARIADIC_ZVAL();
    RETURN_BOOL(ds_set_contains_va(THIS_DS_SET(), argc, argv));
}

ARGINFO_DS_RETURN_DS(diff, set, Set, Set)
METHOD(diff)
{
    PARSE_OBJ(obj, php_ds_set_ce);
    RETURN_DS_SET(ds_set_diff(THIS_DS_SET(), Z_DS_SET_P(obj)));
}

ARGINFO_DS_RETURN_DS(intersect, set, Set, Set)
METHOD(intersect)
{
    PARSE_OBJ(obj, php_ds_set_ce);
    RETURN_DS_SET(ds_set_intersect(THIS_DS_SET(), Z_DS_SET_P(obj)));
}

ARGINFO_DS_RETURN_DS(xor, set, Set, Set)
METHOD(xor)
{
    PARSE_OBJ(obj, php_ds_set_ce);
    RETURN_DS_SET(ds_set_xor(THIS_DS_SET(), Z_DS_SET_P(obj)));
}

ARGINFO_NONE(first)
METHOD(first)
{
    PARSE_NONE;
    RETURN_ZVAL_COPY(ds_set_get_first(THIS_DS_SET()));
}

ARGINFO_NONE(last)
METHOD(last)
{
    PARSE_NONE;
    RETURN_ZVAL_COPY(ds_set_get_last(THIS_DS_SET()));
}

ARGINFO_ZVAL_RETURN_DS(merge, values, Set)
METHOD(merge)
{
    PARSE_ZVAL(values);
    RETURN_DS_SET(ds_set_merge(THIS_DS_SET(), values));
}

ARGINFO_DS_RETURN_DS(union, set, Set, Set)
METHOD(union)
{
    PARSE_OBJ(obj, php_ds_set_ce);
    RETURN_DS_SET(ds_set_union(THIS_DS_SET(), Z_DS_SET_P(obj)));
}

ARGINFO_NONE(clear)
METHOD(clear)
{
    PARSE_NONE;
    ds_set_clear(THIS_DS_SET());
}

ARGINFO_NONE_RETURN_ARRAY(toArray)
METHOD(toArray)
{
    PARSE_NONE;
    ds_set_to_array(THIS_DS_SET(), return_value);
}

ARGINFO_NONE_RETURN_LONG(count)
METHOD(count)
{
    PARSE_NONE;
    RETURN_LONG(DS_SET_SIZE(THIS_DS_SET()));
}

ARGINFO_NONE_RETURN_BOOL(isEmpty)
METHOD(isEmpty)
{
    PARSE_NONE;
    RETURN_BOOL(DS_SET_IS_EMPTY(THIS_DS_SET()));
}

ARGINFO_OPTIONAL_CALLABLE(sort, comparator)
METHOD(sort)
{
    if (ZEND_NUM_ARGS()) {
        PARSE_COMPARE_CALLABLE();
        ds_set_sort_callback(THIS_DS_SET());
    } else {
        ds_set_sort(THIS_DS_SET());
    }
}

ARGINFO_OPTIONAL_CALLABLE_RETURN_DS(sorted, comparator, Set)
METHOD(sorted)
{
    if (ZEND_NUM_ARGS()) {
        PARSE_COMPARE_CALLABLE();
        RETURN_DS_SET(ds_set_sorted_callback(THIS_DS_SET()));
    } else {
        RETURN_DS_SET(ds_set_sorted(THIS_DS_SET()));
    }
}

ARGINFO_NONE_RETURN_DS(copy, Set)
METHOD(copy)
{
    PARSE_NONE;
    RETURN_OBJ(php_ds_set_create_clone(THIS_DS_SET()));
}

ARGINFO_CALLABLE_OPTIONAL_ZVAL(reduce, callback, initial)
METHOD(reduce)
{
    PARSE_CALLABLE_AND_OPTIONAL_ZVAL(initial);
    ds_set_reduce(THIS_DS_SET(), FCI_ARGS, initial, return_value);
}

ARGINFO_LONG_OPTIONAL_LONG_RETURN_DS(slice, index, length, Set)
METHOD(slice)
{
    ds_set_t *set = THIS_DS_SET();

    if (ZEND_NUM_ARGS() > 1) {
        PARSE_LONG_AND_LONG(index, length);
        RETURN_DS_SET(ds_set_slice(set, index, length));
    } else {
        PARSE_LONG(index);
        RETURN_DS_SET(ds_set_slice(set, index, DS_SET_SIZE(set)));
    }
}

ARGINFO_OPTIONAL_CALLABLE_RETURN_DS(filter, callback, Set)
METHOD(filter)
{
    if (ZEND_NUM_ARGS()) {
        PARSE_CALLABLE();
        RETURN_DS_SET(ds_set_filter_callback(THIS_DS_SET(), FCI_ARGS));
    } else {
        RETURN_DS_SET(ds_set_filter(THIS_DS_SET()));
    }
}

ARGINFO_NONE(reverse)
METHOD(reverse)
{
    PARSE_NONE;
    ds_set_reverse(THIS_DS_SET());
}

ARGINFO_NONE_RETURN_DS(reversed, Set)
METHOD(reversed)
{
    PARSE_NONE;
    RETURN_DS_SET(ds_set_reversed(THIS_DS_SET()));
}

ARGINFO_NONE(sum)
METHOD(sum)
{
    PARSE_NONE;
    ds_set_sum(THIS_DS_SET(), return_value);
}

ARGINFO_NONE(jsonSerialize)
METHOD(jsonSerialize)
{
    PARSE_NONE;
    ds_set_to_array(THIS_DS_SET(), return_value);
}

void php_ds_register_set()
{
    zend_class_entry ce;

    zend_function_entry methods[] = {
        PHP_DS_ME(Set, __construct)
        PHP_DS_ME(Set, add)
        PHP_DS_ME(Set, allocate)
        PHP_DS_ME(Set, capacity)
        PHP_DS_ME(Set, contains)
        PHP_DS_ME(Set, diff)
        PHP_DS_ME(Set, filter)
        PHP_DS_ME(Set, first)
        PHP_DS_ME(Set, get)
        PHP_DS_ME(Set, intersect)
        PHP_DS_ME(Set, join)
        PHP_DS_ME(Set, last)
        PHP_DS_ME(Set, merge)
        PHP_DS_ME(Set, reduce)
        PHP_DS_ME(Set, remove)
        PHP_DS_ME(Set, reverse)
        PHP_DS_ME(Set, reversed)
        PHP_DS_ME(Set, slice)
        PHP_DS_ME(Set, sort)
        PHP_DS_ME(Set, sorted)
        PHP_DS_ME(Set, sum)
        PHP_DS_ME(Set, union)
        PHP_DS_ME(Set, xor)

        PHP_DS_COLLECTION_ME_LIST(Set)
        PHP_FE_END
    };

    INIT_CLASS_ENTRY(ce, PHP_DS_NS(Set), methods);

    php_ds_set_ce = zend_register_internal_class(&ce);
    php_ds_set_ce->ce_flags      |= ZEND_ACC_FINAL;
    php_ds_set_ce->create_object  = php_ds_set_create_object;
    php_ds_set_ce->get_iterator   = php_ds_set_get_iterator;
    php_ds_set_ce->serialize      = php_ds_set_serialize;
    php_ds_set_ce->unserialize    = php_ds_set_unserialize;

    zend_declare_class_constant_long(
        php_ds_set_ce,
        STR_AND_LEN("MIN_CAPACITY"),
        DS_HTABLE_MIN_CAPACITY
    );

    zend_class_implements(php_ds_set_ce, 1, collection_ce);
    php_ds_register_set_handlers();
}
