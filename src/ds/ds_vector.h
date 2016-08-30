#ifndef DS_VECTOR_H
#define DS_VECTOR_H

#include "../common.h"
#include "ds_map.h"

typedef struct ds_vector {
    zval       *buffer;
    zend_long   capacity;  // Buffer length
    zend_long   size;      // Number of values in the buffer
} ds_vector_t;

#define DS_VECTOR_MIN_CAPACITY  8  // Does not have to be a power of 2

#define DS_VECTOR_SIZE(v)     ((v)->size)
#define DS_VECTOR_IS_EMPTY(v) (DS_VECTOR_SIZE(v) == 0)

#define DS_VECTOR_FOREACH(v, z)             \
do {                                        \
    ds_vector_t *_v = v;                    \
                                            \
    zval *x = _v->buffer;                   \
    zval *y = _v->buffer + _v->size;        \
    for (; x < y; ++x) {                    \
        z = x;

#define DS_VECTOR_FOREACH_REVERSED(v, z)    \
do {                                        \
    ds_vector_t *_v = v;                    \
                                            \
    zval *x = _v->buffer;                   \
    zval *y = _v->buffer + _v->size - 1;    \
    for (; x <= y; --y) {                   \
        z = y;

/**
 * Call this after DS_VECTOR_FOREACH*
 */
#define DS_VECTOR_FOREACH_END() \
    } \
} while (0)

ds_vector_t *ds_vector_clone(ds_vector_t *src);
ds_vector_t *ds_vector();
ds_vector_t *ds_vector_ex(zend_long capacity);
ds_vector_t *ds_vector_from_buffer(zval *buffer, zend_long size);

void ds_vector_allocate(ds_vector_t *vector, zend_long capacity);

void ds_vector_clear(ds_vector_t *vector);
void ds_vector_free(ds_vector_t *vector);

void ds_vector_set(ds_vector_t *vector, zend_long index, zval *value);
void ds_vector_pop(ds_vector_t *vector, zval *return_value);
void ds_vector_pop_throw(ds_vector_t *vector, zval *return_value);
void ds_vector_shift(ds_vector_t *vector, zval *return_value);
void ds_vector_shift_throw(ds_vector_t *vector, zval *return_value);
void ds_vector_find(ds_vector_t *vector, zval *value, zval *return_value);
void ds_vector_remove(ds_vector_t *vector, zend_long index, zval *return_value);

void ds_vector_insert(ds_vector_t *vector, zend_long index, zval *value);
void ds_vector_insert_va(ds_vector_t *vector, zend_long index, VA_PARAMS);

void ds_vector_unshift(ds_vector_t *vector, zval *value);
void ds_vector_unshift_va(ds_vector_t *vector, VA_PARAMS);

bool ds_vector_contains(ds_vector_t *vector, zval *value);
bool ds_vector_contains_va(ds_vector_t *vector, VA_PARAMS);

void ds_vector_push(ds_vector_t *vector, zval *value);
void ds_vector_push_va(ds_vector_t *vector, VA_PARAMS);
void ds_vector_push_all(ds_vector_t *vector, zval *values);

zval *ds_vector_get(ds_vector_t *vector, zend_long index);
zval *ds_vector_get_last(ds_vector_t *vector);
zval *ds_vector_get_first(ds_vector_t *vector);
zval *ds_vector_get_last_throw(ds_vector_t *vector);
zval *ds_vector_get_first_throw(ds_vector_t *vector);

ds_vector_t *ds_vector_map(ds_vector_t *vector, FCI_PARAMS);
ds_vector_t *ds_vector_slice(ds_vector_t *vector, zend_long index, zend_long length);
ds_vector_t *ds_vector_filter(ds_vector_t *vector);
ds_vector_t *ds_vector_filter_callback(ds_vector_t *vector, FCI_PARAMS);
ds_vector_t *ds_vector_merge(ds_vector_t *vector, zval *values);
ds_vector_t *ds_vector_reversed(ds_vector_t *vector);
ds_vector_t *ds_vector_pluck(ds_vector_t *vector, zval *key);
ds_map_t    *ds_vector_group_by(ds_vector_t *vector, zval *iteratee);

bool ds_vector_each(ds_vector_t *vector, FCI_PARAMS);
void ds_vector_reduce(ds_vector_t *vector, zval *initial, zval *return_value, FCI_PARAMS);
void ds_vector_reverse(ds_vector_t *vector);
void ds_vector_rotate(ds_vector_t *vector, zend_long rotations);
void ds_vector_join(ds_vector_t *vector, char *str, size_t len, zval *return_value);
void ds_vector_apply(ds_vector_t *vector, FCI_PARAMS);

zend_long ds_vector_partition(ds_vector_t *vector);
zend_long ds_vector_partition_callback(ds_vector_t *vector, FCI_PARAMS);

void ds_vector_sum(ds_vector_t *vector, zval *return_value);

void ds_vector_sort(ds_vector_t *vector);
void ds_vector_sort_callback(ds_vector_t *vector);

void ds_vector_to_array(ds_vector_t *vector, zval *return_value);

bool ds_vector_index_exists(ds_vector_t *vector, zend_long index);
bool ds_vector_isset(ds_vector_t *vector, zend_long index, int check_empty);

#endif
