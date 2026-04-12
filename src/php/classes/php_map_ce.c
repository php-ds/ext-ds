#include "../../common.h"

#include "../parameters.h"
#include "../arginfo.h"

#include "../objects/php_map.h"
#include "../objects/php_pair.h"
#include "../objects/php_set.h"
#include "../objects/php_seq.h"

#include "../iterators/php_map_iterator.h"
#include "../handlers/php_map_handlers.h"

#include "php_ds_ce_common.h"
#include "php_map_ce.h"

#define METHOD(name) PHP_METHOD(Map, name)

#define SEPARATE() ds_map_separate(THIS_DS_MAP())

zend_class_entry *php_ds_map_ce;

METHOD(__construct)
{
    PARSE_OPTIONAL_ZVAL(values);

    if (values) {
        SEPARATE();
        ds_map_put_all(THIS_DS_MAP(), values);
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
    ds_map_allocate(THIS_DS_MAP(), capacity);
}

METHOD(apply)
{
    PARSE_CALLABLE();
    SEPARATE();
    ds_map_apply(THIS_DS_MAP(), FCI_ARGS);
}

METHOD(capacity)
{
    PARSE_NONE;
    RETURN_LONG(ds_map_capacity(THIS_DS_MAP()));
}

METHOD(put)
{
    PARSE_ZVAL_ZVAL(key, value);
    SEPARATE();
    ds_map_put(THIS_DS_MAP(), key, value);
}

METHOD(putAll)
{
    PARSE_ZVAL(values);
    SEPARATE();
    ds_map_put_all(THIS_DS_MAP(), values);
}

METHOD(get)
{
    PARSE_ZVAL_OPTIONAL_ZVAL(key, def);
    RETURN_ZVAL_COPY(ds_map_get(THIS_DS_MAP(), key, def));
}

METHOD(intersect)
{
    PARSE_OBJ(obj, php_ds_map_ce);
    RETURN_DS_MAP(ds_map_intersect(THIS_DS_MAP(), Z_DS_MAP_P(obj)));
}

METHOD(remove)
{
    PARSE_ZVAL_OPTIONAL_ZVAL(key, def);
    SEPARATE();
    ds_map_remove(THIS_DS_MAP(), key, def, return_value);
}

METHOD(hasKey)
{
    PARSE_ZVAL(key);
    RETURN_BOOL(ds_map_has_key(THIS_DS_MAP(), key));
}

METHOD(hasValue)
{
    PARSE_ZVAL(value);
    RETURN_BOOL(ds_map_has_value(THIS_DS_MAP(), value));
}

METHOD(diff)
{
    PARSE_OBJ(obj, php_ds_map_ce);
    RETURN_DS_MAP(ds_map_diff(THIS_DS_MAP(), Z_DS_MAP_P(obj)));
}

METHOD(clear)
{
    PARSE_NONE;
    SEPARATE();
    ds_map_clear(THIS_DS_MAP());
}

METHOD(sort)
{
    SAVE_COMPARE_CALLABLE(saved_fci, saved_fci_cache);
    PARSE_OPTIONAL_COMPARE_CALLABLE();
    SEPARATE();
    if (HAS_COMPARE_CALLABLE()) {
        ds_map_sort_by_value_callback(THIS_DS_MAP());
    } else {
        ds_map_sort_by_value(THIS_DS_MAP());
    }
    RESTORE_COMPARE_CALLABLE(saved_fci, saved_fci_cache);
}

METHOD(sorted)
{
    SAVE_COMPARE_CALLABLE(saved_fci, saved_fci_cache);
    PARSE_OPTIONAL_COMPARE_CALLABLE();
    if (HAS_COMPARE_CALLABLE()) {
        ds_map_t *result = ds_map_sorted_by_value_callback(THIS_DS_MAP());
        RESTORE_COMPARE_CALLABLE(saved_fci, saved_fci_cache);
        RETURN_DS_MAP(result);
    } else {
        ds_map_t *result = ds_map_sorted_by_value(THIS_DS_MAP());
        RESTORE_COMPARE_CALLABLE(saved_fci, saved_fci_cache);
        RETURN_DS_MAP(result);
    }
}

METHOD(ksort)
{
    SAVE_COMPARE_CALLABLE(saved_fci, saved_fci_cache);
    PARSE_OPTIONAL_COMPARE_CALLABLE();
    SEPARATE();
    if (HAS_COMPARE_CALLABLE()) {
        ds_map_sort_by_key_callback(THIS_DS_MAP());
    } else {
        ds_map_sort_by_key(THIS_DS_MAP());
    }
    RESTORE_COMPARE_CALLABLE(saved_fci, saved_fci_cache);
}

METHOD(ksorted)
{
    SAVE_COMPARE_CALLABLE(saved_fci, saved_fci_cache);
    PARSE_OPTIONAL_COMPARE_CALLABLE();
    if (HAS_COMPARE_CALLABLE()) {
        ds_map_t *result = ds_map_sorted_by_key_callback(THIS_DS_MAP());
        RESTORE_COMPARE_CALLABLE(saved_fci, saved_fci_cache);
        RETURN_DS_MAP(result);
    } else {
        ds_map_t *result = ds_map_sorted_by_key(THIS_DS_MAP());
        RESTORE_COMPARE_CALLABLE(saved_fci, saved_fci_cache);
        RETURN_DS_MAP(result);
    }
}

METHOD(keys)
{
    PARSE_NONE;
    RETURN_DS_SET(ds_set_ex(ds_htable_clone(THIS_DS_MAP()->table)));
}

METHOD(last)
{
    PARSE_NONE;
    RETURN_DS_PAIR(ds_map_last(THIS_DS_MAP()));
}

METHOD(merge)
{
    PARSE_ZVAL(values);
    RETURN_DS_MAP(ds_map_merge(THIS_DS_MAP(), values));
}

METHOD(pairs)
{
    ds_map_t *map = THIS_DS_MAP();
    PARSE_NONE;

    ds_seq_t *seq = ds_seq();
    ds_seq_allocate(seq, DS_MAP_SIZE(map));

    zval *key;
    zval *value;

    DS_HTABLE_FOREACH_KEY_VALUE(map->table, key, value) {
        zval pair;
        ZVAL_DS_PAIR(&pair, php_ds_pair_ex(key, value));
        ds_seq_push(seq, &pair);
        zval_ptr_dtor(&pair);
    }
    DS_HTABLE_FOREACH_END();

    RETURN_DS_SEQ(seq);
}

METHOD(toArray)
{
    PARSE_NONE;
    ds_map_to_array(THIS_DS_MAP(), return_value);
}

METHOD(count)
{
    PARSE_NONE;
    RETURN_LONG(DS_MAP_SIZE(THIS_DS_MAP()));
}

METHOD(isEmpty)
{
    PARSE_NONE;
    RETURN_BOOL(DS_MAP_IS_EMPTY(THIS_DS_MAP()));
}

METHOD(copy)
{
    PARSE_NONE;
    RETURN_OBJ(php_ds_map_create_clone(THIS_DS_MAP()));
}

METHOD(jsonSerialize)
{
    PARSE_NONE;
    ds_map_to_array(THIS_DS_MAP(), return_value);
    convert_to_object(return_value);
}

METHOD(filter)
{
    if (ZEND_NUM_ARGS()) {
        PARSE_CALLABLE();
        RETURN_DS_MAP(ds_map_filter_callback(THIS_DS_MAP(), FCI_ARGS));
    } else {
        RETURN_DS_MAP(ds_map_filter(THIS_DS_MAP()));
    }
}

METHOD(first)
{
    PARSE_NONE;
    RETURN_DS_PAIR(ds_map_first(THIS_DS_MAP()));
}

METHOD(reduce)
{
    PARSE_CALLABLE_AND_OPTIONAL_ZVAL(initial);
    ds_map_reduce(THIS_DS_MAP(), FCI_ARGS, initial, return_value);
}

METHOD(reverse)
{
    PARSE_NONE;
    SEPARATE();
    ds_map_reverse(THIS_DS_MAP());
}

METHOD(reversed)
{
    PARSE_NONE;
    RETURN_DS_MAP(ds_map_reversed(THIS_DS_MAP()));
}

METHOD(skip)
{
    PARSE_LONG(position);
    RETURN_DS_PAIR(ds_map_skip(THIS_DS_MAP(), position));
}

METHOD(map)
{
    PARSE_CALLABLE();
    RETURN_DS_MAP(ds_map_map(THIS_DS_MAP(), FCI_ARGS));
}

METHOD(slice)
{
    ds_map_t *map = THIS_DS_MAP();

    PARSE_LONG_AND_OPTIONAL_ZVAL(index, length);

    if (ZEND_NUM_ARGS() > 1 && Z_TYPE_P(length) != IS_NULL) {
        if (Z_TYPE_P(length) != IS_LONG) {
            INTEGER_LENGTH_REQUIRED(length);
        } else {
            RETURN_DS_MAP(ds_map_slice(map, index, Z_LVAL_P(length)));
        }
    } else {
        RETURN_DS_MAP(ds_map_slice(map, index, DS_MAP_SIZE(map)));
    }
}

METHOD(sum)
{
    PARSE_NONE;
    ds_map_sum(THIS_DS_MAP(), return_value);
}

METHOD(union)
{
    PARSE_OBJ(obj, php_ds_map_ce);
    RETURN_DS_MAP(ds_map_union(THIS_DS_MAP(), Z_DS_MAP_P(obj)));
}

METHOD(values)
{
    ds_map_t *map = THIS_DS_MAP();
    PARSE_NONE;

    ds_seq_t *seq = ds_seq();
    ds_seq_allocate(seq, DS_MAP_SIZE(map));

    zval *value;

    DS_HTABLE_FOREACH_VALUE(map->table, value) {
        ds_seq_push(seq, value);
    }
    DS_HTABLE_FOREACH_END();

    RETURN_DS_SEQ(seq);
}

METHOD(xor)
{
    PARSE_OBJ(obj, php_ds_map_ce);
    RETURN_DS_MAP(ds_map_xor(THIS_DS_MAP(), Z_DS_MAP_P(obj)));
}

METHOD(__serialize)
{
    PARSE_NONE;
    ds_map_t *map = THIS_DS_MAP();

    zval *key;
    zval *value;

    array_init_size(return_value, DS_MAP_SIZE(map));

    DS_HTABLE_FOREACH_KEY_VALUE(map->table, key, value) {
        zval pair;
        array_init_size(&pair, 2);
        Z_TRY_ADDREF_P(key);
        Z_TRY_ADDREF_P(value);
        add_next_index_zval(&pair, key);
        add_next_index_zval(&pair, value);
        add_next_index_zval(return_value, &pair);
    }
    DS_HTABLE_FOREACH_END();
}

METHOD(__unserialize)
{
    PARSE_ZVAL(data);
    SEPARATE();
    ds_map_t *map = THIS_DS_MAP();

    zval *entry;
    ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(data), entry) {
        zval *key = zend_hash_index_find(Z_ARRVAL_P(entry), 0);
        zval *value = zend_hash_index_find(Z_ARRVAL_P(entry), 1);
        if (key && value) {
            ds_map_put(map, key, value);
        }
    } ZEND_HASH_FOREACH_END();
}

METHOD(getIterator) {
    PARSE_NONE;
    ZVAL_COPY(return_value, getThis());
}

METHOD(offsetExists)
{
    PARSE_ZVAL(offset);
    RETURN_BOOL(ds_htable_isset(THIS_DS_MAP()->table, offset, false));
}

METHOD(offsetGet)
{
    PARSE_ZVAL(offset);
    RETURN_ZVAL_COPY(ds_map_get(THIS_DS_MAP(), offset, NULL));
}

METHOD(offsetSet)
{
    PARSE_ZVAL_ZVAL(offset, value);
    SEPARATE();
    ds_map_put(THIS_DS_MAP(), offset, value);
}

METHOD(offsetUnset)
{
    PARSE_ZVAL(offset);
    SEPARATE();
    ds_map_remove(THIS_DS_MAP(), offset, NULL, return_value);
}

void php_ds_register_map()
{
    zend_class_entry ce;

    zend_function_entry methods[] = {
        PHP_DS_ME(Map, __construct)
        PHP_DS_ME(Map, allocate)
        PHP_DS_ME(Map, apply)
        PHP_DS_ME(Map, capacity)
        PHP_DS_ME(Map, diff)
        PHP_DS_ME(Map, filter)
        PHP_DS_ME(Map, first)
        PHP_DS_ME(Map, get)
        PHP_DS_ME(Map, hasKey)
        PHP_DS_ME(Map, hasValue)
        PHP_DS_ME(Map, intersect)
        PHP_DS_ME(Map, keys)
        PHP_DS_ME(Map, ksort)
        PHP_DS_ME(Map, ksorted)
        PHP_DS_ME(Map, last)
        PHP_DS_ME(Map, map)
        PHP_DS_ME(Map, merge)
        PHP_DS_ME(Map, pairs)
        PHP_DS_ME(Map, put)
        PHP_DS_ME(Map, putAll)
        PHP_DS_ME(Map, reduce)
        PHP_DS_ME(Map, remove)
        PHP_DS_ME(Map, reverse)
        PHP_DS_ME(Map, reversed)
        PHP_DS_ME(Map, skip)
        PHP_DS_ME(Map, slice)
        PHP_DS_ME(Map, sort)
        PHP_DS_ME(Map, sorted)
        PHP_DS_ME(Map, sum)
        PHP_DS_ME(Map, union)
        PHP_DS_ME(Map, values)
        PHP_DS_ME(Map, xor)
        PHP_DS_ME(Map, getIterator)

        PHP_DS_ME(Map, offsetExists)
        PHP_DS_ME(Map, offsetGet) 
        PHP_DS_ME(Map, offsetSet) 
        PHP_DS_ME(Map, offsetUnset)

        PHP_DS_COLLECTION_ME_LIST(Map)
        PHP_FE_END
    };

    INIT_CLASS_ENTRY(ce, PHP_DS_NS(Map), methods);

    php_ds_map_ce = zend_register_internal_class(&ce);
    php_ds_map_ce->ce_flags      |= ZEND_ACC_FINAL;
    php_ds_map_ce->create_object  = php_ds_map_create_object;
    php_ds_map_ce->get_iterator   = php_ds_map_get_iterator;
    php_ds_map_ce->serialize      = php_ds_map_serialize;
    php_ds_map_ce->unserialize    = php_ds_map_unserialize;

    zend_declare_class_constant_long(
        php_ds_map_ce,
        STR_AND_LEN("MIN_CAPACITY"),
        DS_HTABLE_MIN_CAPACITY
    );
    
    zend_class_implements(php_ds_map_ce, 4,
        spl_ce_Aggregate,
        spl_ce_Countable,
        php_json_serializable_ce,
        zend_ce_arrayaccess);

    php_ds_register_map_handlers();
}
