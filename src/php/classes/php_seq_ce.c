#include "../../common.h"

#include "../parameters.h"
#include "../arginfo.h"

#include "../objects/php_seq.h"
#include "../iterators/php_seq_iterator.h"
#include "../handlers/php_seq_handlers.h"

#include "php_ds_ce_common.h"
#include "php_seq_ce.h"

#define METHOD(name) PHP_METHOD(Seq, name)

#define SEPARATE() ds_seq_separate( \
    &php_ds_seq_fetch_object(Z_OBJ_P(getThis()))->seq)

zend_class_entry *php_ds_seq_ce;

METHOD(__construct)
{
    PARSE_OPTIONAL_ZVAL(values);

    if (values) {
        SEPARATE();
        ds_seq_push_all(THIS_DS_SEQ(), values);
    }
}

METHOD(join)
{
    if (ZEND_NUM_ARGS()) {
        PARSE_STRING();
        ds_seq_join(THIS_DS_SEQ(), str, len, return_value);
    } else {
        ds_seq_join(THIS_DS_SEQ(), NULL, 0, return_value);
    }
}

METHOD(allocate)
{
    PARSE_LONG(capacity);
    if (capacity < 0) {
        CAPACITY_INVALID(capacity);
        return;
    }
    SEPARATE();
    ds_seq_allocate(THIS_DS_SEQ(), capacity);
}

METHOD(apply)
{
    PARSE_CALLABLE();
    SEPARATE();
    ds_seq_apply(THIS_DS_SEQ(), FCI_ARGS);
}

METHOD(capacity)
{
    PARSE_NONE;
    RETURN_LONG((THIS_DS_SEQ())->capacity);
}

METHOD(map)
{
    PARSE_CALLABLE();
    RETURN_DS_SEQ(ds_seq_map(THIS_DS_SEQ(), FCI_ARGS));
}

METHOD(merge)
{
    PARSE_ZVAL(values);
    RETURN_DS_SEQ(ds_seq_merge(THIS_DS_SEQ(), values));
}

METHOD(reduce)
{
    PARSE_CALLABLE_AND_OPTIONAL_ZVAL(initial);
    ds_seq_reduce(THIS_DS_SEQ(), initial, return_value, FCI_ARGS);
}

METHOD(filter)
{
    if (ZEND_NUM_ARGS()) {
        PARSE_CALLABLE();
        RETURN_DS_SEQ(ds_seq_filter_callback(THIS_DS_SEQ(), FCI_ARGS));
    } else {
        PARSE_NONE;
        RETURN_DS_SEQ(ds_seq_filter(THIS_DS_SEQ()));
    }
}

METHOD(slice)
{
    ds_seq_t *seq = THIS_DS_SEQ();

    PARSE_LONG_AND_OPTIONAL_ZVAL(index, length);

    if (ZEND_NUM_ARGS() > 1 && Z_TYPE_P(length) != IS_NULL) {
        if (Z_TYPE_P(length) != IS_LONG) {
            INTEGER_LENGTH_REQUIRED(length);
        } else {
            RETURN_DS_SEQ(ds_seq_slice(seq, index, Z_LVAL_P(length)));
        }
    } else {
        RETURN_DS_SEQ(ds_seq_slice(seq, index, seq->size));
    }
}

METHOD(sort)
{
    SAVE_COMPARE_CALLABLE(saved_fci, saved_fci_cache);
    PARSE_OPTIONAL_COMPARE_CALLABLE();
    SEPARATE();
    ds_seq_t *sorted = THIS_DS_SEQ();

    if (HAS_COMPARE_CALLABLE()) {
        ds_seq_sort_callback(sorted);
    } else {
        ds_seq_sort(sorted);
    }
    RESTORE_COMPARE_CALLABLE(saved_fci, saved_fci_cache);
}

METHOD(sorted)
{
    SAVE_COMPARE_CALLABLE(saved_fci, saved_fci_cache);
    PARSE_OPTIONAL_COMPARE_CALLABLE();
    ds_seq_t *sorted = ds_seq_clone(THIS_DS_SEQ());

    if (HAS_COMPARE_CALLABLE()) {
        ds_seq_sort_callback(sorted);
    } else {
        ds_seq_sort(sorted);
    }

    RESTORE_COMPARE_CALLABLE(saved_fci, saved_fci_cache);
    RETURN_DS_SEQ(sorted);
}

METHOD(push)
{
    PARSE_VARIADIC_ZVAL();
    SEPARATE();
    if (argc == 1) {
        ds_seq_push(THIS_DS_SEQ(), argv);
    } else {
        ds_seq_push_va(THIS_DS_SEQ(), argc, argv);
    }
}

METHOD(unshift)
{
    PARSE_VARIADIC_ZVAL();
    SEPARATE();
    ds_seq_unshift_va(THIS_DS_SEQ(), argc, argv);
}

METHOD(pop)
{
    PARSE_NONE;
    SEPARATE();
    ds_seq_pop_throw(THIS_DS_SEQ(), return_value);
}

METHOD(shift)
{
    PARSE_NONE;
    SEPARATE();
    ds_seq_shift_throw(THIS_DS_SEQ(), return_value);
}

METHOD(first)
{
    PARSE_NONE;
    RETURN_ZVAL_COPY(ds_seq_get_first_throw(THIS_DS_SEQ()));
}

METHOD(last)
{
    PARSE_NONE;
    RETURN_ZVAL_COPY(ds_seq_get_last_throw(THIS_DS_SEQ()));
}

METHOD(count)
{
    ds_seq_t *seq = THIS_DS_SEQ();
    PARSE_NONE;
    RETURN_LONG(seq->size);
}

METHOD(clear)
{
    PARSE_NONE;
    SEPARATE();
    ds_seq_clear(THIS_DS_SEQ());
}

METHOD(contains)
{
    PARSE_VARIADIC_ZVAL();
    RETURN_BOOL(ds_seq_contains_va(THIS_DS_SEQ(), argc, argv));
}

METHOD(sum)
{
    PARSE_NONE;
    ds_seq_sum(THIS_DS_SEQ(), return_value);
}

METHOD(toArray)
{
    PARSE_NONE;
    ds_seq_to_array(THIS_DS_SEQ(), return_value);
}

METHOD(get)
{
    PARSE_LONG(index);
    RETURN_ZVAL_COPY(ds_seq_get(THIS_DS_SEQ(), index));
}

METHOD(set)
{
    PARSE_LONG_AND_ZVAL(index, value);
    SEPARATE();
    ds_seq_set(THIS_DS_SEQ(), index, value);
}

METHOD(find)
{
    PARSE_ZVAL(value);
    ds_seq_find(THIS_DS_SEQ(), value, return_value);
}

METHOD(remove)
{
    PARSE_LONG(index);
    SEPARATE();
    ds_seq_remove(THIS_DS_SEQ(), index, return_value);
}

METHOD(insert)
{
    PARSE_LONG_AND_VARIADIC_ZVAL(index);
    SEPARATE();
    ds_seq_insert_va(THIS_DS_SEQ(), index, argc, argv);
}

METHOD(reverse)
{
    PARSE_NONE;
    SEPARATE();
    ds_seq_reverse(THIS_DS_SEQ());
}

METHOD(reversed)
{
    PARSE_NONE;
    RETURN_DS_SEQ(ds_seq_reversed(THIS_DS_SEQ()));
}

METHOD(rotate)
{
    PARSE_LONG(rotations);
    SEPARATE();
    ds_seq_rotate(THIS_DS_SEQ(), rotations);
}

METHOD(isEmpty)
{
    PARSE_NONE;
    RETURN_BOOL(DS_SEQ_IS_EMPTY(THIS_DS_SEQ()));
}

METHOD(copy)
{
    PARSE_NONE;
    RETURN_OBJ(php_ds_seq_create_clone(THIS_DS_SEQ()));
}

METHOD(jsonSerialize)
{
    PARSE_NONE;
    ds_seq_to_array(THIS_DS_SEQ(), return_value);
}

METHOD(__serialize)
{
    PARSE_NONE;
    ds_seq_to_array(THIS_DS_SEQ(), return_value);
}

METHOD(__unserialize)
{
    PARSE_ZVAL(data);
    SEPARATE();
    ds_seq_push_all(THIS_DS_SEQ(), data);
}

METHOD(getIterator) {
    PARSE_NONE;
    ZVAL_COPY(return_value, getThis());
}

METHOD(offsetExists)
{
    PARSE_LONG(index);
    RETURN_BOOL(ds_seq_isset(THIS_DS_SEQ(), index, false));
}

METHOD(offsetGet)
{
    PARSE_LONG(index);
    RETURN_ZVAL_COPY(ds_seq_get(THIS_DS_SEQ(), index));
}

METHOD(offsetSet)
{
    PARSE_ZVAL_ZVAL(offset, value);
    SEPARATE();

    if (Z_TYPE_P(offset) == IS_NULL) {
        ds_seq_push(THIS_DS_SEQ(), value);
    } else {
        if (Z_TYPE_P(offset) != IS_LONG) {
            INTEGER_INDEX_REQUIRED(offset);
        } else {
            ds_seq_set(THIS_DS_SEQ(), Z_LVAL_P(offset), value);
        }
    }
}

METHOD(offsetUnset)
{
    PARSE_LONG(index);
    SEPARATE();
    ds_seq_remove(THIS_DS_SEQ(), index, return_value);
}

void php_ds_register_seq()
{
    zend_class_entry ce;

    zend_function_entry methods[] = {
        PHP_DS_ME(Seq, __construct)
        PHP_DS_ME(Seq, getIterator)

        PHP_DS_COLLECTION_ME_LIST(Seq)
        PHP_DS_SEQUENCE_ME_LIST(Seq)
        PHP_FE_END
    };

    INIT_CLASS_ENTRY(ce, PHP_DS_NS(Seq), methods);

    php_ds_seq_ce = zend_register_internal_class(&ce);
    php_ds_seq_ce->ce_flags      |= ZEND_ACC_FINAL;
    php_ds_seq_ce->create_object  = php_ds_seq_create_object;
    php_ds_seq_ce->get_iterator   = php_ds_seq_get_iterator;
    php_ds_seq_ce->serialize      = php_ds_seq_serialize;
    php_ds_seq_ce->unserialize    = php_ds_seq_unserialize;

    zend_declare_class_constant_long(php_ds_seq_ce, STR_AND_LEN("MIN_CAPACITY"), DS_SEQ_MIN_CAPACITY);
    zend_class_implements(php_ds_seq_ce, 4,
        spl_ce_Aggregate,
        spl_ce_Countable,
        php_json_serializable_ce,
        zend_ce_arrayaccess);

    php_ds_register_seq_handlers();
}
