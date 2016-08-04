#include "../common.h"

#include "../php/iterators/php_set_iterator.h"
#include "../php/handlers/php_set_handlers.h"
#include "../php/classes/php_set_ce.h"

#include "ds_set.h"
#include "ds_htable.h"

ds_set_t *ds_set_ex(ds_htable_t *table)
{
    ds_set_t *set = ecalloc(1, sizeof(ds_set_t));
    set->table = table;
    return set;
}

ds_set_t *ds_set()
{
    return ds_set_ex(ds_htable());
}

ds_set_t *ds_set_clone(ds_set_t *set)
{
    return ds_set_ex(ds_htable_clone(set->table));
}

void ds_set_allocate(ds_set_t *set, zend_long capacity)
{
    ds_htable_ensure_capacity(set->table, capacity);
}

void ds_set_sort_callback(ds_set_t *set)
{
    ds_htable_sort_callback_by_key(set->table);
}

void ds_set_sort(ds_set_t *set)
{
    ds_htable_sort_by_key(set->table);
}

ds_set_t *ds_set_sorted_callback(ds_set_t *set)
{
    ds_set_t *sorted = ds_set_clone(set);
    ds_set_sort_callback(sorted);
    return sorted;
}

ds_set_t *ds_set_sorted(ds_set_t *set)
{
    ds_set_t *sorted = ds_set_clone(set);
    ds_set_sort(sorted);
    return sorted;
}

static inline void _set_add(ds_set_t *set, zval *value)
{
    ds_htable_bucket_t *b;
    ds_htable_lookup_or_next(set->table, value, &b);
    ZVAL_NULL(&b->value);
}

void ds_set_add(ds_set_t *set, zval *value)
{
    _set_add(set, value);
}

void ds_set_add_va(ds_set_t *set, VA_PARAMS)
{
    for (; argc != 0; argc--) {
        _set_add(set, argv++);
    }
}

static int iterator_add(zend_object_iterator *iterator, void *puser)
{
    _set_add((ds_set_t *) puser, iterator->funcs->get_current_data(iterator));
    return SUCCESS;
}

static inline void add_traversable_to_set(ds_set_t *set, zval *obj)
{
    spl_iterator_apply(obj, iterator_add, set);
}

static inline void add_array_to_set(ds_set_t *set, HashTable *array)
{
    zval *value;
    ZEND_HASH_FOREACH_VAL(array, value) {
        _set_add(set, value);
    }
    ZEND_HASH_FOREACH_END();
}

void ds_set_add_all(ds_set_t *set, zval *values)
{
    if (values == NULL) {
        return;
    }

    if (ds_is_array(values)) {
        add_array_to_set(set, Z_ARRVAL_P(values));
        return;
    }

    if (ds_is_traversable(values)) {
        add_traversable_to_set(set, values);
        return;
    }

    ARRAY_OR_TRAVERSABLE_REQUIRED();
}

static inline bool _set_contains(ds_set_t *set, zval *value)
{
    return ds_htable_has_key(set->table, value);
}

bool ds_set_contains(ds_set_t *set, zval *value)
{
    return _set_contains(set, value);
}

bool ds_set_contains_va(ds_set_t *set, VA_PARAMS)
{
    return ds_htable_has_keys(set->table, argc, argv);
}

static inline void ds_set_remove(ds_set_t *set, zval *value)
{
    ds_htable_remove(set->table, value, NULL);
}

void ds_set_remove_va(ds_set_t *set, VA_PARAMS)
{
    while (argc--) {
        ds_set_remove(set, argv++);
    }
}

zval *ds_set_get(ds_set_t *set, zend_long index)
{
    ds_htable_bucket_t *bucket = ds_htable_lookup_by_position(set->table, index);

    if (bucket) {
        return &bucket->key;
    }

    INDEX_OUT_OF_RANGE(index, set->table->size);
    return NULL;
}

zval *ds_set_get_first(ds_set_t *set)
{
    ds_htable_bucket_t *bucket = ds_htable_lookup_by_position(set->table, 0);

    if ( ! bucket) {
        NOT_ALLOWED_WHEN_EMPTY();
        return NULL;
    }

    return &bucket->key;
}

zval *ds_set_get_last(ds_set_t *set)
{
    ds_htable_bucket_t *bucket = ds_htable_lookup_by_position(set->table, DS_SET_SIZE(set) - 1);

    if ( ! bucket) {
        NOT_ALLOWED_WHEN_EMPTY();
        return NULL;
    }

    return &bucket->key;
}

void ds_set_join(ds_set_t *set, const char *glue, const size_t len, zval *return_value)
{
    zend_string *str = ds_htable_join_keys(set->table, glue, len);
    ZVAL_STR(return_value, str);
}

ds_set_t *ds_set_slice(ds_set_t *set, zend_long index, zend_long length)
{
    return ds_set_ex(ds_htable_slice(set->table, index, length));
}

ds_set_t *ds_set_diff(ds_set_t *set, ds_set_t *other)
{
    return ds_set_ex(ds_htable_diff(set->table, other->table));
}

void ds_set_assign_diff(ds_set_t *set, ds_set_t *other)
{
    zval *value;
    DS_SET_FOREACH(other, value) {
        ds_set_remove(set, value);
    }
    DS_SET_FOREACH_END();
}

ds_set_t *ds_set_intersect(ds_set_t *set, ds_set_t *other)
{
    return ds_set_ex(ds_htable_intersect(set->table, other->table));
}

void ds_set_assign_intersect(ds_set_t *set, ds_set_t *other)
{
    zval *value;
    DS_SET_FOREACH(set, value) {
        if ( ! _set_contains(other, value)) {
            ds_set_remove(set, value);
        }
    }
    DS_SET_FOREACH_END();
}

// Returns a new ds_set_t with buffer in either A or B but not both
ds_set_t *ds_set_xor(ds_set_t *set, ds_set_t *other)
{
    return ds_set_ex(ds_htable_xor(set->table, other->table));
}

// Elements in either A or B but not both
void ds_set_assign_xor(ds_set_t *set, ds_set_t *other)
{
    zval *value;
    ds_set_t *result = ds_set();

    DS_SET_FOREACH(set, value) {
        if (_set_contains(other, value)) {
            ds_set_remove(set, value);
        }
    }
    DS_SET_FOREACH_END();

    DS_SET_FOREACH(other, value) {
        ds_set_remove(set, value);
    }
    DS_SET_FOREACH_END();
}

ds_set_t *ds_set_union(ds_set_t *set, ds_set_t *other)
{
    return ds_set_ex(ds_htable_merge(set->table, other->table));
}

ds_set_t *ds_set_merge(ds_set_t *set, zval *values)
{
    if (values && (ds_is_array(values) || ds_is_traversable(values))) {
        ds_set_t *merged = ds_set_clone(set);
        ds_set_add_all(merged, values);
        return merged;
    }

    ARRAY_OR_TRAVERSABLE_REQUIRED();
    return NULL;
}

void ds_set_assign_union(ds_set_t *set, ds_set_t *other)
{
    zval *value;
    DS_SET_FOREACH(other, value) {
        _set_add(set, value);
    }
    DS_SET_FOREACH_END();
}

void ds_set_clear(ds_set_t *set)
{
    ds_htable_clear(set->table);
}

void ds_set_free(ds_set_t *set)
{
    ds_htable_free(set->table);
    efree(set);
}

void ds_set_reduce(ds_set_t *set, FCI_PARAMS, zval *initial, zval *return_value)
{
    zval *value;
    zval carry;
    zval params[2];

    if (initial == NULL) {
        ZVAL_NULL(&carry);
    } else {
        ZVAL_COPY_VALUE(&carry, initial);
    }

    DS_SET_FOREACH(set, value) {
        ZVAL_COPY_VALUE(&params[0], &carry);
        ZVAL_COPY_VALUE(&params[1], value);

        fci.param_count = 2;
        fci.params      = params;
        fci.retval      = &carry;

        if (zend_call_function(&fci, &fci_cache) == FAILURE || Z_ISUNDEF(carry)) {
            ZVAL_NULL(return_value);
            return;
        }

        Z_TRY_DELREF_P(&carry);
    }
    DS_SET_FOREACH_END();
    ZVAL_COPY(return_value, &carry);
}

ds_set_t *ds_set_filter_callback(ds_set_t *set, FCI_PARAMS)
{
    if (DS_SET_IS_EMPTY(set)) {
        return ds_set();

    } else {
        zval *value;
        zval param;
        zval retval;

        ds_set_t *filtered = ds_set();

        DS_SET_FOREACH(set, value) {
            ZVAL_COPY_VALUE(&param, value);

            fci.param_count = 1;
            fci.params      = &param;
            fci.retval      = &retval;

            if (zend_call_function(&fci, &fci_cache) == FAILURE || Z_ISUNDEF(retval)) {
                ds_set_free(filtered);
                return NULL;
            }

            if (zend_is_true(&retval)) {
                _set_add(filtered, value);
            }

            zval_ptr_dtor(&retval);
        }
        DS_SET_FOREACH_END();

        return filtered;
    }
}

ds_set_t *ds_set_filter(ds_set_t *set)
{
    if (DS_SET_IS_EMPTY(set)) {
        return ds_set();

    } else {
        zval *value;
        ds_set_t *filtered = ds_set();

        DS_SET_FOREACH(set, value) {
            if (zend_is_true(value)) {
                _set_add(filtered, value);
            }
        }
        DS_SET_FOREACH_END();
        return filtered;
    }
}

void ds_set_reverse(ds_set_t *set)
{
    ds_htable_reverse(set->table);
}

ds_set_t *ds_set_reversed(ds_set_t *set)
{
    return ds_set_ex(ds_htable_reversed(set->table));
}

void ds_set_to_array(ds_set_t *set, zval *arr)
{
    zval *value;

    array_init_size(arr, set->table->size);

    DS_HTABLE_FOREACH_KEY(set->table, value) {
        add_next_index_zval(arr, value);
        Z_TRY_ADDREF_P(value);
    }
    DS_HTABLE_FOREACH_END();
}

void ds_set_sum(ds_set_t *set, zval *return_value)
{
    zval *value;

    ZVAL_LONG(return_value, 0);

    DS_SET_FOREACH(set, value) {
        DS_ADD_TO_SUM(value, return_value);
    }
    DS_SET_FOREACH_END();
}
