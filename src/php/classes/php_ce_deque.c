#include "../../common.h"

#include "../parameters.h"
#include "../arginfo.h"

#include "../objects/php_ds_deque.h"
#include "../iterators/php_deque_iterator.h"
#include "../handlers/php_deque_handlers.h"

#include "php_ce_collection.h"
#include "php_ce_sequence.h"
#include "php_ce_deque.h"

#define METHOD(name) PHP_METHOD(Deque, name)

zend_class_entry *php_ds_deque_ce;

METHOD(__construct)
{
    PARSE_OPTIONAL_ZVAL(values);

    if ( ! values) {
        return;
    } else if (Z_TYPE_P(values) == IS_LONG) {
        ds_deque_user_allocate(THIS_DEQUE(), Z_LVAL_P(values));
    } else {
        ds_deque_push_all(THIS_DEQUE(), values);
    }
}

METHOD(join)
{
    if (ZEND_NUM_ARGS()) {
        PARSE_STRING();
        ds_deque_join(THIS_DEQUE(), str, len, return_value);
    } else {
        ds_deque_join(THIS_DEQUE(), NULL, 0, return_value);
    }
}

METHOD(rotate)
{
    PARSE_LONG(rotations);
    ds_deque_rotate(THIS_DEQUE(), rotations);
}

METHOD(allocate)
{
    PARSE_LONG(capacity);
    ds_deque_user_allocate(THIS_DEQUE(), capacity);
}

METHOD(capacity)
{
    PARSE_NONE;
    RETURN_LONG((THIS_DEQUE())->capacity);
}

METHOD(pushAll)
{
    PARSE_ZVAL(values);
    ds_deque_push_all(THIS_DEQUE(), values);
}

METHOD(map)
{
    PARSE_CALLABLE();
    RETURN_DEQUE(ds_deque_map(THIS_DEQUE(), FCI_ARGS));
}

METHOD(merge)
{
    PARSE_ZVAL(values);
    deque_merge(THIS_DEQUE(), values, return_value);
}

METHOD(reduce)
{
    PARSE_CALLABLE_AND_OPTIONAL_ZVAL(initial);
    ds_deque_reduce(THIS_DEQUE(), initial, return_value, FCI_ARGS);
}

METHOD(filter)
{
    if (ZEND_NUM_ARGS()) {
        PARSE_CALLABLE();
        RETURN_DEQUE(ds_deque_filter_callback(THIS_DEQUE(), FCI_ARGS));
    } else {
        PARSE_NONE;
        RETURN_DEQUE(ds_deque_filter(THIS_DEQUE()));
    }
}

METHOD(slice)
{
    ds_deque_t *deque = THIS_DEQUE();

    if (ZEND_NUM_ARGS() > 1) {
        PARSE_LONG_AND_LONG(index, length);
        RETURN_DEQUE(ds_deque_slice(deque, index, length));
    } else {
        PARSE_LONG(index);
        RETURN_DEQUE(ds_deque_slice(deque, index, DS_DEQUE_SIZE(deque)));
    }
}

METHOD(sort)
{
    ds_deque_t *sorted = ds_deque_create_copy(THIS_DEQUE());

    if (ZEND_NUM_ARGS()) {
        PARSE_COMPARE_CALLABLE();
        ds_deque_sort_callback(sorted);
    } else {
        ds_deque_sort(sorted);
    }

    RETURN_DEQUE(sorted);
}

METHOD(push)
{
    PARSE_VARIADIC_ZVAL();
    ds_deque_push_va(THIS_DEQUE(), argc, argv);
}

METHOD(unshift)
{
    PARSE_VARIADIC_ZVAL();
    ds_deque_unshift_va(THIS_DEQUE(), argc, argv);
}

METHOD(pop)
{
    PARSE_NONE;
    ds_deque_pop(THIS_DEQUE(), return_value);
}

METHOD(shift)
{
    PARSE_NONE;
    ds_deque_shift(THIS_DEQUE(), return_value);
}

METHOD(first)
{
    PARSE_NONE;
    RETURN_ZVAL_COPY(ds_deque_get_first(THIS_DEQUE()));
}

METHOD(last)
{
    PARSE_NONE;
    RETURN_ZVAL_COPY(ds_deque_get_last(THIS_DEQUE()));
}

METHOD(count)
{
    ds_deque_t *deque = THIS_DEQUE();
    PARSE_NONE;
    RETURN_LONG(DS_DEQUE_SIZE(deque));
}

METHOD(clear)
{
    PARSE_NONE;
    ds_deque_clear(THIS_DEQUE());
}

METHOD(contains)
{
    PARSE_VARIADIC_ZVAL();
    RETURN_BOOL(ds_deque_contains_va(THIS_DEQUE(), argc, argv));
}

METHOD(toArray)
{
    PARSE_NONE;
    ds_deque_to_array(THIS_DEQUE(), return_value);
}

METHOD(get)
{
    PARSE_LONG(index);
    RETURN_ZVAL_COPY(ds_deque_get(THIS_DEQUE(), index));
}

METHOD(set)
{
    PARSE_LONG_AND_ZVAL(index, value);
    ds_deque_set(THIS_DEQUE(), index, value);
}

METHOD(find)
{
    PARSE_ZVAL(value);
    ds_deque_find(THIS_DEQUE(), value, return_value);
}

METHOD(remove)
{
    PARSE_LONG(index);
    ds_deque_remove(THIS_DEQUE(), index, return_value);
}

METHOD(insert)
{
    PARSE_LONG_AND_VARARGS(index);
    ds_deque_insert_va(THIS_DEQUE(), index, argc, argv);
}

METHOD(reverse)
{
    PARSE_NONE;
    RETURN_DEQUE(ds_deque_reversed(THIS_DEQUE()));
}

METHOD(isEmpty)
{
    PARSE_NONE;
    RETURN_BOOL(DS_DEQUE_IS_EMPTY(THIS_DEQUE()));
}

METHOD(copy)
{
    PARSE_NONE;
    RETURN_OBJ(php_ds_deque_create_clone(THIS_DEQUE()));
}

METHOD(jsonSerialize)
{
    PARSE_NONE;
    ds_deque_to_array(THIS_DEQUE(), return_value);
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

    php_ds_deque_ce = zend_register_internal_class(&ce);
    php_ds_deque_ce->ce_flags      |= ZEND_ACC_FINAL;
    php_ds_deque_ce->create_object  = php_ds_deque_create_object;
    php_ds_deque_ce->get_iterator   = ds_deque_get_iterator;
    php_ds_deque_ce->serialize      = php_ds_deque_serialize;
    php_ds_deque_ce->unserialize    = php_ds_deque_unserialize;

    zend_declare_class_constant_long(deque_ce, STR_AND_LEN("MIN_CAPACITY"), DEQUE_MIN_CAPACITY);
    zend_class_implements(php_ds_deque_ce, 1, sequence_ce);

    register_deque_handlers();
}
