#include "../common.h"

#include "ds_pair.h"
#include "ds_htable.h"
#include "ds_set.h"
#include "ds_vector.h"

#include "../php/classes/php_hashable_ce.h"

static uint32_t next_power_of_2(uint32_t size)
{
    uint32_t c = MAX(size, DS_HTABLE_MIN_CAPACITY);

    c--;
    c |= c >> 1;
    c |= c >> 2;
    c |= c >> 4;
    c |= c >> 8;
    c |= c >> 16;
    c++;

    return c;
}

static inline ds_htable_bucket_t *ds_htable_allocate_buckets(uint32_t capacity)
{
    return ecalloc(capacity, sizeof(ds_htable_bucket_t));
}

static inline ds_htable_bucket_t *ds_htable_reallocate_buckets(ds_htable_t *table, uint32_t capacity)
{
    return erealloc(table->buckets, capacity * sizeof(ds_htable_bucket_t));
}

static inline uint32_t *ds_htable_allocate_lookup(uint32_t capacity)
{
    return emalloc(capacity * sizeof(uint32_t));
}

static inline uint32_t *ds_htable_reallocate_lookup(uint32_t *lookup, uint32_t capacity)
{
    return erealloc(lookup, capacity * sizeof(uint32_t));
}

static inline void ds_htable_reset_lookup(ds_htable_t *table)
{
    memset(table->lookup, DS_HTABLE_INVALID_INDEX, table->capacity * sizeof(uint32_t));
}

static inline void ds_htable_realloc(ds_htable_t *table, uint32_t capacity)
{
    table->buckets  = ds_htable_reallocate_buckets(table, capacity);
    table->lookup   = ds_htable_reallocate_lookup(table->lookup, capacity);
    table->capacity = capacity;
}

static void ds_htable_rehash(ds_htable_t *table)
{
    const uint32_t mask = table->capacity - 1;

    ds_htable_reset_lookup(table);

    // Rehash removes all deleted buckets, so we can reset min deleted.
    table->min_deleted = table->capacity;

    // No need to rehash if the table is empty.
    if (table->size == 0) {
        table->next = 0;
        return;

    } else {
        uint32_t index  = 0;
        ds_htable_bucket_t *bucket = table->buckets;

        if (DS_HTABLE_IS_PACKED(table)) { // No deleted buckets
            do {
                DS_HTABLE_BUCKET_REHASH(table, bucket, mask, index);
                bucket++;
            } while (++index < table->next);

        } else { // There are deleted buckets
            do {
                if (DS_HTABLE_BUCKET_DELETED(bucket)) {
                    uint32_t j = index;
                    ds_htable_bucket_t *q = bucket;

                    while (++index < table->next) {
                        if ( ! DS_HTABLE_BUCKET_DELETED(++bucket)) {
                            *q = *bucket;

                            DS_HTABLE_BUCKET_REHASH(table, q, mask, j);
                            q++;
                            j++;
                        }
                    }

                    table->next = j;
                    break;
                }
                DS_HTABLE_BUCKET_REHASH(table, bucket, mask, index);
                bucket++;

            } while (++index < table->next);
        }
    }
}

static void ds_htable_pack(ds_htable_t *table)
{
    if ( ! DS_HTABLE_IS_PACKED(table)) {
        ds_htable_bucket_t *end = table->buckets + table->next;
        ds_htable_bucket_t *src = table->buckets + table->min_deleted;
        ds_htable_bucket_t *dst = src;

        while (++src != end) {
            if ( ! DS_HTABLE_BUCKET_DELETED(src)) {
                if (dst != src) *dst = *src;
                dst++;
            }
        }

        table->next = table->size;
        table->min_deleted = table->capacity;
    }
}

static inline void ds_htable_double_capacity(ds_htable_t *table)
{
    ds_htable_realloc(table, table->capacity << 1);
    ds_htable_rehash(table);
}

static inline void ds_htable_halve_capacity(ds_htable_t *table)
{
    ds_htable_pack(table);
    ds_htable_realloc(table, table->capacity >> 1);
    ds_htable_rehash(table);
}

ds_htable_t *ds_htable_ex(uint32_t capacity)
{
    ds_htable_t *table = ecalloc(1, sizeof(ds_htable_t));

    table->buckets     = ds_htable_allocate_buckets(capacity);
    table->lookup      = ds_htable_allocate_lookup(capacity);
    table->capacity    = capacity;
    table->min_deleted = capacity;
    table->size        = 0;
    table->next        = 0;

    ds_htable_reset_lookup(table);
    return table;
}

ds_htable_t *ds_htable()
{
    return ds_htable_ex(DS_HTABLE_MIN_CAPACITY);
}

static void ds_htable_copy(ds_htable_t *_src, ds_htable_t *_dst)
{
    ds_htable_bucket_t *src = _src->buckets;
    ds_htable_bucket_t *end = _src->buckets + _src->next;
    ds_htable_bucket_t *dst = _dst->buckets;

    memcpy(_dst->lookup, _src->lookup, _src->capacity * sizeof(uint32_t));

    for (; src != end; ++src, ++dst) {
        if ( ! DS_HTABLE_BUCKET_DELETED(src)) {
            DS_HTABLE_BUCKET_COPY(dst, src);
        }
    }
}

ds_htable_t *ds_htable_clone(ds_htable_t *src)
{
    ds_htable_t *dst = ecalloc(1, sizeof(ds_htable_t));

    dst->buckets     = ds_htable_allocate_buckets(src->capacity);
    dst->lookup      = ds_htable_allocate_lookup(src->capacity);
    dst->capacity    = src->capacity;
    dst->size        = src->size;
    dst->next        = src->next;
    dst->min_deleted = src->min_deleted;

    ds_htable_copy(src, dst);
    return dst;
}

static inline bool implements_hashable(zval *key) {
    return instanceof_function(Z_OBJCE_P(key), hashable_ce);
}

static inline bool user_hashable_equals(zval *a, zval *b)
{
    if (Z_OBJCE_P(a) != Z_OBJCE_P(b)) {
        return false;

    } else {

        zval equals;
        zend_call_method_with_1_params(a, Z_OBJCE_P(a), NULL, "equals", &equals, b);
        return Z_TYPE(equals) == IS_TRUE;
    }
}

static inline bool key_is_identical(zval *a, zval *b)
{
    if (Z_TYPE_P(a) == IS_OBJECT && implements_hashable(a)) {
        return Z_TYPE_P(b) == IS_OBJECT && user_hashable_equals(a, b);
    }

    return zend_is_identical(a, b);
}

static inline bool ds_htable_bucket_key_match(ds_htable_bucket_t *bucket, zval *key)
{
    return key_is_identical(&bucket->key, key);
}

static inline uint32_t get_string_hash(zend_string *str)
{
    return ZSTR_HASH(str);
}

static inline uint32_t get_string_zval_hash(zval *value)
{
    return get_string_hash(Z_STR_P(value));
}

static uint32_t get_array_hash(zval *arr)
{
    uint32_t                 hash;
    php_serialize_data_t       var_hash;
    smart_str                  buffer = {0};

    PHP_VAR_SERIALIZE_INIT(var_hash);
    php_var_serialize(&buffer, arr, &var_hash);
    PHP_VAR_SERIALIZE_DESTROY(var_hash);

    smart_str_0(&buffer);

    if (buffer.s) {
        hash = get_string_hash(buffer.s);
        zend_string_free(buffer.s);
    } else {
        hash = 0;
    }

    return hash;
}

static inline uint32_t get_spl_object_hash(zval *obj)
{
    zend_string *s = php_spl_object_hash(obj);
    uint32_t hash = get_string_hash(s);
    zend_string_free(s);
    return hash;
}

static inline uint32_t get_resource_hash(zval *resource)
{
    return Z_RES_HANDLE_P(resource);
}

static uint32_t get_object_hash(zval *obj)
{
    if (implements_hashable(obj)) {

        zval hash;
        zend_call_method_with_0_params(obj, Z_OBJCE_P(obj), NULL, "hash", &hash);

        switch (Z_TYPE(hash)) {

            case IS_LONG:
                return Z_LVAL(hash);

            case IS_DOUBLE:
                return zval_get_long(&hash);

            case IS_STRING:
                return get_string_zval_hash(&hash);

            case IS_TRUE:
                return 1;

            case IS_FALSE:
            case IS_NULL:
                return 0;

            default:
                OBJ_HASH_MUST_BE_SCALAR(&hash);
                return 0;
        }
    }

    // Not instance of Hashable, so use spl_object_hash
    return get_spl_object_hash(obj);
}

static uint32_t get_hash(zval *value)
{
    switch (Z_TYPE_P(value)) {

        case IS_LONG:
            return Z_LVAL_P(value);

        case IS_DOUBLE:
            return zval_get_long(value);

        case IS_STRING:
            return get_string_zval_hash(value);

        case IS_TRUE:
            return 1;

        case IS_FALSE:
        case IS_NULL:
            return 0;

        case IS_OBJECT:
            return get_object_hash(value);

        case IS_ARRAY:
            return get_array_hash(value);

        case IS_RESOURCE:
            return get_resource_hash(value);

        case IS_REFERENCE:
            return get_hash(Z_REFVAL_P(value));

        default:
            return 0;
    }
}

static ds_htable_bucket_t *ds_htable_lookup_bucket_by_hash(ds_htable_t *table, zval *key, const uint32_t hash)
{
    ds_htable_bucket_t *bucket;

    uint32_t index = DS_HTABLE_BUCKET_LOOKUP(table, hash);

    for (; index != DS_HTABLE_INVALID_INDEX; index = DS_HTABLE_BUCKET_NEXT(bucket)) {
        bucket = &table->buckets[index];

        if (DS_HTABLE_BUCKET_HASH(bucket) == hash && ds_htable_bucket_key_match(bucket, key)) {
            return bucket;
        }
    }

    return NULL;
}

ds_htable_bucket_t *ds_htable_lookup_by_key(ds_htable_t *table, zval *key)
{
    return ds_htable_lookup_bucket_by_hash(table, key, get_hash(key));
}

ds_htable_bucket_t *ds_htable_lookup_by_position(ds_htable_t *table, uint32_t position)
{
    if (table->size == 0 || position >= table->size) {
        return NULL;

    } else if (DS_HTABLE_IS_PACKED(table) || position < table->min_deleted) {
        return &table->buckets[position];

    } else {
        ds_htable_bucket_t *bucket;
        uint32_t index;

        // Determine from which end to traverse the buffer
        if (position > table->size / 2) {

            // Start at the back
            index = table->size - 1;
            DS_HTABLE_FOREACH_BUCKET_REVERSED(table, bucket) {
                if (index-- == position) {
                    return bucket;
                }
            }
            DS_HTABLE_FOREACH_END();

        } else {

            // Start at the front
            index = 0;
            DS_HTABLE_FOREACH_BUCKET(table, bucket) {
                if (index++ == position) {
                    return bucket;
                }
            }
            DS_HTABLE_FOREACH_END();
        }
    }

    return NULL;
}

ds_htable_bucket_t *ds_htable_lookup_by_value(ds_htable_t *table, zval *value)
{
    ds_htable_bucket_t *bucket;

    DS_HTABLE_FOREACH_BUCKET(table, bucket) {
        if (zend_is_identical(value, &bucket->value)) {
            return bucket;
        }
    }
    DS_HTABLE_FOREACH_END();

    return NULL;
}

bool ds_htable_isset(ds_htable_t *table, zval *key, bool check_empty)
{
    ds_htable_bucket_t *bucket = ds_htable_lookup_by_key(table, key);
    return bucket && ds_zval_isset(&bucket->value, check_empty);
}

zval *ds_htable_get(ds_htable_t *table, zval *key)
{
    ds_htable_bucket_t *bucket = ds_htable_lookup_by_key(table, key);
    return bucket ? &bucket->value : NULL;
}

bool ds_htable_has_key(ds_htable_t *table, zval *key)
{
    return ds_htable_lookup_by_key(table, key) != NULL;
}

bool ds_htable_has_keys(ds_htable_t *table, VA_PARAMS)
{
    while (argc-- > 0) {
        if ( ! ds_htable_has_key(table, argv++)) {
            return false;
        }
    }

    return true;
}

bool ds_htable_has_value(ds_htable_t *table, zval *value)
{
    return ds_htable_lookup_by_value(table, value) != NULL;
}

bool ds_htable_has_values(ds_htable_t *table, VA_PARAMS)
{
    while (argc-- > 0) {
        if ( ! ds_htable_lookup_by_value(table, argv++)) {
            return false;
        }
    }

    return true;
}

void ds_htable_clear(ds_htable_t *table)
{
    ds_htable_bucket_t *bucket;

    if (table->size == 0) {
        return;
    }

    DS_HTABLE_FOREACH_BUCKET(table, bucket) {
        DS_HTABLE_BUCKET_DELETE(bucket);
    }
    DS_HTABLE_FOREACH_END();

    if (table->capacity > DS_HTABLE_MIN_CAPACITY) {
        ds_htable_realloc(table, DS_HTABLE_MIN_CAPACITY);
    }

    table->size = 0;
    table->next = 0;

    table->min_deleted = table->capacity;
}

void ds_htable_free(ds_htable_t *table)
{
    ds_htable_clear(table);

    efree(table->buckets);
    efree(table->lookup);
    efree(table);
}

static inline void ds_htable_sort_ex(ds_htable_t *table, compare_func_t compare_func)
{
    ds_htable_pack(table);
    qsort(table->buckets, table->size, sizeof(ds_htable_bucket_t), compare_func);
    ds_htable_rehash(table);
}

void ds_htable_sort(ds_htable_t *table, compare_func_t compare_func)
{
    ds_htable_sort_ex(table, compare_func);
}

static int user_compare_by_value(const void *a, const void *b)
{
    zval params[2];
    zval retval;

    ds_htable_bucket_t *x = ((ds_htable_bucket_t*)a);
    ds_htable_bucket_t *y = ((ds_htable_bucket_t*)b);

    ZVAL_COPY_VALUE(&params[0], &x->value);
    ZVAL_COPY_VALUE(&params[1], &y->value);

    DSG(user_compare_fci).param_count = 2;
    DSG(user_compare_fci).params      = params;
    DSG(user_compare_fci).retval      = &retval;

    if (zend_call_function(&DSG(user_compare_fci), &DSG(user_compare_fci_cache)) == SUCCESS) {
        return zval_get_long(&retval);
    }

    return 0;
}

static int user_compare_by_key(const void *a, const void *b)
{
    zval params[2];
    zval retval;

    ds_htable_bucket_t *x = ((ds_htable_bucket_t*)a);
    ds_htable_bucket_t *y = ((ds_htable_bucket_t*)b);

    ZVAL_COPY_VALUE(&params[0], &x->key);
    ZVAL_COPY_VALUE(&params[1], &y->key);

    DSG(user_compare_fci).param_count = 2;
    DSG(user_compare_fci).params      = params;
    DSG(user_compare_fci).retval      = &retval;

    if (zend_call_function(&DSG(user_compare_fci), &DSG(user_compare_fci_cache)) == SUCCESS) {
        return zval_get_long(&retval);
    }

    return 0;
}

static int compare_by_key(const void *a, const void *b)
{
    zval retval;
    ds_htable_bucket_t *x = (ds_htable_bucket_t*) a;
    ds_htable_bucket_t *y = (ds_htable_bucket_t*) b;

    if (compare_function(&retval, &x->key, &y->key) == SUCCESS) {
        return zval_get_long(&retval);
    }

    return 0;
}

static int compare_by_value(const void *a, const void *b)
{
    zval retval;
    ds_htable_bucket_t *x = (ds_htable_bucket_t*) a;
    ds_htable_bucket_t *y = (ds_htable_bucket_t*) b;

    if (compare_function(&retval, &x->value, &y->value) == SUCCESS) {
        return zval_get_long(&retval);
    }

    return 0;
}

void ds_htable_sort_by_key(ds_htable_t *table)
{
    ds_htable_sort(table, compare_by_key);
}

void ds_htable_sort_by_value(ds_htable_t *table)
{
    ds_htable_sort(table, compare_by_value);
}

void ds_htable_sort_callback_by_key(ds_htable_t *table)
{
    ds_htable_sort(table, user_compare_by_key);
}

void ds_htable_sort_callback_by_value(ds_htable_t *table)
{
    ds_htable_sort(table, user_compare_by_value);
}

static inline void ds_htable_increase_capacity(ds_htable_t *table)
{
    if (table->next > table->size + (table->size >> 5)) {
        ds_htable_rehash(table);
        return;
    }

    ds_htable_double_capacity(table);
}

void ds_htable_ensure_capacity(ds_htable_t *table, uint32_t capacity)
{
    if (capacity > table->capacity) {
        ds_htable_realloc(table, next_power_of_2(capacity));
        ds_htable_rehash(table);
    }
}

/**
 * Adds a bucket to the table knowing that its key doesn't already exist.
 */
static void ds_htable_put_distinct(ds_htable_t *table, ds_htable_bucket_t *bucket)
{
    DS_HTABLE_BUCKET_COPY(&table->buckets[table->next], bucket);
    DS_HTABLE_BUCKET_REHASH(table, bucket, table->capacity - 1, table->next);

    table->next++;
    table->size++;

    if (table->next == table->capacity) {
        ds_htable_increase_capacity(table);
    }
}

static ds_htable_bucket_t *ds_htable_next_bucket(ds_htable_t *table, zval *key, const uint32_t hash)
{
    ds_htable_bucket_t *bucket = &table->buckets[table->next];

    DS_HTABLE_BUCKET_HASH(bucket) = hash;
    ZVAL_COPY(&bucket->key, key);
    DS_HTABLE_BUCKET_REHASH(table, bucket, table->capacity - 1, table->next);

    table->next++;
    table->size++;

    return bucket;
}

bool ds_htable_lookup_or_next(ds_htable_t *table, zval *key, ds_htable_bucket_t **bucket)
{
    const uint32_t hash = get_hash(key);

    // Attempt to find the bucket
    if ((*bucket = ds_htable_lookup_bucket_by_hash(table, key, hash))) {
        return true;
    }

    if (table->next == table->capacity) {
        ds_htable_increase_capacity(table);
    }

    // Bucket couldn't be found, so create as new bucket in the buffer.
    *bucket = ds_htable_next_bucket(table, key, hash);

    return false;
}

void ds_htable_put(ds_htable_t *table, zval *key, zval *value)
{
    ds_htable_bucket_t *bucket;

    bool found = ds_htable_lookup_or_next(table, key, &bucket);

    if (found) {
        zval_ptr_dtor(&bucket->value);
    }

    ZVAL_COPY(&bucket->value, value);
}

static zval *create_zval_buffer_of_values(ds_htable_t *table)
{
    zval *buffer = ALLOC_ZVAL_BUFFER(table->size);
    zval *target = buffer;
    zval *value;

    DS_HTABLE_FOREACH_VALUE(table, value) {
        ZVAL_COPY(target++, value);
    }
    DS_HTABLE_FOREACH_END();

    return buffer;
}

static zval *create_zval_buffer_of_keys(ds_htable_t *table)
{
    zval *buffer = ALLOC_ZVAL_BUFFER(table->size);
    zval *target = buffer;
    zval *key;

    DS_HTABLE_FOREACH_KEY(table, key) {
        ZVAL_COPY(target++, key);
    }
    DS_HTABLE_FOREACH_END();

    return buffer;
}

ds_vector_t *ds_htable_values_to_vector(ds_htable_t *table)
{
    zval *buffer = create_zval_buffer_of_values(table);
    return ds_vector_from_buffer(buffer, table->size);
}

static ds_htable_bucket_t *get_last_bucket(ds_htable_t *table)
{
    if (table->size > 0) {
        ds_htable_bucket_t *last = table->buckets + table->next;
        for (;;) {
            if ( ! DS_HTABLE_BUCKET_DELETED(--last)) {
                return last;
            }
        }
    }

    return NULL;
}

zend_string *ds_htable_join_keys(ds_htable_t *table, const char* glue, const size_t len)
{
    smart_str str = {0};

    if (table->size == 0) {
        return ZSTR_EMPTY_ALLOC();
    }

    if (table->size == 1) {
        ds_htable_bucket_t *last = get_last_bucket(table);
        return zval_get_string(&last->key);
    }

    if (glue && len) {
        ds_htable_bucket_t *pos = table->buckets;
        ds_htable_bucket_t *end = get_last_bucket(table);
        do {
            if ( ! DS_HTABLE_BUCKET_DELETED(pos)) {
                smart_str_append (&str, zval_get_string(&pos->key));
                smart_str_appendl(&str, glue, len);
            }
        } while (++pos != end);

        // Append last keys
        smart_str_append(&str, zval_get_string(&end->key));

    } else {
        // No glue, so just append the keys.
        zval *key;
        DS_HTABLE_FOREACH_KEY(table, key) {
            smart_str_append(&str, zval_get_string(key));
        }
        DS_HTABLE_FOREACH_END();
    }

    smart_str_0(&str);
    return str.s;
}

int ds_htable_remove(ds_htable_t *table, zval *key, zval *return_value)
{
    uint32_t hash  = get_hash(key);
    uint32_t index = DS_HTABLE_BUCKET_LOOKUP(table, hash);

    ds_htable_bucket_t *bucket = NULL;
    ds_htable_bucket_t *prev   = NULL;

    for (; index != DS_HTABLE_INVALID_INDEX; index = DS_HTABLE_BUCKET_NEXT(bucket)) {
        bucket = &table->buckets[index];

        if (DS_HTABLE_BUCKET_HASH(bucket) != hash || ! ds_htable_bucket_key_match(bucket, key)) {
            prev = bucket;
            continue;
        }

        if (return_value) {
            ZVAL_COPY(return_value, &bucket->value);
        }

        // Clear the lookup if there wasn't a collision chain,
        // otherwise remove the link in the chain.
        if (prev == NULL) {
            DS_HTABLE_BUCKET_LOOKUP(table, hash) = DS_HTABLE_BUCKET_NEXT(bucket);
        } else {
            DS_HTABLE_BUCKET_NEXT(prev) = DS_HTABLE_BUCKET_NEXT(bucket);
        }

        DS_HTABLE_BUCKET_DELETE(bucket);

        // If we're removing the last bucket, we might be able to move the
        // next open slot back if preceeding buckets are also deleted.
        if (index == table->next - 1 && table->size > 1) {
            do {
                table->next--;
                bucket--;
            }
            while (DS_HTABLE_BUCKET_DELETED(bucket));
        }

        //
        if (index < table->min_deleted) {
            table->min_deleted = index;
        }

        //
        if ((--table->size) <= table->capacity >> 2) {
            ds_htable_halve_capacity(table);
        }

        return SUCCESS;
    }

    if (return_value) {
        ZVAL_NULL(return_value);
    }

    return FAILURE;
}

ds_htable_t *ds_htable_slice(ds_htable_t *table, zend_long index, zend_long length)
{
    ds_normalize_slice_args(&index, &length, table->size);

    if (length == 0) {
        return ds_htable();

    } else {
        ds_htable_t *slice = ds_htable_ex(length);

        /**
         * If the table doesn't have any deleted buckets or if the first deleted
         * bucket comes after the slice we're after, we can safely seek
         * to the index and copy each bucket. We don't have to worry about
         * increasing capacity or checking if a key already exists.
         */
        if (DS_HTABLE_IS_PACKED(table) || (index + length) <= table->min_deleted) {
            ds_htable_bucket_t *src = &table->buckets[index];

            for (; length-- > 0; src++) {
                ds_htable_bucket_t *dst = ds_htable_next_bucket(slice, &src->key, DS_HTABLE_BUCKET_HASH(src));
                ZVAL_COPY(&dst->value, &src->value);
            }

        /**
         * If the table does have deleted buckets but the first one comes after
         * the index of the slice, we can safely seek to the index.
         */
        } else if (index < table->min_deleted) {
            ds_htable_bucket_t *src = &table->buckets[index];

            for (;;) {
                ds_htable_bucket_t *dst = ds_htable_next_bucket(slice, &src->key, DS_HTABLE_BUCKET_HASH(src));
                ZVAL_COPY(&dst->value, &src->value);

                if (--length == 0) {
                    break;
                }

                // Skip deleted buckets
                while (DS_HTABLE_BUCKET_DELETED(++src));
            }

        /**
         * Otherwise there are deleted buckets but they're in the slice range.
         * The best we can do is just skip any deleted buckets we encounter.
         */
        } else {
            uint32_t pos = 0;
            ds_htable_bucket_t *src = table->buckets;

            // We have to seek iteratively until we reach the index
            while (pos < index) {
                if (DS_HTABLE_BUCKET_DELETED(src++)) {
                    continue;
                }
                pos++;
            }

            // We're at the index, so gather across.
            for (; length > 0; src++) {
                if (DS_HTABLE_BUCKET_DELETED(src)) {
                    continue;
                }

                ds_htable_bucket_t *dst = ds_htable_next_bucket(slice, &src->key, DS_HTABLE_BUCKET_HASH(src));
                ZVAL_COPY(&dst->value, &src->value);
                length--;
            }
        }

        return slice;
    }
}

void ds_htable_apply(ds_htable_t *table, FCI_PARAMS)
{
    zval params[2];
    zval retval;

    ds_htable_bucket_t *bucket;

    DS_HTABLE_FOREACH_BUCKET(table, bucket) {
        ZVAL_COPY_VALUE(&params[0], &bucket->key);
        ZVAL_COPY_VALUE(&params[1], &bucket->value);

        fci.param_count = 2;
        fci.params      = params;
        fci.retval      = &retval;

        if (zend_call_function(&fci, &fci_cache) == FAILURE || Z_ISUNDEF(retval)) {
            return;
        } else {
            ZVAL_DTOR_COPY(&bucket->value, &retval);
        }
    }
    DS_HTABLE_FOREACH_END();
}

ds_htable_t *ds_htable_map(ds_htable_t *table, FCI_PARAMS)
{
    zval params[2];
    zval retval;

    ds_htable_t *clone = ds_htable_clone(table);

    ds_htable_bucket_t *end = table->buckets + table->next;
    ds_htable_bucket_t *src = table->buckets;
    ds_htable_bucket_t *dst = clone->buckets;

    for (; src != end; ++src, ++dst) {
        if (DS_HTABLE_BUCKET_DELETED(src)) {
            continue;
        }

        ZVAL_COPY_VALUE(&params[0], &src->key);
        ZVAL_COPY_VALUE(&params[1], &src->value);

        fci.param_count = 2;
        fci.params      = params;
        fci.retval      = &retval;

        if (zend_call_function(&fci, &fci_cache) == FAILURE || Z_ISUNDEF(retval)) {
            ds_htable_free(clone);
            return NULL;
        } else {
            zval_ptr_dtor(&dst->value);
            ZVAL_COPY(&dst->value, &retval);
        }
    }

    return clone;
}

ds_htable_t *ds_htable_filter(ds_htable_t *table)
{
    ds_htable_bucket_t *src, *dst;

    ds_htable_t *filtered = ds_htable_ex(table->capacity);

    DS_HTABLE_FOREACH_BUCKET(table, src) {
        if (zend_is_true(&src->value)) {
            dst = ds_htable_next_bucket(filtered, &src->key, DS_HTABLE_BUCKET_HASH(src));
            ZVAL_COPY(&dst->value, &src->value);
        }
    }
    DS_HTABLE_FOREACH_END();

    return filtered;
}

ds_htable_t *ds_htable_filter_callback(ds_htable_t *table, FCI_PARAMS)
{
    ds_htable_bucket_t *src, *dst;

    zval params[2];
    zval retval;

    ds_htable_t *filtered = ds_htable_ex(table->capacity);

    DS_HTABLE_FOREACH_BUCKET(table, src) {
        ZVAL_COPY_VALUE(&params[0], &src->key);
        ZVAL_COPY_VALUE(&params[1], &src->value);

        fci.param_count = 2;
        fci.params      = params;
        fci.retval      = &retval;

        if (zend_call_function(&fci, &fci_cache) == FAILURE || Z_ISUNDEF(retval)) {
            ds_htable_free(filtered);
            return NULL;
        } else {
            if (zend_is_true(&retval)) {
                dst = ds_htable_next_bucket(filtered, &src->key, DS_HTABLE_BUCKET_HASH(src));
                ZVAL_COPY(&dst->value, &src->value);
            }
        }
    }
    DS_HTABLE_FOREACH_END();
    return filtered;
}

void ds_htable_reduce(ds_htable_t *table, FCI_PARAMS, zval *initial, zval *return_value)
{
    zval *key, *value;
    zval carry;

    zval params[3];
    zval retval;

    if (initial == NULL) {
        ZVAL_NULL(&carry);
    } else {
        ZVAL_COPY_VALUE(&carry, initial);
    }

    DS_HTABLE_FOREACH_KEY_VALUE(table, key, value) {
        ZVAL_COPY_VALUE(&params[0], &carry);
        ZVAL_COPY_VALUE(&params[1], key);
        ZVAL_COPY_VALUE(&params[2], value);

        fci.param_count = 3;
        fci.params      = params;
        fci.retval      = &retval;

        if (zend_call_function(&fci, &fci_cache) == FAILURE || Z_ISUNDEF(retval)) {
            ZVAL_NULL(return_value);
            return;
        } else {
            ZVAL_COPY_VALUE(&carry, &retval);
        }
    }
    DS_HTABLE_FOREACH_END();

    ZVAL_COPY(return_value, &carry);
}

ds_htable_t *ds_htable_xor(ds_htable_t *table, ds_htable_t *other)
{
    ds_htable_bucket_t *bucket;
    ds_htable_t *xor = ds_htable();

    DS_HTABLE_FOREACH_BUCKET(table, bucket) {
        if ( ! ds_htable_has_key(other, &bucket->key)) {
            ds_htable_put_distinct(xor, bucket);
        }
    }
    DS_HTABLE_FOREACH_END();

    DS_HTABLE_FOREACH_BUCKET(other, bucket) {
        if ( ! ds_htable_has_key(table, &bucket->key)) {
            ds_htable_put_distinct(xor, bucket);
        }
    }
    DS_HTABLE_FOREACH_END();

    return xor;
}

ds_htable_t *ds_htable_diff(ds_htable_t *table, ds_htable_t *other)
{
    ds_htable_bucket_t *bucket;
    ds_htable_t *diff = ds_htable();

    DS_HTABLE_FOREACH_BUCKET(table, bucket) {
        if ( ! ds_htable_has_key(other, &bucket->key)) {
            ds_htable_put_distinct(diff, bucket);
        }
    }
    DS_HTABLE_FOREACH_END();

    return diff;
}

ds_htable_t *ds_htable_intersect(ds_htable_t *table, ds_htable_t *other)
{
    ds_htable_bucket_t *bucket;

    ds_htable_t *intersection = ds_htable();

    DS_HTABLE_FOREACH_BUCKET(table, bucket) {
        if (ds_htable_has_key(other, &bucket->key)) {
            ds_htable_put_distinct(intersection, bucket);
        }
    }
    DS_HTABLE_FOREACH_END();

    return intersection;
}

ds_htable_t *ds_htable_merge(ds_htable_t *table, ds_htable_t *other)
{
    ds_htable_bucket_t *bucket;
    ds_htable_t *merged = ds_htable_clone(table);

    DS_HTABLE_FOREACH_BUCKET(other, bucket) {
        ds_htable_put(merged, &bucket->key, &bucket->value);
    }
    DS_HTABLE_FOREACH_END();

    return merged;
}

ds_htable_bucket_t *ds_htable_last(ds_htable_t *table)
{
    if (table->size == 0) {
        return NULL;

    } else {
        ds_htable_bucket_t *last = &table->buckets[table->next - 1];

        if ( ! DS_HTABLE_IS_PACKED(table)) {
            while (DS_HTABLE_BUCKET_DELETED(last)) {
                last--;
            }
        }

        return last;
    }
}

ds_htable_bucket_t *ds_htable_first(ds_htable_t *table)
{
    if (table->size == 0) {
        return NULL;

    } else {
        ds_htable_bucket_t *first = table->buckets;

        if (table->min_deleted > 0) {
            while (DS_HTABLE_BUCKET_DELETED(first)) {
                first++;
            }
        }

        return first;
    }
}

void ds_htable_reverse(ds_htable_t *table)
{
    ds_htable_pack(table);
    {
        ds_htable_bucket_t *a = table->buckets;
        ds_htable_bucket_t *b = table->buckets + table->size - 1;

        for (; a < b; ++a, --b) {
            ds_htable_bucket_t c = *a;
            *a = *b;
            *b = c;
        }
    }
    ds_htable_rehash(table);
}

ds_htable_t *ds_htable_reversed(ds_htable_t *table)
{
    ds_htable_t *reversed = ds_htable_ex(table->capacity);

    ds_htable_bucket_t *src = NULL;
    ds_htable_bucket_t *dst = reversed->buckets;

    const uint32_t mask = reversed->capacity - 1;

    DS_HTABLE_FOREACH_BUCKET_REVERSED(table, src) {
        uint32_t *lookup = &reversed->lookup[DS_HTABLE_BUCKET_HASH(src) & mask];

        DS_HTABLE_BUCKET_COPY(dst, src);
        DS_HTABLE_BUCKET_NEXT(dst) = *lookup;
        *lookup = reversed->next++;
        dst++;
    }
    DS_HTABLE_FOREACH_END();

    reversed->size = table->size;
    return reversed;
}

void ds_htable_to_array(ds_htable_t *table, zval *return_value)
{
    HashTable *array;
    zval *key;
    zval *val;

    array_init_size(return_value, table->size);
    array = Z_ARR_P(return_value);

    DS_HTABLE_FOREACH_KEY_VALUE(table, key, val) {
        array_set_zval_key(array, key, val);
    }
    DS_HTABLE_FOREACH_END();
}

int ds_htable_serialize(ds_htable_t *table, unsigned char **buffer, size_t *length, zend_serialize_data *data)
{
    php_serialize_data_t serialize_data = (php_serialize_data_t) data;
    PHP_VAR_SERIALIZE_INIT(serialize_data);

    if (table->size == 0) {
        SERIALIZE_SET_ZSTR(ZSTR_EMPTY_ALLOC());

    } else {

        zval *key, *value;

        smart_str buf = {0};

        DS_HTABLE_FOREACH_KEY_VALUE(table, key, value) {
            php_var_serialize(&buf, key, &serialize_data);
            php_var_serialize(&buf, value, &serialize_data);
        }
        DS_HTABLE_FOREACH_END();

        smart_str_0(&buf);

        SERIALIZE_SET_ZSTR(buf.s);
        zend_string_release(buf.s);
    }

    PHP_VAR_SERIALIZE_DESTROY(serialize_data);
    return SUCCESS;
}

int ds_htable_unserialize(ds_htable_t *table, const unsigned char *buffer, size_t length, zend_unserialize_data *data)
{
    php_unserialize_data_t unserialize_data = (php_unserialize_data_t) data;

    const unsigned char *pos = buffer;
    const unsigned char *max = buffer + length;

    PHP_VAR_UNSERIALIZE_INIT(unserialize_data);

    while (*pos != '}') {

        zval *key   = var_tmp_var(&unserialize_data);
        zval *value = var_tmp_var(&unserialize_data);

        if (php_var_unserialize(key, &pos, max, &unserialize_data)) {
            var_push_dtor(&unserialize_data, key);
        } else {
            goto error;
        }

        if (php_var_unserialize(value, &pos, max, &unserialize_data)) {
            var_push_dtor(&unserialize_data, value);
        } else {
            goto error;
        }

        ds_htable_put(table, key, value);
    }

    if (*(++pos) != '\0') {
        goto error;
    }

    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    return SUCCESS;

error:
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    UNSERIALIZE_ERROR();
    return FAILURE;
}
