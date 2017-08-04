#include "../../ds_common.h"

#include "../php_ds_parameters.h"
#include "../php_ds_arginfo.h"

#include "../iterators/php_ds_sequence_iterator.h"
#include "../handlers/php_ds_sequence_handlers.h"

#include "php_ds_collection_ce.h"
#include "php_ds_sequence_ce.h"
#include "php_ds_map_ce.h"

zend_class_entry *php_ds_sequence_ce;

#define METHOD(name) PHP_METHOD(Sequence, name)

METHOD(__construct)
{
    PHP_DS_PARAM_PARSE_OPTIONAL_ZVAL(values);

    if (values) {
        ds_deque_push_all(THIS_DS_DEQUE(), values);
    }
}

METHOD(join)
{
    if (ZEND_NUM_ARGS()) {
        PHP_DS_PARAM_PARSE_STRING();
        ds_deque_join(THIS_DS_DEQUE(), str, len, return_value);
    } else {
        ds_deque_join(THIS_DS_DEQUE(), NULL, 0, return_value);
    }
}

METHOD(allocate)
{
    PHP_DS_PARAM_PARSE_LONG(capacity);
    ds_deque_allocate(THIS_DS_DEQUE(), capacity);
}

METHOD(apply)
{
    PHP_DS_PARAM_PARSE_CALLABLE();
    ds_deque_apply(THIS_DS_DEQUE(), FCI_ARGS);
}

METHOD(capacity)
{
    PHP_DS_PARAM_PARSE_NONE;
    RETURN_LONG((THIS_DS_DEQUE())->capacity);
}

METHOD(pushAll)
{
    PHP_DS_PARAM_PARSE_ZVAL(values);
    ds_deque_push_all(THIS_DS_DEQUE(), values);
}

METHOD(map)
{
    PHP_DS_PARAM_PARSE_CALLABLE();
    RETURN_DS_SEQUENCE(ds_deque_map(THIS_DS_DEQUE(), FCI_ARGS));
}

METHOD(merge)
{
    PHP_DS_PARAM_PARSE_ZVAL(values);
    RETURN_DS_SEQUENCE(ds_deque_merge(THIS_DS_DEQUE(), values));
}

METHOD(each)
{
    PHP_DS_PARAM_PARSE_CALLABLE();
    RETURN_BOOL(ds_deque_each(THIS_DS_DEQUE(), FCI_ARGS));
}

METHOD(groupBy)
{
    PHP_DS_PARAM_PARSE_ZVAL(iteratee);
    RETURN_DS_MAP(ds_deque_group_by(THIS_DS_DEQUE(), iteratee));
}

METHOD(partition)
{
    if (ZEND_NUM_ARGS()) {
        PHP_DS_PARAM_PARSE_CALLABLE();
        RETURN_LONG(ds_deque_partition_callback(THIS_DS_DEQUE(), FCI_ARGS));
    } else {
        RETURN_LONG(ds_deque_partition(THIS_DS_DEQUE()));
    }
}

METHOD(pluck)
{
    PHP_DS_PARAM_PARSE_ZVAL(key);
    RETURN_DS_SEQUENCE(ds_deque_pluck(THIS_DS_DEQUE(), key));
}


METHOD(reduce)
{
    PHP_DS_PARAM_PARSE_CALLABLE_AND_OPTIONAL_ZVAL(initial);
    ds_deque_reduce(THIS_DS_DEQUE(), initial, return_value, FCI_ARGS);
}

METHOD(filter)
{
    if (ZEND_NUM_ARGS()) {
        PHP_DS_PARAM_PARSE_CALLABLE();
        RETURN_DS_SEQUENCE(ds_deque_filter_callback(THIS_DS_DEQUE(), FCI_ARGS));
    } else {
        PHP_DS_PARAM_PARSE_NONE;
        RETURN_DS_SEQUENCE(ds_deque_filter(THIS_DS_DEQUE()));
    }
}

METHOD(slice)
{
    ds_deque_t *deque = THIS_DS_DEQUE();

    if (ZEND_NUM_ARGS() > 1) {
        PHP_DS_PARAM_PARSE_LONG_AND_LONG(index, length);
        RETURN_DS_SEQUENCE(ds_deque_slice(deque, index, length));
    } else {
        PHP_DS_PARAM_PARSE_LONG(index);
        RETURN_DS_SEQUENCE(ds_deque_slice(deque, index, deque->size));
    }
}

METHOD(sort)
{
    ds_deque_t *sorted = THIS_DS_DEQUE();

    if (ZEND_NUM_ARGS()) {
        PHP_DS_PARAM_PARSE_COMPARE_CALLABLE();
        ds_deque_sort_callback(sorted);
    } else {
        ds_deque_sort(sorted);
    }
}

METHOD(sorted)
{
    ds_deque_t *sorted = ds_deque_clone(THIS_DS_DEQUE());

    if (ZEND_NUM_ARGS()) {
        PHP_DS_PARAM_PARSE_COMPARE_CALLABLE();
        ds_deque_sort_callback(sorted);
    } else {
        ds_deque_sort(sorted);
    }

    RETURN_DS_SEQUENCE(sorted);
}

METHOD(push)
{
    PHP_DS_PARAM_PARSE_VARIADIC_ZVAL();
    if (argc == 1) {
        ds_deque_push(THIS_DS_DEQUE(), argv);
    } else {
        ds_deque_push_va(THIS_DS_DEQUE(), argc, argv);
    }
}

METHOD(unshift)
{
    PHP_DS_PARAM_PARSE_VARIADIC_ZVAL();
    ds_deque_unshift_va(THIS_DS_DEQUE(), argc, argv);
}

METHOD(pop)
{
    PHP_DS_PARAM_PARSE_NONE;
    ds_deque_pop_throw(THIS_DS_DEQUE(), return_value);
}

METHOD(shift)
{
    PHP_DS_PARAM_PARSE_NONE;
    ds_deque_shift_throw(THIS_DS_DEQUE(), return_value);
}

METHOD(first)
{
    PHP_DS_PARAM_PARSE_NONE;
    RETURN_ZVAL_COPY(ds_deque_get_first_throw(THIS_DS_DEQUE()));
}

METHOD(last)
{
    PHP_DS_PARAM_PARSE_NONE;
    RETURN_ZVAL_COPY(ds_deque_get_last_throw(THIS_DS_DEQUE()));
}

METHOD(count)
{
    ds_deque_t *deque = THIS_DS_DEQUE();
    PHP_DS_PARAM_PARSE_NONE;
    RETURN_LONG(deque->size);
}

METHOD(clear)
{
    PHP_DS_PARAM_PARSE_NONE;
    ds_deque_clear(THIS_DS_DEQUE());
}

METHOD(contains)
{
    PHP_DS_PARAM_PARSE_VARIADIC_ZVAL();
    RETURN_BOOL(ds_deque_contains_va(THIS_DS_DEQUE(), argc, argv));
}

METHOD(sum)
{
    PHP_DS_PARAM_PARSE_NONE;
    ds_deque_sum(THIS_DS_DEQUE(), return_value);
}

METHOD(toArray)
{
    PHP_DS_PARAM_PARSE_NONE;
    ds_deque_to_array(THIS_DS_DEQUE(), return_value);
}

METHOD(get)
{
    PHP_DS_PARAM_PARSE_LONG(index);
    RETURN_ZVAL_COPY(ds_deque_get(THIS_DS_DEQUE(), index));
}

METHOD(set)
{
    PHP_DS_PARAM_PARSE_LONG_AND_ZVAL(index, value);
    ds_deque_set(THIS_DS_DEQUE(), index, value);
}

METHOD(find)
{
    PHP_DS_PARAM_PARSE_ZVAL(value);
    ds_deque_find(THIS_DS_DEQUE(), value, return_value);
}

METHOD(remove)
{
    PHP_DS_PARAM_PARSE_LONG(index);
    ds_deque_remove(THIS_DS_DEQUE(), index, return_value);
}

METHOD(insert)
{
    PHP_DS_PARAM_PARSE_LONG_AND_VARIADIC_ZVAL(index);
    ds_deque_insert_va(THIS_DS_DEQUE(), index, argc, argv);
}

METHOD(reverse)
{
    PHP_DS_PARAM_PARSE_NONE;
    ds_deque_reverse(THIS_DS_DEQUE());
}

METHOD(reversed)
{
    PHP_DS_PARAM_PARSE_NONE;
    RETURN_DS_SEQUENCE(ds_deque_reversed(THIS_DS_DEQUE()));
}

METHOD(rotate)
{
    PHP_DS_PARAM_PARSE_LONG(rotations);
    ds_deque_rotate(THIS_DS_DEQUE(), rotations);
}

METHOD(isEmpty)
{
    PHP_DS_PARAM_PARSE_NONE;
    RETURN_BOOL(DS_DEQUE_IS_EMPTY(THIS_DS_DEQUE()));
}

METHOD(copy)
{
    PHP_DS_PARAM_PARSE_NONE;
    RETURN_OBJ(php_ds_sequence_create_clone(THIS_DS_DEQUE()));
}

METHOD(jsonSerialize)
{
    PHP_DS_PARAM_PARSE_NONE;
    ds_deque_to_array(THIS_DS_DEQUE(), return_value);
}

PHP_DS_ARRAY_ACCESS_FORWARDING_METHODS(php_ds_sequence);

void php_ds_register_sequence()
{
    zend_class_entry ce;

    zend_function_entry methods[] = {
        PHP_DS_ME(Sequence, __construct)
        PHP_DS_ME(Sequence, allocate)
        PHP_DS_ME(Sequence, apply)
        PHP_DS_ME(Sequence, capacity)
        PHP_DS_ME(Sequence, contains)
        PHP_DS_ME(Sequence, each)
        PHP_DS_ME(Sequence, filter)
        PHP_DS_ME(Sequence, find)
        PHP_DS_ME(Sequence, first)
        PHP_DS_ME(Sequence, get)
        PHP_DS_ME(Sequence, groupBy)
        PHP_DS_ME(Sequence, insert)
        PHP_DS_ME(Sequence, join)
        PHP_DS_ME(Sequence, last)
        PHP_DS_ME(Sequence, map)
        PHP_DS_ME(Sequence, merge)
        PHP_DS_ME(Sequence, partition)
        PHP_DS_ME(Sequence, pluck)
        PHP_DS_ME(Sequence, pop)
        PHP_DS_ME(Sequence, push)
        PHP_DS_ME(Sequence, reduce)
        PHP_DS_ME(Sequence, remove)
        PHP_DS_ME(Sequence, reverse)
        PHP_DS_ME(Sequence, reversed)
        PHP_DS_ME(Sequence, rotate)
        PHP_DS_ME(Sequence, set)
        PHP_DS_ME(Sequence, shift)
        PHP_DS_ME(Sequence, slice)
        PHP_DS_ME(Sequence, sort)
        PHP_DS_ME(Sequence, sorted)
        PHP_DS_ME(Sequence, sum)
        PHP_DS_ME(Sequence, unshift)

        PHP_DS_COLLECTION_ME_LIST(Sequence)
        PHP_FE_END
    };

    INIT_CLASS_ENTRY(ce, PHP_DS_NS(Sequence), methods);

    php_ds_sequence_ce                 = zend_register_internal_class(&ce);
    php_ds_sequence_ce->ce_flags      |= ZEND_ACC_FINAL;
    php_ds_sequence_ce->create_object  = php_ds_sequence_create_object;
    php_ds_sequence_ce->get_iterator   = php_ds_sequence_get_iterator;
    php_ds_sequence_ce->serialize      = php_ds_sequence_serialize;
    php_ds_sequence_ce->unserialize    = php_ds_sequence_unserialize;

    zend_class_implements(php_ds_sequence_ce, 1, php_ds_collection_ce);

    php_ds_register_sequence_handlers();
}
