#ifndef PHP_DS_HTABLE_H
#define PHP_DS_HTABLE_H

#include <stdbool.h>
#include "php.h"
#include "zend_exceptions.h"
#include "zend_interfaces.h"
#include "ext/spl/spl_iterators.h"

typedef struct _HBucket {
    zval         key;
    zval         value;
} HBucket;

typedef struct _HTable {
    HBucket  *buckets;
    uint32_t *lookup;
    uint32_t  next;
    uint32_t  size;
    uint32_t  capacity;
    uint32_t  min_deleted;
} HTable;

#define HTABLE_MIN_CAPACITY        8
#define INVALID_INDEX              ((uint32_t) -1)

#define BUCKET_HASH(b)        (Z_NEXT((b)->key))
#define NEXT_BUCKET(b)        (Z_NEXT((b)->value))
#define BUCKET_DELETED(b)     (Z_ISUNDEF((b)->key))
#define BUCKET_NOT_DELETED(b) (!BUCKET_DELETED(b))
#define BUCKET_LOOKUP(t, h)   ((t)->lookup[h & ((t)->capacity - 1)])
#define HTABLE_IS_PACKED(t)   ((t)->size == (t)->next)

#define REHASH_BUCKET(table, bucket, mask, index) \
do { \
    uint32_t *_pos = &table->lookup[BUCKET_HASH(bucket) & mask]; \
    NEXT_BUCKET(bucket) = *_pos; \
    *_pos = index; \
} while (0)

#define COPY_BUCKET(dst, src) \
do { \
    HBucket *_src = src; \
    HBucket *_dst = dst; \
    ZVAL_COPY(&_dst->key, &_src->key); \
    ZVAL_COPY(&_dst->value, &_src->value); \
    BUCKET_HASH(_dst) = BUCKET_HASH(_src); \
} while (0)

#define DELETE_BUCKET(b) \
    DTOR_AND_UNDEF(&(b)->value); \
    DTOR_AND_UNDEF(&(b)->key); \
    NEXT_BUCKET((b)) = INVALID_INDEX

#define HTABLE_FOREACH_BUCKET(h, b)         \
do {                                        \
    HTable  *_h = h;                        \
    HBucket *_x = _h->buckets;              \
    HBucket *_y = _x + _h->next;            \
    for (; _x < _y; ++_x) {                 \
        if (BUCKET_DELETED(_x)) continue;   \
        b = _x;

#define HTABLE_FOREACH_BUCKET_BY_INDEX(h, i, b) \
do {                                            \
    uint32_t _i = 0;                            \
    HTable  *_h = h;                            \
    HBucket *_x = _h->buckets;                  \
    HBucket *_y = _x + _h->next;                \
    for (; _x < _y; ++_x) {                     \
        if (BUCKET_DELETED(_x)) continue;       \
        b = _x;                                 \
        i = _i++;

#define HTABLE_FOREACH_BUCKET_REVERSED(h, b)    \
do {                                            \
    HTable  *_h  = h;                           \
    HBucket *_x = _h->buckets;                  \
    HBucket *_y = _x + _h->next - 1;            \
    for (; _y >= _x; --_y) {                    \
        if (BUCKET_DELETED(_y)) continue;       \
        b = _y;

#define HTABLE_FOREACH(h, i, k, v)          \
do {                                        \
    uint32_t _i;                            \
    HBucket *_b = (h)->buckets;             \
    const uint32_t _n = (h)->size;          \
                                            \
    for (_i = 0; _i < _n; ++_b) {           \
        if (BUCKET_DELETED(_b)) continue;   \
        k = &_b->key;                       \
        v = &_b->value;                     \
        i = _i++;                           \

// To avoid redefinition when using multiple foreach in the same scope.
static HBucket *_b;

#define HTABLE_FOREACH_KEY(h, k) \
HTABLE_FOREACH_BUCKET(h, _b);    \
k = &_b->key;                    \

#define HTABLE_FOREACH_VALUE(h, v) \
HTABLE_FOREACH_BUCKET(h, _b);      \
v = &_b->value;                    \

#define HTABLE_FOREACH_KEY_VALUE(h, k, v)   \
HTABLE_FOREACH_BUCKET(h, _b);               \
k = &_b->key;                               \
v = &_b->value;                             \

#define HTABLE_FOREACH_END() \
    }                        \
} while (0)

void htable_create_key_set(HTable *table, zval *return_value);
void htable_create_value_sequence(HTable *table, zval *return_value);
void htable_create_pair_sequence(HTable *table, zval *return_value);
void htable_ensure_capacity(HTable *table, uint32_t capacity);

void htable_sort(HTable *table, compare_func_t compare_func);
void htable_sort_by_key(HTable *table);
void htable_sort_by_value(HTable *table);
void htable_sort_by_pair(HTable *table);
void htable_sort_callback_by_key(HTable *table);
void htable_sort_callback_by_value(HTable *table);
void htable_sort_callback(HTable *table);
HBucket *htable_lookup_by_value(HTable *h, zval *key);
HBucket *htable_lookup_by_key(HTable *h, zval *key);
HBucket *htable_lookup_by_position(HTable *table, uint32_t position);
bool htable_lookup_or_next(HTable *table, zval *key, HBucket **return_value);
bool htable_has_keys(HTable *h, VA_PARAMS);
bool htable_has_key(HTable *table, zval *key);
bool htable_has_values(HTable *h, VA_PARAMS);
bool htable_has_value(HTable *h, zval *value);
int htable_remove(HTable *h, zval *key, zval *return_value);
void htable_put(HTable *h, zval *key, zval *value);
void htable_to_array(HTable *h, zval *arr);
void htable_destroy(HTable *h);
zval *htable_get(HTable *h, zval *key);
HTable *htable_slice(HTable *table, zend_long index, zend_long length);
HTable *htable_init();
void htable_clear(HTable *h);
HTable *htable_clone(HTable *source);
HashTable *htable_pairs_to_php_ht(HTable *h);
bool htable_isset(HTable *h, zval *key, bool check_empty);
zend_string *htable_join_keys(HTable *table, const char* glue, const size_t len);
void htable_reverse(HTable *table);
HTable *htable_reversed(HTable *table);

HTable *htable_xor(HTable *table, HTable *other);
HTable *htable_diff(HTable *table, HTable *other);
HTable *htable_intersect(HTable *table, HTable *other);
HTable *htable_merge(HTable *table, HTable *other);

HBucket *htable_last(HTable *table);
HBucket *htable_first(HTable *table);

HTable *htable_map(HTable *table, FCI_PARAMS);
HTable *htable_filter_callback(HTable *table, FCI_PARAMS);
void htable_reduce(HTable *table, FCI_PARAMS, zval *initial, zval *return_value);

int htable_serialize(HTable *table, unsigned char **buffer, size_t *buf_len, zend_serialize_data *data);
int htable_unserialize(HTable *table, const unsigned char *buffer, size_t length, zend_unserialize_data *data);

#endif
