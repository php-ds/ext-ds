// #include "../iterators/php_map_iterator.h"
#include "../handlers/php_map_handlers.h"
#include "../classes/php_ce_map.h"

#include "php_ds_map.h"

zend_object *php_ds_map_create_object_ex(ds_map_t *map)
{
    php_ds_map_t *obj = ecalloc(1, sizeof(php_ds_map_t));
    zend_object_std_init(&obj->std, php_ds_map_ce);
    obj->std.handlers = &php_ds_map_handlers;
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

int php_ds_map_serialize(zval *object, unsigned char **buffer, size_t *length, zend_serialize_data *data)
{
    ds_map_t *map = Z_DS_MAP_P(object);
    return ds_htable_serialize(map->table, buffer, length, data);
}

int php_ds_map_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t length, zend_unserialize_data *data)
{
    ds_map_t *map = ds_map();
    ZVAL_DS_MAP(object, map);
    return ds_htable_unserialize(map->table, buffer, length, data);
}
