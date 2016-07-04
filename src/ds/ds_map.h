#ifndef DS_MAP_H
#define DS_MAP_H

#include "../common.h"
#include "ds_htable.h"
#include "ds_vector.h"

typedef struct _ds_map_t {
    ds_htable_t *table;
} ds_map_t;

#define DS_MAP_SIZE(m) ((m)->table->size)
#define DS_MAP_IS_EMPTY(m) (DS_MAP_SIZE(m) == 0)

ds_map_t *ds_map();
ds_map_t *ds_map_clone(ds_map_t *map);

void ds_map_reverse(ds_map_t *map);
ds_map_t *ds_map_reversed(ds_map_t *map);
zval *ds_map_get(ds_map_t *map, zval *key, zval *def);
void ds_map_put(ds_map_t *map, zval *key, zval *value);
void ds_map_remove(ds_map_t *map, zval *key, zval *def, zval *return_value);
bool ds_map_has_keys(ds_map_t *map, VA_PARAMS);
bool ds_map_has_values(ds_map_t *map, VA_PARAMS);
void ds_map_clear(ds_map_t *map);
void ds_map_to_array(ds_map_t *map, zval *return_value);
void ds_map_put_all(ds_map_t *map, zval *values);
ds_map_t *ds_map_slice(ds_map_t *map, zend_long index, zend_long length);
void ds_map_create_key_set(ds_map_t *map, zval *return_value);

ds_vector_t *ds_map_values_to_vector(ds_map_t *map);
ds_vector_t *ds_map_pairs_to_vector(ds_map_t *map);

ds_map_t *ds_map_map(ds_map_t *map, FCI_PARAMS);
ds_map_t *ds_map_filter_callback(ds_map_t *map, FCI_PARAMS);
void ds_map_reduce(ds_map_t *map, FCI_PARAMS, zval *initial, zval *return_value);

void ds_map_user_allocate(ds_map_t *map, zend_long capacity);
zend_long ds_map_capacity(ds_map_t *map);

ds_map_t *ds_map_sorted_by_value_callback(ds_map_t *map);
ds_map_t *ds_map_sorted_by_value(ds_map_t *map);
ds_map_t *ds_map_sorted_by_key_callback(ds_map_t *map);
ds_map_t *ds_map_sorted_by_key(ds_map_t *map);

ds_map_t *ds_map_merge(ds_map_t *map, zval *values);
ds_map_t *ds_map_xor(ds_map_t *map, ds_map_t *other);
ds_map_t *ds_map_diff(ds_map_t *map, ds_map_t *other);
ds_map_t *ds_map_intersect(ds_map_t *map, ds_map_t *other);

void ds_map_first(ds_map_t *map, zval *return_value);
void ds_map_last(ds_map_t *map, zval *return_value);
void ds_map_skip(ds_map_t *map, zend_long position, zval *return_value);

void ds_map_destroy(ds_map_t *map);

#endif
