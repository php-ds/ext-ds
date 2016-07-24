#ifndef DS_SET_H
#define DS_SET_H

#include "../common.h"
#include "ds_htable.h"

#define DS_SET_SIZE(s)     ((s)->table->size)
#define DS_SET_CAPACITY(s) ((s)->table->capacity)
#define SET_IS_EMPTY(s)    (DS_SET_SIZE(s) == 0)

#define DS_SET_FOREACH(set, value)  DS_HTABLE_FOREACH_KEY(set->table, value)
#define DS_SET_FOREACH_END()        DS_HTABLE_FOREACH_END()

typedef struct _ds_set_t {
    ds_htable_t *table;
} ds_set_t;

ds_set_t *ds_set();
ds_set_t *ds_set_ex(ds_htable_t *table);
ds_set_t *ds_set_clone(ds_set_t *set);

void ds_set_free(ds_set_t *set);
void ds_set_clear(ds_set_t *set);
void ds_set_allocate(ds_set_t *set, zend_long capacity);

void ds_set_add(ds_set_t *set, zval *value);
void ds_set_add_va(ds_set_t *set, VA_PARAMS);
bool ds_set_contains_va(ds_set_t *set, VA_PARAMS);
bool ds_set_contains(ds_set_t *set, zval *value);
void ds_set_remove_va(ds_set_t *set, VA_PARAMS);

void ds_set_to_array(ds_set_t *set, zval *arr);
void ds_set_add_all(ds_set_t *set, zval *value);

zval *ds_set_get(ds_set_t *set, zend_long index);
zval *ds_set_get_first(ds_set_t *set);
zval *ds_set_get_last(ds_set_t *set);

ds_set_t *ds_set_slice(ds_set_t *set, zend_long index, zend_long length);

void ds_set_sort_callback(ds_set_t *set);
void ds_set_sort(ds_set_t *set);

ds_set_t *ds_set_sorted_callback(ds_set_t *set);
ds_set_t *ds_set_sorted(ds_set_t *set);

void ds_set_join (ds_set_t *set, const char *glue, const size_t len, zval *return_value);
void ds_set_reduce(ds_set_t *set, FCI_PARAMS, zval *initial, zval *return_value);

ds_set_t *ds_set_filter_callback(ds_set_t *set, FCI_PARAMS);
ds_set_t *ds_set_filter(ds_set_t *set);

void ds_set_reverse (ds_set_t *set);
ds_set_t *ds_set_reversed(ds_set_t *set);

ds_set_t *ds_set_diff(ds_set_t *set, ds_set_t *other);
ds_set_t *ds_set_intersect(ds_set_t *set, ds_set_t *other);
ds_set_t *ds_set_xor(ds_set_t *set, ds_set_t *other);
ds_set_t *ds_set_union(ds_set_t *set, ds_set_t *other);

void ds_set_assign_diff(ds_set_t *set, ds_set_t *other);
void ds_set_assign_intersect(ds_set_t *set, ds_set_t *other);
void ds_set_assign_xor(ds_set_t *set, ds_set_t *other);
void ds_set_assign_union(ds_set_t *set, ds_set_t *other);

#endif
