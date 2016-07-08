#include "../../common.h"

#include "../parameters.h"
#include "../arginfo.h"

#include "../objects/php_deque.h"
#include "../iterators/php_deque_iterator.h"
#include "../handlers/php_deque_handlers.h"

#include "php_collection_ce.h"
#include "php_sequence_ce.h"
#include "php_deque_ce.h"

#define METHOD(name) PHP_METHOD(Deque, name)

zend_class_entry *php_ds_deque_ce;

METHOD(__construct)
{
    PARSE_OPTIONAL_ZVAL(values);

    if ( ! values) {
        return;
    } else if (Z_TYPE_P(values) == IS_LONG) {
        ds_deque_allocate(THIS_DS_DEQUE(), Z_LVAL_P(values));
    } else {
        ds_deque_push_all(THIS_DS_DEQUE(), values);
    }
}

METHOD(join)
{
    if (ZEND_NUM_ARGS()) {
        PARSE_STRING();
        ds_deque_join(THIS_DS_DEQUE(), str, len, return_value);
    } else {
        ds_deque_join(THIS_DS_DEQUE(), NULL, 0, return_value);
    }
}

METHOD(rotate)
{
    PARSE_LONG(rotations);
    ds_deque_rotate(THIS_DS_DEQUE(), rotations);
}

METHOD(allocate)
{
    PARSE_LONG(capacity);
    ds_deque_allocate(THIS_DS_DEQUE(), capacity);
}

METHOD(capacity)
{
    PARSE_NONE;
    RETURN_LONG((THIS_DS_DEQUE())->capacity);
}

METHOD(pushAll)
{
    PARSE_ZVAL(values);
    ds_deque_push_all(THIS_DS_DEQUE(), values);
}

METHOD(map)
{
    PARSE_CALLABLE();
    RETURN_DS_DEQUE(ds_deque_map(THIS_DS_DEQUE(), FCI_ARGS));
}

METHOD(merge)
{
    PARSE_ZVAL(values);
    RETURN_DS_DEQUE(ds_deque_merge(THIS_DS_DEQUE(), values));
}

METHOD(reduce)
{
    PARSE_CALLABLE_AND_OPTIONAL_ZVAL(initial);
    ds_deque_reduce(THIS_DS_DEQUE(), initial, return_value, FCI_ARGS);
}

METHOD(filter)
{
    if (ZEND_NUM_ARGS()) {
        PARSE_CALLABLE();
        RETURN_DS_DEQUE(ds_deque_filter_callback(THIS_DS_DEQUE(), FCI_ARGS));
    } else {
        PARSE_NONE;
        RETURN_DS_DEQUE(ds_deque_filter(THIS_DS_DEQUE()));
    }
}

METHOD(slice)
{
    ds_deque_t *deque = THIS_DS_DEQUE();

    if (ZEND_NUM_ARGS() > 1) {
        PARSE_LONG_AND_LONG(index, length);
        RETURN_DS_DEQUE(ds_deque_slice(deque, index, length));
    } else {
        PARSE_LONG(index);
        RETURN_DS_DEQUE(ds_deque_slice(deque, index, deque->size));
    }
}

METHOD(sort)
{
    ds_deque_t *sorted = ds_deque_clone(THIS_DS_DEQUE());

    if (ZEND_NUM_ARGS()) {
        PARSE_COMPARE_CALLABLE();
        ds_deque_sort_callback(sorted);
    } else {
        ds_deque_sort(sorted);
    }

    RETURN_DS_DEQUE(sorted);
}

METHOD(push)
{
    PARSE_VARIADIC_ZVAL();
    if (argc == 1) {
        ds_deque_push(THIS_DS_DEQUE(), argv);
    } else {
        ds_deque_push_va(THIS_DS_DEQUE(), argc, argv);
    }
}

METHOD(unshift)
{
    PARSE_VARIADIC_ZVAL();
    ds_deque_unshift_va(THIS_DS_DEQUE(), argc, argv);
}

METHOD(pop)
{
    PARSE_NONE;
    ds_deque_pop(THIS_DS_DEQUE(), return_value);
}

METHOD(shift)
{
    PARSE_NONE;
    ds_deque_shift(THIS_DS_DEQUE(), return_value);
}

METHOD(first)
{
    PARSE_NONE;
    RETURN_ZVAL_COPY(ds_deque_get_first(THIS_DS_DEQUE()));
}

METHOD(last)
{
    PARSE_NONE;
    RETURN_ZVAL_COPY(ds_deque_get_last(THIS_DS_DEQUE()));
}

METHOD(count)
{
    ds_deque_t *deque = THIS_DS_DEQUE();
    PARSE_NONE;
    RETURN_LONG(deque->size);
}

METHOD(clear)
{
    PARSE_NONE;
    ds_deque_clear(THIS_DS_DEQUE());
}

METHOD(contains)
{
    PARSE_VARIADIC_ZVAL();
    RETURN_BOOL(ds_deque_contains_va(THIS_DS_DEQUE(), argc, argv));
}

METHOD(toArray)
{
    PARSE_NONE;
    ds_deque_to_array(THIS_DS_DEQUE(), return_value);
}

METHOD(get)
{
    PARSE_LONG(index);
    RETURN_ZVAL_COPY(ds_deque_get(THIS_DS_DEQUE(), index));
}

METHOD(set)
{
    PARSE_LONG_AND_ZVAL(index, value);
    ds_deque_set(THIS_DS_DEQUE(), index, value);
}

METHOD(find)
{
    PARSE_ZVAL(value);
    ds_deque_find(THIS_DS_DEQUE(), value, return_value);
}

METHOD(remove)
{
    PARSE_LONG(index);
    ds_deque_remove(THIS_DS_DEQUE(), index, return_value);
}

METHOD(insert)
{
    PARSE_LONG_AND_VARARGS(index);
    ds_deque_insert_va(THIS_DS_DEQUE(), index, argc, argv);
}

METHOD(reverse)
{
    PARSE_NONE;
    RETURN_DS_DEQUE(ds_deque_reversed(THIS_DS_DEQUE()));
}

METHOD(isEmpty)
{
    PARSE_NONE;
    RETURN_BOOL(DS_DEQUE_IS_EMPTY(THIS_DS_DEQUE()));
}

METHOD(copy)
{
    PARSE_NONE;
    RETURN_OBJ(php_ds_deque_create_clone(THIS_DS_DEQUE()));
}

METHOD(jsonSerialize)
{
    PARSE_NONE;
    ds_deque_to_array(THIS_DS_DEQUE(), return_value);
}

void php_ds_register_deque()
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
    php_ds_deque_ce->get_iterator   = php_ds_deque_get_iterator;
    php_ds_deque_ce->serialize      = php_ds_deque_serialize;
    php_ds_deque_ce->unserialize    = php_ds_deque_unserialize;

    zend_declare_class_constant_long(php_ds_deque_ce, STR_AND_LEN("MIN_CAPACITY"), DS_DEQUE_MIN_CAPACITY);
    zend_class_implements(php_ds_deque_ce, 1, sequence_ce);

    php_ds_register_deque_handlers();
}
