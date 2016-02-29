#include "../common.h"
#include "../internal/php_deque.h"
#include "../iterators/php_deque_iterator.h"
#include "../handlers/php_deque_handlers.h"
#include "php_ce_collection.h"
#include "php_ce_sequence.h"
#include "php_ce_deque.h"

#define METHOD(name) PHP_METHOD(Deque, name)

zend_class_entry *deque_ce;

METHOD(__construct)
{
    PARSE_OPTIONAL_ZVAL(values);

    if ( ! values) {
        return;
    } else if (Z_TYPE_P(values) == IS_LONG) {
        deque_user_allocate(THIS_DEQUE(), Z_LVAL_P(values));
    } else {
        deque_push_all(THIS_DEQUE(), values);
    }
}

METHOD(join)
{
    if (ZEND_NUM_ARGS()) {
        PARSE_STRING();
        deque_join(THIS_DEQUE(), str, len, return_value);
    } else {
        deque_join(THIS_DEQUE(), NULL, 0, return_value);
    }
}

METHOD(rotate)
{
    PARSE_LONG(rotations);
    deque_rotate(THIS_DEQUE(), rotations);
}

METHOD(allocate)
{
    PARSE_LONG(capacity);
    deque_user_allocate(THIS_DEQUE(), capacity);
}

METHOD(capacity)
{
    PARSE_NONE;
    RETURN_LONG((THIS_DEQUE())->capacity);
}

METHOD(pushAll)
{
    PARSE_ZVAL(values);
    deque_push_all(THIS_DEQUE(), values);
}

METHOD(map)
{
    PARSE_CALLABLE();
    deque_map(THIS_DEQUE(), return_value, FCI_ARGS);
}

METHOD(merge)
{
    PARSE_ZVAL(values);
    deque_merge(THIS_DEQUE(), values, return_value);
}

METHOD(reduce)
{
    PARSE_CALLABLE_AND_OPTIONAL_ZVAL(initial);
    deque_reduce(THIS_DEQUE(), initial, return_value, FCI_ARGS);
}

METHOD(filter)
{
    if (ZEND_NUM_ARGS()) {
        PARSE_CALLABLE();
        deque_filter_callback(THIS_DEQUE(), return_value, FCI_ARGS);
    } else {
        PARSE_NONE;
        deque_filter(THIS_DEQUE(), return_value);
    }
}

METHOD(slice)
{
    Deque *deque = THIS_DEQUE();

    if (ZEND_NUM_ARGS() > 1) {
        PARSE_LONG_AND_LONG(index, length);
        deque_slice(deque, index, length, return_value);
    } else {
        PARSE_LONG(index);
        deque_slice(deque, index, DEQUE_SIZE(deque), return_value);
    }
}

METHOD(sort)
{
    Deque *sorted = deque_create_copy(THIS_DEQUE());

    if (ZEND_NUM_ARGS()) {
        PARSE_COMPARE_CALLABLE();
        deque_sort_callback(sorted);
    } else {
        deque_sort(sorted);
    }

    RETURN_DEQUE(sorted);
}

METHOD(push)
{
    PARSE_VARIADIC_ZVAL();
    deque_push_va(THIS_DEQUE(), argc, argv);
}

METHOD(unshift)
{
    PARSE_VARIADIC_ZVAL();
    deque_unshift_va(THIS_DEQUE(), argc, argv);
}

METHOD(pop)
{
    PARSE_NONE;
    deque_pop(THIS_DEQUE(), return_value);
}

METHOD(shift)
{
    PARSE_NONE;
    deque_shift(THIS_DEQUE(), return_value);
}

METHOD(first)
{
    PARSE_NONE;
    RETURN_ZVAL_COPY(deque_get_first(THIS_DEQUE()));
}

METHOD(last)
{
    PARSE_NONE;
    RETURN_ZVAL_COPY(deque_get_last(THIS_DEQUE()));
}

METHOD(count)
{
    Deque *deque = THIS_DEQUE();
    PARSE_NONE;
    RETURN_LONG(DEQUE_SIZE(deque));
}

METHOD(clear)
{
    PARSE_NONE;
    deque_clear(THIS_DEQUE());
}

METHOD(contains)
{
    PARSE_VARIADIC_ZVAL();
    RETURN_BOOL(deque_contains_va(THIS_DEQUE(), argc, argv));
}

METHOD(toArray)
{
    PARSE_NONE;
    deque_to_array(THIS_DEQUE(), return_value);
}

METHOD(get)
{
    PARSE_LONG(index);
    RETURN_ZVAL_COPY(deque_get(THIS_DEQUE(), index));
}

METHOD(set)
{
    PARSE_LONG_AND_ZVAL(index, value);
    deque_set(THIS_DEQUE(), index, value);
}

METHOD(find)
{
    PARSE_ZVAL(value);
    deque_find(THIS_DEQUE(), value, return_value);
}

METHOD(remove)
{
    PARSE_LONG(index);
    deque_remove(THIS_DEQUE(), index, return_value);
}

METHOD(insert)
{
    PARSE_LONG_AND_VARARGS(index);
    deque_insert_va(THIS_DEQUE(), index, argc, argv);
}

METHOD(reverse)
{
    PARSE_NONE;
    RETURN_DEQUE(deque_reversed(THIS_DEQUE()));
}

METHOD(isEmpty)
{
    PARSE_NONE;
    RETURN_BOOL(DEQUE_IS_EMPTY(THIS_DEQUE()));
}

METHOD(copy)
{
    PARSE_NONE;
    RETURN_OBJ(deque_create_clone(THIS_DEQUE()));
}

METHOD(jsonSerialize)
{
    PARSE_NONE;
    deque_to_array(THIS_DEQUE(), return_value);
}


void register_deque()
{
    zend_class_entry ce;

    zend_function_entry methods[] = {
        COLLECTION_ME_LIST(Deque)
        SEQUENCE_ME_LIST(Deque)
        PHP_FE_END
    };

    INIT_CLASS_ENTRY(ce, DS_NS(Deque), methods);

    deque_ce = zend_register_internal_class(&ce);
    deque_ce->ce_flags      |= ZEND_ACC_FINAL;
    deque_ce->create_object  = deque_create_object;
    deque_ce->get_iterator   = deque_get_iterator;
    deque_ce->serialize      = deque_serialize;
    deque_ce->unserialize    = deque_unserialize;

    zend_declare_class_constant_long(deque_ce, STR_AND_LEN("MIN_CAPACITY"), DEQUE_MIN_CAPACITY);

    zend_class_implements(deque_ce, 1, sequence_ce);
    register_deque_handlers();
}
