#include "../handlers/php_map_handlers.h"
#include "../classes/php_map_ce.h"

#include "php_map.h"
#include "php_pair.h"

zend_object *php_ds_map_create_object_ex(ds_map_t *map)
{
    php_ds_map_t *obj = ecalloc(1, sizeof(php_ds_map_t));
    zend_object_std_init(&obj->std, php_ds_map_ce);
    obj->std.handlers = &php_map_handlers;
    obj->map = map;
    return &obj->std;
}

zend_object *php_ds_map_create_object(zend_class_entry *ce)
{
    return php_ds_map_create_object_ex(ds_map());
}

zend_object *php_ds_map_create_clone(ds_map_t *map)
{
    return php_ds_map_create_object_ex(ds_map_clone(map));
}

HashTable *ds_map_pairs_to_php_hashtable(ds_map_t *map)
{
    HashTable *array;

    zval *key;
    zval *value;

    zval pair;

    ALLOC_HASHTABLE(array);
    zend_hash_init(array, DS_MAP_SIZE(map), NULL, ZVAL_PTR_DTOR, 0);

    DS_HTABLE_FOREACH_KEY_VALUE(map->table, key, value) {
        ZVAL_DS_PAIR(&pair, ds_pair_ex(key, value));
        zend_hash_next_index_insert(array, &pair);
    }
    DS_HTABLE_FOREACH_END();

    return array;
}

zval *ds_map_pairs(ds_map_t *map)
{
    zval *buffer = ds_allocate_zval_buffer(DS_MAP_SIZE(map));
    zval *target = buffer;

    zval *key;
    zval *value;

    DS_HTABLE_FOREACH_KEY_VALUE(map->table, key, value) {
        ZVAL_DS_PAIR(target++, ds_pair_ex(key, value));
    }
    DS_HTABLE_FOREACH_END();

    return buffer;
}

int php_ds_map_serialize(zval *object, unsigned char **buffer, size_t *length, zend_serialize_data *data)
{
    return ds_htable_serialize(Z_DS_MAP_P(object)->table, buffer, length, data);
}

int php_ds_map_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t length, zend_unserialize_data *data)
{
    ds_map_t *map = ds_map();

    if (ds_htable_unserialize(map->table, buffer, length, data) == FAILURE) {
        ds_map_free(map);
        return FAILURE;
    }

    ZVAL_DS_MAP(object, map);
    return SUCCESS;
}
