#ifndef DS_HEAP_H
#define DS_HEAP_H

#include "../common.h"

#define DS_HEAP_MIN_CAPACITY 8

#define DS_HEAP_SIZE(h)      ((h)->size)
#define DS_HEAP_IS_EMPTY(h)  ((h)->size == 0)

typedef int (*ds_heap_compare_func_t)(zval *a, zval *b);

typedef struct _ds_heap_t {
    zval                    *nodes;
    uint32_t                 capacity;
    uint32_t                 size;
    uint32_t                 refs;    // COW reference count. 0 = sole owner.
} ds_heap_t;

ds_heap_t *ds_heap();
ds_heap_t *ds_heap_clone(ds_heap_t *heap);

void ds_heap_separate(ds_heap_t **heap);
void ds_heap_release(ds_heap_t *heap);

void ds_heap_push(ds_heap_t *heap, zval *value, ds_heap_compare_func_t compare);
void ds_heap_push_all(ds_heap_t *heap, zval *values, ds_heap_compare_func_t compare);
void ds_heap_pop(ds_heap_t *heap, zval *return_value, ds_heap_compare_func_t compare);
zval *ds_heap_peek(ds_heap_t *heap);

void ds_heap_allocate(ds_heap_t *heap, uint32_t capacity);
uint32_t ds_heap_capacity(ds_heap_t *heap);

void ds_heap_to_array(ds_heap_t *heap, zval *array, ds_heap_compare_func_t compare);

void ds_heap_clear(ds_heap_t *heap);
void ds_heap_free(ds_heap_t *heap);

int ds_heap_default_compare(zval *a, zval *b);

#endif
