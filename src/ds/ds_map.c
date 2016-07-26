#include "../common.h"

#include "../php/handlers/php_map_handlers.h"
#include "../php/classes/php_map_ce.h"
#include "../php/classes/php_set_ce.h"

#include "ds_htable.h"
#include "ds_vector.h"
#include "ds_map.h"
#include "ds_set.h"
#include "ds_pair.h"

static ds_map_t *ds_map_ex(ds_htable_t *table)
{
    ds_map_t *map = ecalloc(1, sizeof(ds_map_t));
    map->table = table;

    return map;
}

ds_map_t *ds_map()
{
    return ds_map_ex(ds_htable());
}

ds_map_t *ds_map_clone(ds_map_t *map)
{
    return ds_map_ex(ds_htable_clone(map->table));
}

void ds_map_allocate(ds_map_t *map, zend_long capacity)
{
    ds_htable_ensure_capacity(map->table, capacity);
}

zend_long ds_map_capacity(ds_map_t *map)
{
    return map->table->capacity;
}

void ds_map_reverse(ds_map_t *map)
{
    ds_htable_reverse(map->table);
}

ds_map_t *ds_map_reversed(ds_map_t *map)
{
    return ds_map_ex(ds_htable_reversed(map->table));
}

void ds_map_put(ds_map_t *map, zval *key, zval *value)
{
    ds_htable_put(map->table, key, value);
}

void ds_map_reduce(ds_map_t *map, FCI_PARAMS, zval *initial, zval *return_value)
{
    ds_htable_reduce(map->table, FCI_ARGS, initial, return_value);
}

ds_map_t *ds_map_map(ds_map_t *map, FCI_PARAMS)
{
    ds_htable_t *table = ds_htable_map(map->table, FCI_ARGS);

    if (table) {
        return ds_map_ex(table);
    }

    return NULL;
}

ds_map_t *ds_map_filter_callback(ds_map_t *map, FCI_PARAMS)
{
    ds_htable_t *table = ds_htable_filter_callback(map->table, FCI_ARGS);

    if (table) {
        return ds_map_ex(table);
    }

    return NULL;
}

zval *ds_map_get(ds_map_t *map, zval *key, zval *def)
{
    zval *value = ds_htable_get(map->table, key);

    if (value) {
        return value;
    }

    if (def) {
        return def;
    }

    KEY_NOT_FOUND();
    return NULL;
}

void ds_map_remove(ds_map_t *map, zval *key, zval *def, zval *return_value)
{
    int removed = ds_htable_remove(map->table, key, return_value);

    if (removed == FAILURE) {
        // Failed to remove value

        if ( ! def) {
            // Did not specify a default value
            KEY_NOT_FOUND();
            ZVAL_NULL(return_value);
            return;
        }

        // Default value was provided
        ZVAL_COPY(return_value, def);
    }
}

bool ds_map_has_key(ds_map_t *map, zval *key)
{
    return ds_htable_has_key(map->table, key);
}

bool ds_map_has_value(ds_map_t *map, zval *value)
{
    return ds_htable_has_value(map->table, value);
}

bool ds_map_has_keys(ds_map_t *map, VA_PARAMS)
{
    return ds_htable_has_keys(map->table, argc, argv);
}

bool ds_map_has_values(ds_map_t *map, VA_PARAMS)
{
    return ds_htable_has_values(map->table, argc, argv);
}

void ds_map_clear(ds_map_t *map)
{
    ds_htable_clear(map->table);
}

void ds_map_sort_by_value_callback(ds_map_t *map)
{
    ds_htable_sort_callback_by_value(map->table);
}

void ds_map_sort_by_value(ds_map_t *map)
{
    ds_htable_sort_by_value(map->table);
}

void ds_map_sort_by_key_callback(ds_map_t *map)
{
    ds_htable_sort_callback_by_key(map->table);
}

void ds_map_sort_by_key(ds_map_t *map)
{
    ds_htable_sort_by_key(map->table);
}

ds_map_t *ds_map_sorted_by_value_callback(ds_map_t *map)
{
    ds_map_t *sorted = ds_map_clone(map);
    ds_htable_sort_callback_by_value(sorted->table);
    return sorted;
}

ds_map_t *ds_map_sorted_by_value(ds_map_t *map)
{
    ds_map_t *sorted = ds_map_clone(map);
    ds_htable_sort_by_value(sorted->table);
    return sorted;
}

ds_map_t *ds_map_sorted_by_key_callback(ds_map_t *map)
{
    ds_map_t *sorted = ds_map_clone(map);
    ds_htable_sort_callback_by_key(sorted->table);
    return sorted;
}

ds_map_t *ds_map_sorted_by_key(ds_map_t *map)
{
    ds_map_t *sorted = ds_map_clone(map);
    ds_htable_sort_by_key(sorted->table);
    return sorted;
}

void ds_map_to_array(ds_map_t *map, zval *return_value)
{
    ds_htable_to_array(map->table, return_value);
}

ds_vector_t *ds_map_values_to_vector(ds_map_t *map)
{
    return ds_htable_values_to_vector(map->table);
}

ds_map_t *ds_map_slice(ds_map_t *map, zend_long index, zend_long length)
{
    return ds_map_ex(ds_htable_slice(map->table, index, length));
}

ds_map_t *ds_map_merge(ds_map_t *map, zval *values)
{
    if (ds_is_array(values) || ds_is_traversable(values)) {
        ds_map_t *merged = ds_map_clone(map);
        ds_map_put_all(merged, values);
        return merged;
    }

    ARRAY_OR_TRAVERSABLE_REQUIRED();
    return NULL;
}

ds_map_t *ds_map_xor(ds_map_t *map, ds_map_t *other)
{
    return ds_map_ex(ds_htable_xor(map->table, other->table));
}

ds_map_t *ds_map_diff(ds_map_t *map, ds_map_t *other)
{
    return ds_map_ex(ds_htable_diff(map->table, other->table));
}

ds_map_t *ds_map_intersect(ds_map_t *map, ds_map_t *other)
{
    return ds_map_ex(ds_htable_intersect(map->table, other->table));
}

ds_map_t *ds_map_union(ds_map_t *map, ds_map_t *other)
{
    return ds_map_ex(ds_htable_merge(map->table, other->table));
}

ds_pair_t *ds_map_first(ds_map_t *map)
{
    ds_htable_bucket_t *bucket = ds_htable_first(map->table);

    if ( ! bucket) {
        NOT_ALLOWED_WHEN_EMPTY();
        return NULL;
    }

    return ds_pair_ex(&bucket->key, &bucket->value);
}

ds_pair_t *ds_map_last(ds_map_t *map)
{
    ds_htable_bucket_t *bucket = ds_htable_last(map->table);

    if ( ! bucket) {
        NOT_ALLOWED_WHEN_EMPTY();
        return NULL;
    }

    return ds_pair_ex(&bucket->key, &bucket->value);
}

ds_pair_t *ds_map_skip(ds_map_t *map, zend_long position)
{
    ds_htable_bucket_t *bucket = ds_htable_lookup_by_position(map->table, position);

    if ( ! bucket) {
        INDEX_OUT_OF_RANGE(position, map->table->size);
        return NULL;
    }

    return ds_pair_ex(&bucket->key, &bucket->value);
}

static int iterator_add(zend_object_iterator *iterator, void *puser)
{
    ds_map_t *map = (ds_map_t *) puser;
    zval *value   = iterator->funcs->get_current_data(iterator);

    zval key;
    iterator->funcs->get_current_key(iterator, &key);

    ds_map_put(map, &key, value);

    return ZEND_HASH_APPLY_KEEP;
}

static inline void add_traversable_to_map(ds_map_t *map, zval *obj)
{
    spl_iterator_apply(obj, iterator_add, (void*) map);
}

static inline void add_ht_to_map(ds_map_t *map, HashTable *ht)
{
    uint32_t index;
    zend_string *key;
    zval zkey;
    zval *value;

    ZEND_HASH_FOREACH_KEY_VAL(ht, index, key, value) {
        if (key) {
            ZVAL_STR(&zkey, key);
        } else {
            ZVAL_LONG(&zkey, index);
        }

        ds_map_put(map, &zkey, value);
    }

    ZEND_HASH_FOREACH_END();
}

void ds_map_put_all(ds_map_t *map, zval *values)
{
    if ( ! values) {
        return;
    }

    if (ds_is_array(values)) {
        HashTable *ht = Z_ARRVAL_P(values);
        add_ht_to_map(map, ht);
        return;
    }

    if (ds_is_traversable(values)) {
        add_traversable_to_map(map, values);
        return;
    }

    ARRAY_OR_TRAVERSABLE_REQUIRED();
}

void ds_map_sum(ds_map_t *map, zval *return_value)
{
    zval *value;

    ZVAL_LONG(return_value, 0);

    DS_HTABLE_FOREACH_VALUE(map->table, value) {
        DS_ADD_TO_SUM(value, return_value);
    }
    DS_HTABLE_FOREACH_END();
}

void ds_map_destroy(ds_map_t *map)
{
    ds_htable_destroy(map->table);
    efree(map);
}
