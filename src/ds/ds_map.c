#include "../common.h"
#include "../php/classes/php_ce_map.h"
#include "../php/handlers/php_map_handlers.h"
#include "ds_htable.h"
#include "ds_map.h"

static Map *map_init_ex(HTable *table)
{
    Map *map = ecalloc(1, sizeof(Map));
    zend_object_std_init(&map->std, map_ce);

    map->std.handlers = &map_handlers;
    map->table = table;

    return map;
}

Map *map_create()
{
    return map_init_ex(htable_init());
}

zend_object *map_create_object(zend_class_entry *ce)
{
    return &map_create()->std;
}

Map *map_clone(Map *map)
{
    return map_init_ex(htable_clone(map->table));
}

zend_object *map_create_clone(Map *map)
{
    return &map_clone(map)->std;
}

void map_init_zval_ex(zval *obj, Map *map)
{
    ZVAL_OBJ(obj, &map->std);
}

void map_user_allocate(Map *map, zend_long capacity)
{
    htable_ensure_capacity(map->table, capacity);
}

zend_long map_capacity(Map *map)
{
    return map->table->capacity;
}

void map_reverse(Map *map)
{
    htable_reverse(map->table);
}

void map_reversed(Map *map, zval *obj)
{
    HTable *reversed = htable_reversed(map->table);
    map_init_zval_ex(obj, map_init_ex(reversed));
}

void map_put(Map *map, zval *key, zval *value)
{
    htable_put(map->table, key, value);
}

void map_reduce(Map *map, FCI_PARAMS, zval *initial, zval *return_value)
{
    htable_reduce(map->table, FCI_ARGS, initial, return_value);
}

void map_map(Map *map, FCI_PARAMS, zval *return_value)
{
    HTable *table = htable_map(map->table, FCI_ARGS);
    if (table) {
        map_init_zval_ex(return_value, map_init_ex(table));
    }
}

void map_filter_callback(Map *map, FCI_PARAMS, zval *return_value)
{
    HTable *table = htable_filter_callback(map->table, FCI_ARGS);
    if (table) {
        map_init_zval_ex(return_value, map_init_ex(table));
    }
}

zval *map_get(Map *map, zval *key, zval *def)
{
    zval *value = htable_get(map->table, key);

    if (value) {
        return value;
    }

    if (def) {
        return def;
    }

    KEY_NOT_FOUND();
    return NULL;
}

void map_remove(Map *map, zval *key, zval *def, zval *return_value)
{
    int removed = htable_remove(map->table, key, return_value);

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

bool map_has_key(Map *map, zval *key)
{
    return htable_has_key(map->table, key);
}

bool map_has_value(Map *map, zval *value)
{
    return htable_has_value(map->table, value);
}

bool map_has_keys(Map *map, VA_PARAMS)
{
    return htable_has_keys(map->table, argc, argv);
}

bool map_has_values(Map *map, VA_PARAMS)
{
    return htable_has_values(map->table, argc, argv);
}

void map_clear(Map *map)
{
    htable_clear(map->table);
}

void map_sorted_by_value_callback(Map *map, zval *obj)
{
    Map *sorted = map_clone(map);
    htable_sort_callback_by_value(sorted->table);
    map_init_zval_ex(obj, sorted);
}

void map_sorted_by_value(Map *map, zval *obj)
{
    Map *sorted = map_clone(map);
    htable_sort_by_value(sorted->table);
    map_init_zval_ex(obj, sorted);
}

void map_sorted_by_key_callback(Map *map, zval *obj)
{
    Map *sorted = map_clone(map);
    htable_sort_callback_by_key(sorted->table);
    map_init_zval_ex(obj, sorted);
}

void map_sorted_by_key(Map *map, zval *obj)
{
    Map *sorted = map_clone(map);
    htable_sort_by_key(sorted->table);
    map_init_zval_ex(obj, sorted);
}

void map_to_array(Map *map, zval *return_value)
{
    htable_to_array(map->table, return_value);
}

int map_serialize(zval *object, unsigned char **buffer, size_t *length, zend_serialize_data *data)
{
    Map *map = Z_MAP_P(object);
    return htable_serialize(map->table, buffer, length, data);
}

int map_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t length, zend_unserialize_data *data)
{
    Map *map = map_create();
    ZVAL_OBJ(object, &map->std);
    return htable_unserialize(map->table, buffer, length, data);
}

void map_create_key_set(Map *map, zval *obj)
{
    htable_create_key_set(map->table, obj);
}

ds_vector_t *map_values_to_vector(Map *map)
{
    return htable_values_to_vector(map->table);
}

ds_vector_t *map_pairs_to_vector(Map *map)
{
    return htable_pairs_to_vector(map->table);
}

void map_slice(Map *map, zend_long index, zend_long length, zval *obj)
{
    HTable *sliced = htable_slice(map->table, index, length);
    map_init_zval_ex(obj, map_init_ex(sliced));
}

void map_merge(Map *map, zval *values, zval *obj)
{
    if (is_array(values)) {
        Map *merged = map_clone(map);
        map_put_all(merged, values);
        map_init_zval_ex(obj, merged);
        return;
    }

    if (Z_TYPE_P(values) == IS_OBJECT) {
        if (Z_OBJCE_P(values) == map_ce) {
            HTable *merged = htable_merge(map->table, Z_MAP_P(values)->table);
            map_init_zval_ex(obj, map_init_ex(merged));
            return;
        }

        if (Z_OBJCE_P(values) == set_ce) {
            HTable *merged = htable_merge(map->table, Z_SET_P(values)->table);
            map_init_zval_ex(obj, map_init_ex(merged));
            return;
        }

        if (is_traversable(values)) {
            Map *merged = map_clone(map);
            map_put_all(merged, values);
            map_init_zval_ex(obj, merged);
            return;
        }
    }

    ARRAY_OR_TRAVERSABLE_REQUIRED();
}

void map_xor(Map *map, Map *other, zval *obj)
{
    HTable *xor = htable_xor(map->table, other->table);
    map_init_zval_ex(obj, map_init_ex(xor));
}

void map_diff(Map *map, Map *other, zval *obj)
{
    HTable *diff = htable_diff(map->table, other->table);
    map_init_zval_ex(obj, map_init_ex(diff));
}

void map_intersect(Map *map, Map *other, zval *obj)
{
    HTable *intersection = htable_intersect(map->table, other->table);
    map_init_zval_ex(obj, map_init_ex(intersection));
}

void map_first(Map *map, zval *return_value)
{
    HBucket *bucket = htable_first(map->table);

    if ( ! bucket) {
        NOT_ALLOWED_WHEN_EMPTY();
        ZVAL_NULL(return_value);
        return;
    }

    pair_create_as_zval(&bucket->key, &bucket->value, return_value);
}

void map_last(Map *map, zval *return_value)
{
    HBucket *bucket = htable_last(map->table);

    if ( ! bucket) {
        NOT_ALLOWED_WHEN_EMPTY();
        ZVAL_NULL(return_value);
        return;
    }

    pair_create_as_zval(&bucket->key, &bucket->value, return_value);
}

void map_skip(Map *map, zend_long position, zval *return_value)
{
    HBucket *bucket = htable_lookup_by_position(map->table, position);

    if ( ! bucket) {
        INDEX_OUT_OF_RANGE(position, map->table->size);
        ZVAL_NULL(return_value);
        return;
    }

    pair_create_as_zval(&bucket->key, &bucket->value, return_value);
}

static int iterator_add(zend_object_iterator *iterator, void *puser)
{
    Map *map = (Map *) puser;
    zval *value   = iterator->funcs->get_current_data(iterator);

    zval key;
    iterator->funcs->get_current_key(iterator, &key);

    map_put(map, &key, value);

    return ZEND_HASH_APPLY_KEEP;
}

static inline void add_traversable_to_map(Map *map, zval *obj)
{
    spl_iterator_apply(obj, iterator_add, (void*) map);
}

static inline void add_ht_to_map(Map *map, HashTable *ht)
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

        map_put(map, &zkey, value);
    }

    ZEND_HASH_FOREACH_END();
}

void map_put_all(Map *map, zval *values)
{
    if ( ! values) {
        return;
    }

    if (is_array(values)) {
        HashTable *ht = Z_ARRVAL_P(values);
        add_ht_to_map(map, ht);
        return;
    }

    if (is_traversable(values)) {
        add_traversable_to_map(map, values);
        return;
    }

    ARRAY_OR_TRAVERSABLE_REQUIRED();
}
