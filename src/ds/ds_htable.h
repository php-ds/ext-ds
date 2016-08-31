#ifndef DS_HTABLE_H
#define DS_HTABLE_H

#include "../common.h"

#define DS_HTABLE_MIN_CAPACITY  16  // Must be a power of 2

/**
 * Marker to indicate an invalid index in the buffer.
 */
#define DS_HTABLE_INVALID_INDEX ((uint32_t) -1)

/**
 * Determines the calculated hash of a bucket, before mod.
 */
#define DS_HTABLE_BUCKET_HASH(_bucket) (Z_NEXT((_bucket)->key))

/**
 * Determines the buffer index of the next bucket in the collision chain.
 * An invalid index indicates that it's the last bucket in the chain.
 */
#define DS_HTABLE_BUCKET_NEXT(_bucket) (Z_NEXT((_bucket)->value))

/**
 * Determines if a bucket has been deleted.
 */
#define DS_HTABLE_BUCKET_DELETED(_bucket) (Z_ISUNDEF((_bucket)->key))

/**
 * Finds the start of the collision chain for a given hash.
 * An invalid index indicates that a chain doesn't exist.
 */
#define DS_HTABLE_BUCKET_LOOKUP(t, h) ((t)->lookup[h & ((t)->capacity - 1)])

/**
 * Determines if a table is packed, ie. doesn't have deleted buckets.
 */
#define DS_HTABLE_IS_PACKED(t) ((t)->size == (t)->next)

/**
 * Rehashes a bucket into a table.
 *
 * 1. Determine where the bucket's chain would start.
 * 2. Set the bucket's next bucket to be the start of the chain.
 * 3. Set the start of the chain to the bucket's position in the buffer.
 *
 * This means that the next bucket can come before another in the buffer,
 * because a rehash unshifts the bucket into the chain.
 */
#define DS_HTABLE_BUCKET_REHASH(table, bucket, mask, idx)   \
do {                                                        \
    ds_htable_bucket_t *_bucket = bucket;                   \
    ds_htable_t        *_table  = table;                    \
                                                            \
    uint32_t  hash = DS_HTABLE_BUCKET_HASH(_bucket);        \
    uint32_t *head = &_table->lookup[hash & (mask)];        \
                                                            \
    DS_HTABLE_BUCKET_NEXT(_bucket) = *head;                 \
    *head = idx;                                            \
} while (0)

/**
 * Copies a bucket's state into another, including: key, value, hash and next.
 */
#define DS_HTABLE_BUCKET_COPY(dst, src)                         \
do {                                                            \
    ds_htable_bucket_t *_src = src;                             \
    ds_htable_bucket_t *_dst = dst;                             \
    ZVAL_COPY(&_dst->key, &_src->key);                          \
    ZVAL_COPY(&_dst->value, &_src->value);                      \
    DS_HTABLE_BUCKET_NEXT(_dst) = DS_HTABLE_BUCKET_NEXT(_src);  \
    DS_HTABLE_BUCKET_HASH(_dst) = DS_HTABLE_BUCKET_HASH(_src);  \
} while (0)

/**
 * Marks a bucket as deleted, destructing both the key and the value.
 */
#define DS_HTABLE_BUCKET_DELETE(b)                          \
    DTOR_AND_UNDEF(&(b)->value);                            \
    DTOR_AND_UNDEF(&(b)->key);                              \
    DS_HTABLE_BUCKET_NEXT((b)) = DS_HTABLE_INVALID_INDEX

#define DS_HTABLE_FOREACH_BUCKET(h, b)                  \
do {                                                    \
    ds_htable_t *_h = h;                                \
    ds_htable_bucket_t *_x = _h->buckets;               \
    ds_htable_bucket_t *_y = _h->buckets + _h->next;    \
    for (; _x < _y; ++_x) {                             \
        if (DS_HTABLE_BUCKET_DELETED(_x)) continue;     \
        b = _x;

#define DS_HTABLE_FOREACH_BUCKET_REVERSED(h, b)         \
do {                                                    \
    ds_htable_t *_h  = h;                               \
    ds_htable_bucket_t *_x = _h->buckets;               \
    ds_htable_bucket_t *_y = _x + _h->next - 1;         \
    for (; _y >= _x; --_y) {                            \
        if (DS_HTABLE_BUCKET_DELETED(_y)) continue;     \
        b = _y;

#define DS_HTABLE_FOREACH(h, i, k, v)                   \
do {                                                    \
    uint32_t _i;                                        \
    uint32_t _n = (h)->size;                            \
    ds_htable_bucket_t *_b = (h)->buckets;              \
                                                        \
    for (_i = 0; _i < _n; ++_b) {                       \
        if (DS_HTABLE_BUCKET_DELETED(_b)) continue;     \
        k = &_b->key;                                   \
        v = &_b->value;                                 \
        i = _i++;

#define DS_HTABLE_FOREACH_KEY(h, k)                     \
do {                                                    \
    ds_htable_t *_h = h;                                \
    ds_htable_bucket_t *_x = _h->buckets;               \
    ds_htable_bucket_t *_y = _h->buckets + _h->next;    \
    for (; _x < _y; ++_x) {                             \
        if (DS_HTABLE_BUCKET_DELETED(_x)) continue;     \
        k = &_x->key;

#define DS_HTABLE_FOREACH_VALUE(h, v)                   \
do {                                                    \
    ds_htable_t *_h = h;                                \
    ds_htable_bucket_t *_x = _h->buckets;               \
    ds_htable_bucket_t *_y = _h->buckets + _h->next;    \
    for (; _x < _y; ++_x) {                             \
        if (DS_HTABLE_BUCKET_DELETED(_x)) continue;     \
        v = &_x->value;

#define DS_HTABLE_FOREACH_KEY_VALUE(h, k, v)    \
do {                                                    \
    ds_htable_t *_h = h;                                \
    ds_htable_bucket_t *_x = _h->buckets;               \
    ds_htable_bucket_t *_y = _h->buckets + _h->next;    \
    for (; _x < _y; ++_x) {                             \
        if (DS_HTABLE_BUCKET_DELETED(_x)) continue;     \
        k = &_x->key;                                   \
        v = &_x->value;

#define DS_HTABLE_FOREACH_END() \
    }                           \
} while (0)

typedef struct _ds_htable_bucket_t {
    zval key;
    zval value;
} ds_htable_bucket_t;

typedef struct _ds_htable_t {
    ds_htable_bucket_t  *buckets;       // Buffer for the buckets
    uint32_t            *lookup;        // Separated hash lookup table
    uint32_t             next;          // Next open index in the bucket buffer
    uint32_t             size;          // Number of active pairs in the table
    uint32_t             capacity;      // Number of buckets in the table
    uint32_t             min_deleted;   // Lowest deleted bucket buffer index
} ds_htable_t;

ds_htable_t *ds_htable();
zval *ds_htable_values(ds_htable_t *table);

void ds_htable_ensure_capacity(ds_htable_t *table, uint32_t capacity);

void ds_htable_sort(ds_htable_t *table, compare_func_t compare_func);
void ds_htable_sort_by_key(ds_htable_t *table);
void ds_htable_sort_by_value(ds_htable_t *table);
void ds_htable_sort_by_pair(ds_htable_t *table);
void ds_htable_sort_callback_by_key(ds_htable_t *table);
void ds_htable_sort_callback_by_value(ds_htable_t *table);

ds_htable_bucket_t *ds_htable_lookup_by_value(ds_htable_t *h, zval *key);
ds_htable_bucket_t *ds_htable_lookup_by_key(ds_htable_t *h, zval *key);
ds_htable_bucket_t *ds_htable_lookup_by_position(ds_htable_t *table, uint32_t position);

bool ds_htable_lookup_or_next(ds_htable_t *table, zval *key, ds_htable_bucket_t **return_value);
bool ds_htable_has_keys(ds_htable_t *h, VA_PARAMS);
bool ds_htable_has_key(ds_htable_t *table, zval *key);
bool ds_htable_has_values(ds_htable_t *h, VA_PARAMS);
bool ds_htable_has_value(ds_htable_t *h, zval *value);
int  ds_htable_remove(ds_htable_t *h, zval *key, zval *return_value);
void ds_htable_put(ds_htable_t *h, zval *key, zval *value);
void ds_htable_to_array(ds_htable_t *h, zval *arr);
void ds_htable_free(ds_htable_t *h);
zval *ds_htable_get(ds_htable_t *h, zval *key);
ds_htable_t *ds_htable_slice(ds_htable_t *table, zend_long index, zend_long length);

void ds_htable_clear(ds_htable_t *h);
ds_htable_t *ds_htable_clone(ds_htable_t *source);
bool ds_htable_isset(ds_htable_t *h, zval *key, bool check_empty);
zend_string *ds_htable_join_keys(ds_htable_t *table, const char* glue, const size_t len);
void ds_htable_reverse(ds_htable_t *table);
ds_htable_t *ds_htable_reversed(ds_htable_t *table);

ds_htable_bucket_t *ds_htable_first(ds_htable_t *table);
ds_htable_bucket_t *ds_htable_last(ds_htable_t *table);

ds_htable_t *ds_htable_map(ds_htable_t *table, FCI_PARAMS);
ds_htable_t *ds_htable_filter(ds_htable_t *table);
ds_htable_t *ds_htable_filter_callback(ds_htable_t *table, FCI_PARAMS);

void ds_htable_apply(ds_htable_t *table, FCI_PARAMS);
void ds_htable_reduce(ds_htable_t *table, FCI_PARAMS, zval *initial, zval *return_value);

ds_htable_t *ds_htable_xor(ds_htable_t *table, ds_htable_t *other);
ds_htable_t *ds_htable_diff(ds_htable_t *table, ds_htable_t *other);
ds_htable_t *ds_htable_intersect(ds_htable_t *table, ds_htable_t *other);
ds_htable_t *ds_htable_merge(ds_htable_t *table, ds_htable_t *other);

int ds_htable_serialize(ds_htable_t *table, unsigned char **buffer, size_t *buf_len, zend_serialize_data *data);
int ds_htable_unserialize(ds_htable_t *table, const unsigned char *buffer, size_t length, zend_unserialize_data *data);

#endif
