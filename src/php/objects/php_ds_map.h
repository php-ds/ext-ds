#ifndef PHP_DS_MAP_H
#define PHP_DS_MAP_H

#include "../../ds/ds_map.h"

#define Z_DS_MAP(z)   (((php_ds_map_t*)(Z_OBJ(z)))->map)
#define Z_DS_MAP_P(z) Z_DS_MAP(*z)
#define ZVAL_DS_MAP(z, map) ZVAL_OBJ(z, php_ds_map_create_object_ex(map))

typedef struct _php_ds_map_t {
    zend_object  std;
    ds_map_t    *map;
} php_ds_map_t;

zend_object *php_ds_map_create_object_ex(ds_map_t *map);
zend_object *php_ds_map_create_object(zend_class_entry *ce);
zend_object *php_ds_map_create_clone(ds_map_t *map);

zval      *ds_map_pairs(ds_map_t *map);
HashTable *ds_map_pairs_to_php_hashtable(ds_map_t *map);

PHP_DS_SERIALIZE_FUNCIONS(php_ds_map);

#endif
