#ifndef PHP_DS_MAP_H
#define PHP_DS_MAP_H

#include "../../ds/ds_map.h"

typedef struct _php_ds_map_t {
    ds_map_t    *map;
    zend_object  std;
} php_ds_map_t;

static inline php_ds_map_t *php_ds_map_fetch_object(zend_object *obj) {
	return (php_ds_map_t *)((char*)(obj) - XtOffsetOf(php_ds_map_t, std));
}

#define Z_DS_MAP(z)   (php_ds_map_fetch_object(Z_OBJ(z))->map)
#define Z_DS_MAP_P(z) Z_DS_MAP(*z)
#define THIS_DS_MAP() Z_DS_MAP_P(getThis())

#define ZVAL_DS_MAP(z, map) ZVAL_OBJ(z, php_ds_map_create_object_ex(map))

#define RETURN_DS_MAP(m)                    \
do {                                        \
    ds_map_t *_m = m;                       \
    if (_m) {                               \
        ZVAL_DS_MAP(return_value, _m);      \
    } else {                                \
        ZVAL_NULL(return_value);            \
    }                                       \
    return;                                 \
} while(0)

zend_object *php_ds_map_create_object_ex(ds_map_t *map);
zend_object *php_ds_map_create_object(zend_class_entry *ce);
zend_object *php_ds_map_create_clone(ds_map_t *map);

zval      *ds_map_pairs(ds_map_t *map);
HashTable *ds_map_pairs_to_php_hashtable(ds_map_t *map);

PHP_DS_SERIALIZE_FUNCIONS(php_ds_map);

#endif
