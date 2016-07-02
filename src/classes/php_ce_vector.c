#include "../common.h"
#include "../php/php_ds_vector.h"
#include "../iterators/php_vector_iterator.h"
#include "../handlers/php_vector_handlers.h"
#include "php_ce_sequence.h"
#include "php_ce_vector.h"

#define METHOD(name) PHP_METHOD(Vector, name)

zend_class_entry *php_ds_vector_ce;

METHOD(__construct)
{
    PARSE_OPTIONAL_ZVAL(values);

    if (values) {
        if (Z_TYPE_P(values) == IS_LONG) {
            ds_vector_user_allocate(THIS_DS_VECTOR(), Z_LVAL_P(values));
        } else {
            ds_vector_push_all(THIS_DS_VECTOR(), values);
        }
    }
}

METHOD(allocate)
{
    PARSE_LONG(capacity);
    ds_vector_user_allocate(THIS_DS_VECTOR(), capacity);
}

METHOD(capacity)
{
    PARSE_NONE;
    RETURN_LONG((THIS_DS_VECTOR())->capacity);
}

METHOD(clear)
{
    PARSE_NONE;
    ds_vector_clear(THIS_DS_VECTOR());
}

METHOD(contains)
{
    PARSE_VARIADIC_ZVAL();
    RETURN_BOOL(ds_vector_contains_va(THIS_DS_VECTOR(), argc, argv));
}

METHOD(copy)
{
    PARSE_NONE;
    RETURN_OBJ(ds_vector_create_clone(THIS_DS_VECTOR()));
}

METHOD(count)
{
    PARSE_NONE;
    RETURN_LONG(DS_VECTOR_SIZE(THIS_DS_VECTOR()));
}

METHOD(filter)
{
    if (ZEND_NUM_ARGS()) {
        PARSE_CALLABLE();
        ds_vector_filter_callback(THIS_DS_VECTOR(), return_value, FCI_ARGS);
    } else {
        ds_vector_filter(THIS_DS_VECTOR(), return_value);
    }
}

METHOD(find)
{
    PARSE_ZVAL(value);
    ds_vector_find(THIS_DS_VECTOR(), value, return_value);
}

METHOD(first)
{
    PARSE_NONE;
    RETURN_ZVAL_COPY(ds_vector_get_first(THIS_DS_VECTOR()));
}

METHOD(get)
{
    PARSE_LONG(index);
    RETURN_ZVAL_COPY(ds_vector_get(THIS_DS_VECTOR(), index));
}

METHOD(insert)
{
    PARSE_LONG_AND_VARARGS(index);
    ds_vector_insert_va(THIS_DS_VECTOR(), index, argc, argv);
}

METHOD(isEmpty)
{
    PARSE_NONE;
    RETURN_BOOL(DS_VECTOR_IS_EMPTY(THIS_DS_VECTOR()));
}

METHOD(join)
{
    if (ZEND_NUM_ARGS()) {
        PARSE_STRING();
        ds_vector_join(THIS_DS_VECTOR(), str, len, return_value);
    } else {
        ds_vector_join(THIS_DS_VECTOR(), NULL, 0, return_value);
    }
}

METHOD(jsonSerialize)
{
    PARSE_NONE;
    ds_vector_to_array(THIS_DS_VECTOR(), return_value);
}

METHOD(last)
{
    PARSE_NONE;
    RETURN_ZVAL_COPY(ds_vector_get_last(THIS_DS_VECTOR()));
}

METHOD(map)
{
    PARSE_CALLABLE();
    ds_vector_map(THIS_DS_VECTOR(), return_value, FCI_ARGS);
}

METHOD(merge)
{
    PARSE_ZVAL(values);
    ds_vector_merge(THIS_DS_VECTOR(), values, return_value);
}

METHOD(pop)
{
    PARSE_NONE;
    ds_vector_pop(THIS_DS_VECTOR(), return_value);
}

METHOD(push)
{
    PARSE_VARIADIC_ZVAL();
    ds_vector_push_va(THIS_DS_VECTOR(), argc, argv);
}

METHOD(pushAll)
{
    PARSE_ZVAL(values);
    ds_vector_push_all(THIS_DS_VECTOR(), values);
}

METHOD(reduce)
{
    PARSE_CALLABLE_AND_OPTIONAL_ZVAL(initial);
    ds_vector_reduce(THIS_DS_VECTOR(), initial, return_value, FCI_ARGS);
}

METHOD(remove)
{
    PARSE_LONG(index);
    ds_vector_remove(THIS_DS_VECTOR(), index, return_value);
}

METHOD(reverse)
{
    PARSE_NONE;
    {
        ds_vector_t *vector = ds_vector_create_copy(THIS_DS_VECTOR());
        ds_vector_reverse(vector);
        RETURN_DS_VECTOR(vector);
    }
}

METHOD(rotate)
{
    PARSE_LONG(rotations);
    ds_vector_rotate(THIS_DS_VECTOR(), rotations);
}

METHOD(set)
{
    PARSE_LONG_AND_ZVAL(index, value);
    ds_vector_set(THIS_DS_VECTOR(), index, value);
}

METHOD(shift)
{
    PARSE_NONE;
    ds_vector_shift(THIS_DS_VECTOR(), return_value);
}

METHOD(slice)
{
    ds_vector_t *vector = THIS_DS_VECTOR();

    if (ZEND_NUM_ARGS() > 1) {
        PARSE_LONG_AND_LONG(index, length);
        ds_vector_slice(vector, index, length, return_value);
    } else {
        PARSE_LONG(index);
        ds_vector_slice(vector, index, vector->size, return_value);
    }
}

METHOD(sort)
{
    ds_vector_t *vector = ds_vector_create_copy(THIS_DS_VECTOR());

    if (ZEND_NUM_ARGS()) {
        PARSE_COMPARE_CALLABLE();
        ds_vector_sort_callback(vector);
    } else {
        ds_vector_sort(vector);
    }

    RETURN_DS_VECTOR(vector);
}

METHOD(toArray)
{
    PARSE_NONE;
    ds_vector_to_array(THIS_DS_VECTOR(), return_value);
}

METHOD(unshift)
{
    PARSE_VARIADIC_ZVAL();
    ds_vector_unshift_va(THIS_DS_VECTOR(), argc, argv);
}

void php_ds_register_vector()
{
    zend_class_entry ce;

    zend_function_entry methods[] = {
        SEQUENCE_ME_LIST(Vector)
        COLLECTION_ME_LIST(Vector)
        PHP_FE_END
    };

    INIT_CLASS_ENTRY(ce, DS_NS(Vector), methods);

    ds_vector_ce = zend_register_internal_class(&ce);
    ds_vector_ce->ce_flags      |= ZEND_ACC_FINAL;
    ds_vector_ce->create_object  = php_ds_vector;
    ds_vector_ce->get_iterator   = php_ds_vector_get_iterator;
    ds_vector_ce->serialize      = ds_vector_serialize;
    ds_vector_ce->unserialize    = ds_vector_unserialize;

    zend_declare_class_constant_long(ds_vector_ce, STR_AND_LEN("MIN_CAPACITY"), DS_VECTOR_MIN_CAPACITY);

    zend_class_implements(ds_vector_ce, 1, sequence_ce);
    php_ds_register_vector_handlers();
}
