#ifndef DS_MAP_H
#define DS_MAP_H

#include "../common.h"
#include "ds_htable.h"
#include "ds_vector.h"

typedef struct _Map {
    zend_object std;
    HTable     *table;
    zend_long   position;
} Map;

#define MAP_SIZE(m) ((m)->table->size)
#define MAP_IS_EMPTY(m) (MAP_SIZE(m) == 0)

#define Z_MAP(z)   ((Map*)(Z_OBJ(z)))
#define Z_MAP_P(z) Z_MAP(*z)
#define THIS_MAP() Z_MAP_P(getThis())

#define ZVAL_MAP(z, map) ZVAL_OBJ(z, &map->std)

#define RETURN_MAP(map) \
do { \
    ZVAL_MAP(return_value, map); \
    return; \
} while(0)

Map *map_create();
void map_init_zval_ex(zval *obj, Map *map);

zend_object *map_create_object(zend_class_entry *ce);
zend_object *map_create_clone(Map *map);

Map *map_clone(Map *map);
void map_reverse(Map *map);
void map_reversed(Map *map, zval *obj);
zval *map_get(Map *map, zval *key, zval *def);
void map_put(Map *map, zval *key, zval *value);
void map_remove(Map *map, zval *key, zval *def, zval *return_value);
bool map_has_keys(Map *map, VA_PARAMS);
bool map_has_values(Map *map, VA_PARAMS);
void map_clear(Map *map);
void map_to_array(Map *map, zval *return_value);
void map_put_all(Map *map, zval *values);
void map_slice(Map *map, zend_long index, zend_long length, zval *obj);
void map_create_key_set(Map *map, zval *return_value);
ds_vector_t *map_values_to_vector(Map *map);
ds_vector_t *map_pairs_to_vector(Map *map);

void map_map(Map *map, FCI_PARAMS, zval *return_value);
void map_reduce(Map *map, FCI_PARAMS, zval *initial, zval *return_value);
void map_filter_callback(Map *map, FCI_PARAMS, zval *return_value);
void map_user_allocate(Map *map, zend_long capacity);
zend_long map_capacity(Map *map);
void map_sort_callback(Map *map);
void map_sort(Map *map);
void map_sorted_callback(Map *map, zval *obj);
void map_sorted(Map *map, zval *obj);

int map_serialize(zval *object, unsigned char **buffer, size_t *buf_len, zend_serialize_data *data);
int map_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t length, zend_unserialize_data *data);


#endif
