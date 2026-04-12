#ifndef DS_SEQ_H
#define DS_SEQ_H

#include "../common.h"

#define DS_SEQ_MIN_CAPACITY 8 // Must be a power of 2

#define DS_SEQ_SIZE(d)      ((d)->size)
#define DS_SEQ_IS_EMPTY(d)  ((d)->size == 0)

#define DS_SEQ_FOREACH(d, v)                              \
do {                                                        \
    const ds_seq_t *_seq = d;                           \
    const zend_long _mask    = _seq->capacity - 1;        \
    const zend_long _size    = _seq->size;                \
    const zend_long _head    = _seq->head;                \
                                                            \
    zend_long _i;                                           \
    for (_i = 0; _i < _size; _i++) {                        \
        v = &_seq->buffer[(_head + _i) & _mask];

#define DS_SEQ_FOREACH_END() \
    } \
} while (0)

typedef struct _ds_seq_t {
    zval      *buffer;
    zend_long  capacity;
    zend_long  head;
    zend_long  tail;
    zend_long  size;
    uint32_t   refs;    // COW reference count. 0 = sole owner.
} ds_seq_t;

ds_seq_t *ds_seq();
ds_seq_t *ds_seq_ex(zend_long capacity);
ds_seq_t *ds_seq_clone(ds_seq_t *src);

void ds_seq_separate(ds_seq_t **seq);
void ds_seq_release(ds_seq_t *seq);

void ds_seq_clear(ds_seq_t *seq);
void ds_seq_free(ds_seq_t *seq);
void ds_seq_allocate(ds_seq_t *seq, zend_long capacity);
void ds_seq_reset_head(ds_seq_t *seq);

void ds_seq_push(ds_seq_t *seq, zval *value);
void ds_seq_push_va(ds_seq_t *seq, VA_PARAMS);
void ds_seq_push_all(ds_seq_t *seq, zval *values);

void ds_seq_set(ds_seq_t *seq, zend_long index, zval *value);
void ds_seq_pop(ds_seq_t *seq, zval *return_value);
void ds_seq_pop_throw(ds_seq_t *seq, zval *return_value);
void ds_seq_shift(ds_seq_t *seq, zval *return_value);
void ds_seq_shift_throw(ds_seq_t *seq, zval *return_value);
void ds_seq_find(ds_seq_t *seq, zval *value, zval *return_value);
void ds_seq_remove(ds_seq_t *seq, zend_long index, zval *return_value);
void ds_seq_insert_va(ds_seq_t *seq, zend_long index, VA_PARAMS);
void ds_seq_unshift_va(ds_seq_t *seq, VA_PARAMS);

zval *ds_seq_get(ds_seq_t *seq, zend_long index);
zval *ds_seq_get_last(ds_seq_t *seq);
zval *ds_seq_get_last_throw(ds_seq_t *seq);
zval *ds_seq_get_first(ds_seq_t *seq);
zval *ds_seq_get_first_throw(ds_seq_t *seq);

bool ds_seq_contains_va(ds_seq_t *seq, VA_PARAMS);
bool ds_seq_isset(ds_seq_t *seq, zend_long index, int check_empty);
bool ds_seq_index_exists(ds_seq_t *seq, zend_long index);

ds_seq_t *ds_seq_map(ds_seq_t *seq, FCI_PARAMS);
ds_seq_t *ds_seq_filter(ds_seq_t *seq);
ds_seq_t *ds_seq_filter_callback(ds_seq_t *seq, FCI_PARAMS);
ds_seq_t *ds_seq_slice(ds_seq_t *seq, zend_long index, zend_long length);
ds_seq_t *ds_seq_merge(ds_seq_t *seq, zval *values);
ds_seq_t *ds_seq_reversed(ds_seq_t *seq);

void ds_seq_join(ds_seq_t *seq, char *str, size_t len, zval *return_value);
void ds_seq_reduce(ds_seq_t *seq, zval *initial, zval *return_value, FCI_PARAMS);
void ds_seq_rotate(ds_seq_t *seq, zend_long rotations);
void ds_seq_sort_callback(ds_seq_t *seq);
void ds_seq_sort(ds_seq_t *seq);
void ds_seq_reverse(ds_seq_t *seq);
void ds_seq_to_array(ds_seq_t *seq, zval *return_value);
void ds_seq_apply(ds_seq_t *seq, FCI_PARAMS);
void ds_seq_sum(ds_seq_t *seq, zval *return_value);

#endif
