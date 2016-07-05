#ifndef PHP_DS_MAP_H
#define PHP_DS_MAP_H

#include "../../ds/ds_map.h"

typedef struct _php_ds_map_t {
    zend_object  std;
    ds_map_t    *map;
} php_ds_map_t;

#define Z_DS_MAP(z)   (((php_ds_map_t*)(Z_OBJ(z)))->map)
#define Z_DS_MAP_P(z) Z_DS_MAP(*z)
#define THIS_DS_MAP() Z_DS_MAP_P(getThis())

#define ZVAL_DS_MAP(z, map) ZVAL_OBJ(z, php_ds_map_create_object_ex(map))

#define RETURN_DS_MAP(m)                    \
do {                                        \
    if (m) {                                \
        ZVAL_DS_MAP(return_value, m);       \
    } else {                                \
        ZVAL_NULL(return_value);            \
    }                                       \
    return;                                 \
} while(0)

zend_object *php_ds_map_create_object_ex(ds_map_t *map);
zend_object *php_ds_map_create_object(zend_class_entry *ce);
zend_object *php_ds_map_create_clone(ds_map_t *map);

// This can not be handled internally because we have to create new instances
// of php_ds_pair_t, which is outside of the internal scope.
ds_vector_t *php_ds_map_pairs_to_vector(ds_map_t *map);
HashTable *php_ds_map_pairs_to_php_array(ds_map_t *map);

int php_ds_map_serialize(
    zval                    *object,
    unsigned char          **buffer,
    size_t                  *length,
    zend_serialize_data     *data
);

int php_ds_map_unserialize(
    zval                    *object,
    zend_class_entry        *ce,
    const unsigned char     *buffer,
    size_t                   length,
    zend_unserialize_data   *data
);

#endif
