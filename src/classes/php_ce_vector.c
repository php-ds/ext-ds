#include "../common.h"
#include "../internal/php_vector.h"
#include "../iterators/php_vector_iterator.h"
#include "../handlers/php_vector_handlers.h"
#include "php_ce_sequence.h"
#include "php_ce_vector.h"

#define METHOD(name) PHP_METHOD(Vector, name)

zend_class_entry *vector_ce;

METHOD(__construct)
{
    PARSE_OPTIONAL_ZVAL(values);

    if (values) {
        if (Z_TYPE_P(values) == IS_LONG) {
            vector_user_allocate(THIS_VECTOR(), Z_LVAL_P(values));
        } else {
            vector_push_all(THIS_VECTOR(), values);
        }
    }
}

METHOD(allocate)
{
    PARSE_LONG(capacity);
    vector_user_allocate(THIS_VECTOR(), capacity);
}

METHOD(capacity)
{
    PARSE_NONE;
    RETURN_LONG((THIS_VECTOR())->capacity);
}

METHOD(clear)
{
    PARSE_NONE;
    vector_clear(THIS_VECTOR());
}

METHOD(contains)
{
    PARSE_VARIADIC_ZVAL();
    RETURN_BOOL(vector_contains_va(THIS_VECTOR(), argc, argv));
}

METHOD(copy)
{
    PARSE_NONE;
    RETURN_OBJ(vector_create_clone(THIS_VECTOR()));
}

METHOD(count)
{
    PARSE_NONE;
    RETURN_LONG(VECTOR_SIZE(THIS_VECTOR()));
}

METHOD(filter)
{
    if (ZEND_NUM_ARGS()) {
        PARSE_CALLABLE();
        vector_filter_callback(THIS_VECTOR(), return_value, FCI_ARGS);
    } else {
        vector_filter(THIS_VECTOR(), return_value);
    }
}

METHOD(find)
{
    PARSE_ZVAL(value);
    vector_find(THIS_VECTOR(), value, return_value);
}

METHOD(first)
{
    PARSE_NONE;
    RETURN_ZVAL_COPY(vector_get_first(THIS_VECTOR()));
}

METHOD(get)
{
    PARSE_LONG(index);
    RETURN_ZVAL_COPY(vector_get(THIS_VECTOR(), index));
}

METHOD(insert)
{
    PARSE_LONG_AND_VARARGS(index);
    vector_insert_va(THIS_VECTOR(), index, argc, argv);
}

METHOD(isEmpty)
{
    PARSE_NONE;
    RETURN_BOOL(VECTOR_IS_EMPTY(THIS_VECTOR()));
}

METHOD(join)
{
    if (ZEND_NUM_ARGS()) {
        PARSE_STRING();
        vector_join(THIS_VECTOR(), str, len, return_value);
    } else {
        vector_join(THIS_VECTOR(), NULL, 0, return_value);
    }
}

METHOD(jsonSerialize)
{
    PARSE_NONE;
    vector_to_array(THIS_VECTOR(), return_value);
}

METHOD(last)
{
    PARSE_NONE;
    RETURN_ZVAL_COPY(vector_get_last(THIS_VECTOR()));
}

METHOD(map)
{
    PARSE_CALLABLE();
    vector_map(THIS_VECTOR(), return_value, FCI_ARGS);
}

METHOD(merge)
{
    PARSE_ZVAL(values);
    vector_merge(THIS_VECTOR(), values, return_value);
}

METHOD(pop)
{
    PARSE_NONE;
    vector_pop(THIS_VECTOR(), return_value);
}

METHOD(push)
{
    PARSE_VARIADIC_ZVAL();
    vector_push_va(THIS_VECTOR(), argc, argv);
}

METHOD(pushAll)
{
    PARSE_ZVAL(values);
    vector_push_all(THIS_VECTOR(), values);
}

METHOD(reduce)
{
    PARSE_CALLABLE_AND_OPTIONAL_ZVAL(initial);
    vector_reduce(THIS_VECTOR(), initial, return_value, FCI_ARGS);
}

METHOD(remove)
{
    PARSE_LONG(index);
    vector_remove(THIS_VECTOR(), index, return_value);
}

METHOD(reverse)
{
    PARSE_NONE;
    {
        Vector *vector = vector_create_copy(THIS_VECTOR());
        vector_reverse(vector);
        RETURN_VECTOR(vector);
    }
}

METHOD(rotate)
{
    PARSE_LONG(rotations);
    vector_rotate(THIS_VECTOR(), rotations);
}

METHOD(set)
{
    PARSE_LONG_AND_ZVAL(index, value);
    vector_set(THIS_VECTOR(), index, value);
}

METHOD(shift)
{
    PARSE_NONE;
    vector_shift(THIS_VECTOR(), return_value);
}

METHOD(slice)
{
    Vector *vector = THIS_VECTOR();

    if (ZEND_NUM_ARGS() > 1) {
        PARSE_LONG_AND_LONG(index, length);
        vector_slice(vector, index, length, return_value);
    } else {
        PARSE_LONG(index);
        vector_slice(vector, index, vector->size, return_value);
    }
}

METHOD(sort)
{
    Vector *vector = vector_create_copy(THIS_VECTOR());

    if (ZEND_NUM_ARGS()) {
        PARSE_COMPARE_CALLABLE();
        vector_sort_callback(vector);
    } else {
        vector_sort(vector);
    }

    RETURN_VECTOR(vector);
}

METHOD(toArray)
{
    PARSE_NONE;
    vector_to_array(THIS_VECTOR(), return_value);
}

METHOD(unshift)
{
    PARSE_VARIADIC_ZVAL();
    vector_unshift_va(THIS_VECTOR(), argc, argv);
}

void register_vector()
{
    zend_class_entry ce;

    zend_function_entry methods[] = {
        SEQUENCE_ME_LIST(Vector)
        COLLECTION_ME_LIST(Vector)
        PHP_FE_END
    };

    INIT_CLASS_ENTRY(ce, DS_NS(Vector), methods);

    vector_ce = zend_register_internal_class(&ce);
    vector_ce->ce_flags      |= ZEND_ACC_FINAL;
    vector_ce->create_object  = vector_create_object;
    vector_ce->get_iterator   = vector_get_iterator;
    vector_ce->serialize      = vector_serialize;
    vector_ce->unserialize    = vector_unserialize;

    zend_declare_class_constant_long(vector_ce, STR_AND_LEN("MIN_CAPACITY"), VECTOR_MIN_CAPACITY);

    zend_class_implements(vector_ce, 1, sequence_ce);
    register_vector_handlers();
}
