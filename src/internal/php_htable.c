#include "../common.h"
#include "../classes/php_ce_hashable.h"
#include "php_pair.h"
#include "php_htable.h"
#include "php_set.h"

static uint32_t next_power_of_2(uint32_t size)
{
    uint32_t c = MAX(size, HTABLE_MIN_CAPACITY);

    c--;
    c |= c >> 1;
    c |= c >> 2;
    c |= c >> 4;
    c |= c >> 8;
    c |= c >> 16;
    c++;

    return c;
}

static inline bool htable_is_packed(HTable *table)
{
    return table->size == table->next;
}

static inline HBucket *allocate_buckets(uint32_t capacity)
{
    return ecalloc(capacity, sizeof(HBucket));
}

static inline HBucket *reallocate_buckets(HTable *table, uint32_t capacity)
{
    return erealloc(table->buckets, capacity * sizeof(HBucket));
}

static inline uint32_t *allocate_lookup(uint32_t capacity)
{
    return emalloc(capacity * sizeof(uint32_t));
}

static inline uint32_t *reallocate_lookup(uint32_t *lookup, uint32_t capacity)
{
    return erealloc(lookup, capacity * sizeof(uint32_t));
}

static inline void reset_lookup(HTable *table)
{
    memset(table->lookup, INVALID_INDEX, table->capacity * sizeof(uint32_t));
}

static inline void htable_realloc(HTable *table, uint32_t capacity)
{
    table->buckets  = reallocate_buckets(table, capacity);
    table->lookup   = reallocate_lookup(table->lookup, capacity);
    table->capacity = capacity;
}

static void rehash_table(HTable *table)
{
    const uint32_t mask = table->capacity - 1;

    reset_lookup(table);

    // Rehash removes all deleted buckets, so we can reset min deleted.
    table->min_deleted = table->capacity;

    // No need to rehash if the table is empty.
    if (table->size == 0) {
        table->next = 0;
        return;

    } else {
        uint32_t index  = 0;
        HBucket *bucket = table->buckets;

        if (HTABLE_IS_PACKED(table)) { // No deleted buckets
            do {
                REHASH_BUCKET(table, bucket, mask, index);
                bucket++;
            } while (++index < table->next);

        } else { // There are deleted buckets
            do {
                if (BUCKET_DELETED(bucket)) {
                    uint32_t j = index;
                    HBucket *q = bucket;

                    while (++index < table->next) {
                        if (BUCKET_NOT_DELETED(++bucket)) {
                            *q = *bucket;

                            REHASH_BUCKET(table, q, mask, j);
                            q++;
                            j++;
                        }
                    }

                    table->next = j;
                    break;
                }
                REHASH_BUCKET(table, bucket, mask, index);
                bucket++;

            } while (++index < table->next);
        }
    }
}

static void pack_table(HTable *table)
{
    if ( ! htable_is_packed(table)) {
        HBucket *end = table->buckets + table->next;
        HBucket *src = table->buckets + table->min_deleted;
        HBucket *dst = src;

        while (++src != end) {
            if (BUCKET_NOT_DELETED(src)) {
                if (dst != src) *dst = *src;
                dst++;
            }
        }

        table->next = table->size;
        table->min_deleted = table->capacity;
    }
}

static inline void htable_double_capacity(HTable *table)
{
    htable_realloc(table, table->capacity << 1);
    rehash_table(table);
}

static inline void htable_halve_capacity(HTable *table)
{
    pack_table(table);
    htable_realloc(table, table->capacity >> 1);
    rehash_table(table);
}

HTable *htable_init_ex(uint32_t capacity)
{
    HTable *table = ecalloc(1, sizeof(HTable));

    table->buckets     = allocate_buckets(capacity);
    table->lookup      = allocate_lookup(capacity);
    table->capacity    = capacity;
    table->min_deleted = capacity;
    table->size        = 0;
    table->next        = 0;

    reset_lookup(table);
    return table;
}

HTable *htable_init()
{
    return htable_init_ex(HTABLE_MIN_CAPACITY);
}

static void htable_copy(HTable *_src, HTable *_dst)
{
    HBucket *src = _src->buckets;
    HBucket *end = _src->buckets + _src->next;
    HBucket *dst = _dst->buckets;

    memcpy(_dst->lookup, _src->lookup, _src->capacity * sizeof(uint32_t));

    for (; src != end; ++src, ++dst) {
        if (BUCKET_NOT_DELETED(src)) {
            COPY_BUCKET(dst, src);
        }
    }
}

HTable *htable_clone(HTable *src)
{
    HTable *dst = ecalloc(1, sizeof(HTable));

    dst->buckets     = allocate_buckets(src->capacity);
    dst->lookup      = allocate_lookup(src->capacity);
    dst->capacity    = src->capacity;
    dst->size        = src->size;
    dst->next        = src->next;
    dst->min_deleted = src->min_deleted;

    htable_copy(src, dst);
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

static inline bool bucket_key_matches(HBucket *bucket, zval *key)
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

static inline HBucket *lookup_bucket_by_hash(HTable *table, zval *key, const uint32_t hash)
{
    HBucket *bucket;
    uint32_t index = BUCKET_LOOKUP(table, hash);

    for (; index != INVALID_INDEX; index = NEXT_BUCKET(bucket)) {
        bucket = &table->buckets[index];

        if (BUCKET_HASH(bucket) == hash && bucket_key_matches(bucket, key)) {
            return bucket;
        }
    }

    return NULL;
}

HBucket *htable_lookup_by_key(HTable *table, zval *key)
{
    return lookup_bucket_by_hash(table, key, get_hash(key));
}

HBucket *htable_lookup_by_position(HTable *table, uint32_t position)
{
    if (table->size == 0 || position >= table->size) {
        return NULL;

    } else if (HTABLE_IS_PACKED(table) || position < table->min_deleted) {
        return &table->buckets[position];

    } else {
        HBucket *bucket;
        uint32_t index;

        // Determine from which end to traverse the buffer
        if (position > table->size >> 1) {

            // Start at the back
            index = table->size - 1;
            HTABLE_FOREACH_BUCKET_REVERSED(table, bucket) {
                if (index == position) return bucket;
                index--;
            }
            HTABLE_FOREACH_END();

        } else {

            // Start at the front
            index = 0;
            HTABLE_FOREACH_BUCKET(table, bucket) {
                if (index == position) return bucket;
                index++;
            }
            HTABLE_FOREACH_END();
        }
    }

    return NULL;
}

HBucket *htable_lookup_by_value(HTable *table, zval *value)
{
    HBucket *bucket;

    HTABLE_FOREACH_BUCKET(table, bucket) {
        if (zend_is_identical(value, &bucket->value)) {
            return bucket;
        }
    }
    HTABLE_FOREACH_END();
    return NULL;
}

bool htable_isset(HTable *table, zval *key, bool check_empty)
{
    HBucket *bucket = htable_lookup_by_key(table, key);
    return bucket && zval_isset(&bucket->value, check_empty);
}

zval *htable_get(HTable *table, zval *key)
{
    HBucket *bucket = htable_lookup_by_key(table, key);
    return bucket ? &bucket->value : NULL;
}

bool htable_has_key(HTable *table, zval *key)
{
    return htable_lookup_by_key(table, key) != NULL;
}

bool htable_has_keys(HTable *table, VA_PARAMS)
{
    if (argc == 0) {
        return false;
    }

    while (argc--) {
        if ( ! htable_has_key(table, argv++)) {
            return false;
        }
    }

    return true;
}

bool htable_has_value(HTable *table, zval *value)
{
    return htable_lookup_by_key(table, value) != NULL;
}

bool htable_has_values(HTable *table, VA_PARAMS)
{
    if (argc == 0) {
        return false;
    }

    while (argc--) {
        if ( ! htable_lookup_by_value(table, argv++)) {
            return false;
        }
    }

    return true;
}

void htable_clear(HTable *table)
{
    HBucket *bucket;

    if (table->size == 0) {
        return;
    }

    HTABLE_FOREACH_BUCKET(table, bucket) {
        DELETE_BUCKET(bucket);
    }
    HTABLE_FOREACH_END();

    htable_realloc(table, HTABLE_MIN_CAPACITY);
    table->min_deleted = table->capacity;

    table->size = 0;
    table->next = 0;
}

void htable_destroy(HTable *table)
{
    htable_clear(table);

    efree(table->buckets);
    efree(table->lookup);
    efree(table);
}

static inline void htable_sort_ex(HTable *table, compare_func_t compare_func)
{
    pack_table(table);
    qsort(table->buckets, table->size, sizeof(HBucket), compare_func);
    rehash_table(table);
}

void htable_sort(HTable *table, compare_func_t compare_func)
{
    htable_sort_ex(table, compare_func);
}

static int user_compare_by_value(const void *a, const void *b)
{
    zval params[2];
    zval retval;

    HBucket *x = ((HBucket*)a);
    HBucket *y = ((HBucket*)b);

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

    HBucket *x = ((HBucket*)a);
    HBucket *y = ((HBucket*)b);

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

static int user_compare_by_pair(const void *a, const void *b)
{
    zval params[2];
    zval retval;

    HBucket *x = (HBucket*) a;
    HBucket *y = (HBucket*) b;

    pair_create_as_zval(&x->key, &x->value, &params[0]);
    pair_create_as_zval(&y->key, &y->value, &params[1]);

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
    HBucket *x = (HBucket*) a;
    HBucket *y = (HBucket*) b;

    if (compare_function(&retval, &x->key, &y->key) == SUCCESS) {
        return zval_get_long(&retval);
    }

    return 0;
}

static int compare_by_value(const void *a, const void *b)
{
    zval retval;
    HBucket *x = (HBucket*) a;
    HBucket *y = (HBucket*) b;

    if (compare_function(&retval, &x->value, &y->value) == SUCCESS) {
        return zval_get_long(&retval);
    }

    return 0;
}

void htable_sort_by_key(HTable *table)
{
    htable_sort(table, compare_by_key);
}

void htable_sort_by_value(HTable *table)
{
    htable_sort(table, compare_by_value);
}

void htable_sort_callback_by_key(HTable *table)
{
    htable_sort(table, user_compare_by_key);
}

void htable_sort_callback_by_value(HTable *table)
{
    htable_sort(table, user_compare_by_value);
}

void htable_sort_callback(HTable *table)
{
    htable_sort(table, user_compare_by_pair);
}

static inline void htable_increase_capacity(HTable *table)
{
    if (table->next > table->size + (table->size >> 5)) {
        rehash_table(table);
        return;
    }

    htable_double_capacity(table);
}

void htable_ensure_capacity(HTable *table, uint32_t capacity)
{
    if (capacity > table->capacity) {
        htable_realloc(table, next_power_of_2(capacity));
        rehash_table(table);
    }
}


static HBucket *init_next_bucket(HTable *table, zval *key, const uint32_t hash)
{
    HBucket *bucket = &table->buckets[table->next];

    BUCKET_HASH(bucket) = hash;
    ZVAL_COPY(&bucket->key, key);
    REHASH_BUCKET(table, bucket, table->capacity - 1, table->next);

    table->next++;
    table->size++;

    return bucket;
}

static void htable_put_next(HTable *table, HBucket *bucket)
{
    COPY_BUCKET(&table->buckets[table->next], bucket);

    table->next++;
    table->size++;

    if (table->next == table->capacity) {
        htable_increase_capacity(table);
    }
}

bool htable_lookup_or_next(HTable *table, zval *key, HBucket **return_value)
{
    const uint32_t hash = get_hash(key);

    HBucket *bucket = lookup_bucket_by_hash(table, key, hash);
    if (bucket) {
        *return_value = bucket;
        return true;
    }

    if (table->next == table->capacity) {
        htable_increase_capacity(table);
    }

    *return_value = init_next_bucket(table, key, hash);
    return false;
}

void htable_put(HTable *table, zval *key, zval *value)
{
    HBucket *bucket;
    bool found = htable_lookup_or_next(table, key, &bucket);

    if (found) {
        zval_ptr_dtor(&bucket->value);
    }

    ZVAL_COPY(&bucket->value, value);
}

static zval *create_zval_buffer_of_values(HTable *table)
{
    zval *buffer = ALLOC_ZVAL_BUFFER(table->size);
    zval *target = buffer;
    zval *value;

    HTABLE_FOREACH_VALUE(table, value) {
        ZVAL_COPY(target++, value);
    }
    HTABLE_FOREACH_END();

    return buffer;
}

static zval *create_zval_buffer_of_keys(HTable *table)
{
    zval *buffer = ALLOC_ZVAL_BUFFER(table->size);
    zval *target = buffer;
    zval *key;

    HTABLE_FOREACH_KEY(table, key) {
        ZVAL_COPY(target++, key);
    }
    HTABLE_FOREACH_END();

    return buffer;
}

static zval *create_zval_buffer_of_pairs(HTable *table)
{
    zval *buffer = ALLOC_ZVAL_BUFFER(table->size);
    zval *target = buffer;

    zval *key;
    zval *value;

    HTABLE_FOREACH_KEY_VALUE(table, key, value) {
        pair_create_as_zval(key, value, target++);
    }
    HTABLE_FOREACH_END();

    return buffer;
}

void htable_create_key_set(HTable *table, zval *obj)
{
    HTable *clone = htable_clone(table);
    ZVAL_OBJ(obj, set_create_object_ex(clone));
}

void htable_create_value_sequence(HTable *table, zval *obj)
{
    zval *buffer = create_zval_buffer_of_values(table);
    create_sequence(obj, buffer, table->size);
}

void htable_create_pair_sequence(HTable *table, zval *obj)
{
    zval *buffer = create_zval_buffer_of_pairs(table);
    create_sequence(obj, buffer, table->size);
}

static inline HBucket *get_last_bucket(HTable *table)
{
    if (table->size > 0) {
        HBucket *last = table->buckets + table->next;
        for (;;) {
            if (BUCKET_NOT_DELETED(--last)) return last;
        }
    }
    return NULL;
}

zend_string *htable_join_keys(HTable *table, const char* glue, const size_t len)
{
    smart_str str = {0};

    if (table->size == 0) {
        return ZSTR_EMPTY_ALLOC();
    }

    if (table->size == 1) {
        HBucket *last = get_last_bucket(table);
        return zval_get_string(&last->key);
    }

    if (glue && len) {
        HBucket *pos = table->buckets;
        HBucket *end = get_last_bucket(table);
        do {
            if (BUCKET_NOT_DELETED(pos)) {
                smart_str_append(&str, zval_get_string(&pos->key));
                smart_str_appendl(&str, glue, len);
            }
        } while (++pos != end);

        // Append last value
        smart_str_append(&str, zval_get_string(&end->key));

    } else {
        // No glue, so just append the values.

        zval *key;
        HTABLE_FOREACH_KEY(table, key) {
            smart_str_append(&str, zval_get_string(key));
        }
        HTABLE_FOREACH_END();
    }

    smart_str_0(&str);
    return str.s;
}

int htable_remove(HTable *table, zval *key, zval *return_value)
{
    const uint32_t hash = get_hash(key);

    uint32_t index = BUCKET_LOOKUP(table, hash);

    HBucket *bucket = NULL;
    HBucket *prev   = NULL;

    for (; index != INVALID_INDEX; index = NEXT_BUCKET(bucket)) {
        bucket = &table->buckets[index];

        if (BUCKET_HASH(bucket) != hash || ! bucket_key_matches(bucket, key)) {
            prev = bucket;
            continue;
        }

        if (return_value) {
            ZVAL_COPY(return_value, &bucket->value);
        }

        // Clear the lookup if there wasn't a collision chain,
        // otherwise remove the link in the chain.
        if (prev == NULL) {
            BUCKET_LOOKUP(table, hash) = NEXT_BUCKET(bucket);
        } else {
            NEXT_BUCKET(prev) = NEXT_BUCKET(bucket);
        }

        //
        DELETE_BUCKET(bucket);

        // If we're removing the last bucket, we might be able to move the
        // next open slot back if preceeding buckets are also deleted.
        if (index == table->next - 1 && table->size > 1) {
            do {
                table->next--;
                bucket--;
            }
            while (BUCKET_DELETED(bucket));
        }

        //
        table->min_deleted = MIN(index, table->min_deleted);

        //
        if ((--table->size) <= table->capacity >> 2) {
            htable_halve_capacity(table);
        }

        return SUCCESS;
    }

    if (return_value) {
        ZVAL_NULL(return_value);
    }

    return FAILURE;
}

HTable *htable_slice(HTable *table, zend_long index, zend_long length)
{
    normalize_slice_params(&index, &length, table->size);

    if (length == 0) {
        return htable_init();

    } else {
        HTable *slice = htable_init_ex(length);

        /**
         * If the table doesn't have any deleted buckets or if the first deleted
         * bucket comes after the slice we're after, we can safely seek
         * to the index and copy each bucket. We don't have to worry about
         * increasing capacity or checking if a key already exists.
         */
        if (HTABLE_IS_PACKED(table) || (index + length) <= table->min_deleted) {
            HBucket *src = &table->buckets[index];

            for (; length-- > 0; src++) {
                HBucket *dst = init_next_bucket(slice, &src->key, BUCKET_HASH(src));
                ZVAL_COPY(&dst->value, &src->value);
            }

        /**
         * If the table does have deleted buckets but the first one comes after
         * the index of the slice, we can safely seek to the index.
         */
        } else if (index < table->min_deleted) {
            HBucket *src = &table->buckets[index];

            for (;;) {
                HBucket *dst = init_next_bucket(slice, &src->key, BUCKET_HASH(src));
                ZVAL_COPY(&dst->value, &src->value);

                if (--length == 0) {
                    break;
                }

                // Skip deleted buckets
                while (BUCKET_DELETED(++src));
            }

        /**
         * Otherwise there are deleted buckets but they're in the slice range.
         * The best we can do is just skip any deleted buckets we encounter.
         */
        } else {
            uint32_t pos = 0;
            HBucket *src = table->buckets;

            // We have to seek iteratively until we reach the index
            while (pos < index) {
                if (BUCKET_DELETED(src++)) {
                    continue;
                }
                pos++;
            }

            // We're at the index, so gather across.
            for (; length > 0; src++) {
                if (BUCKET_DELETED(src)) {
                    continue;
                }

                HBucket *dst = init_next_bucket(slice, &src->key, BUCKET_HASH(src));
                ZVAL_COPY(&dst->value, &src->value);
                length--;
            }
        }

        return slice;
    }
}

HTable *htable_map(HTable *table, FCI_PARAMS)
{
    zval params[2];
    zval retval;

    HTable *clone = htable_clone(table);

    HBucket *end = table->buckets + table->next;
    HBucket *src = table->buckets;
    HBucket *dst = clone->buckets;

    for (; src != end; ++src, ++dst) {
        if (BUCKET_DELETED(src)) {
            continue;
        }

        ZVAL_COPY_VALUE(&params[0], &src->key);
        ZVAL_COPY_VALUE(&params[1], &src->value);

        fci.param_count = 2;
        fci.params      = params;
        fci.retval      = &retval;

        if (zend_call_function(&fci, &fci_cache) == FAILURE || Z_ISUNDEF(retval)) {
            htable_destroy(clone);
            return NULL;
        } else {
            zval_ptr_dtor(&dst->value);
            ZVAL_COPY(&dst->value, &retval);
        }
    }

    return clone;
}

HTable *htable_filter_callback(HTable *table, FCI_PARAMS)
{
    HBucket *bucket;

    zval params[2];
    zval retval;

    HTable *clone = htable_init_ex(table->capacity);

    HTABLE_FOREACH_BUCKET(table, bucket) {
        ZVAL_COPY_VALUE(&params[0], &bucket->key);
        ZVAL_COPY_VALUE(&params[1], &bucket->value);

        fci.param_count = 2;
        fci.params      = params;
        fci.retval      = &retval;

        if (zend_call_function(&fci, &fci_cache) == FAILURE || Z_ISUNDEF(retval)) {
            htable_destroy(clone);
            return NULL;
        } else {
            if (zend_is_true(&retval)) {
                HBucket *dst = init_next_bucket(clone, &bucket->key, BUCKET_HASH(bucket));
                ZVAL_COPY(&dst->value, &bucket->value);
            }
        }
    }
    HTABLE_FOREACH_END();
    return clone;
}

void htable_reduce(HTable *table, FCI_PARAMS, zval *initial, zval *return_value)
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

    HTABLE_FOREACH_KEY_VALUE(table, key, value) {
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
    HTABLE_FOREACH_END();

    ZVAL_COPY(return_value, &carry);
}

HTable *htable_xor(HTable *table, HTable *other)
{
    HBucket *bucket;
    HTable *xor = htable_init();

    HTABLE_FOREACH_BUCKET(table, bucket) {
        if ( ! htable_has_key(other, &bucket->key)) {
            htable_put_next(xor, bucket);
        }
    }
    HTABLE_FOREACH_END();

    HTABLE_FOREACH_BUCKET(other, bucket) {
        if ( ! htable_has_key(table, &bucket->key)) {
            htable_put_next(xor, bucket);
        }
    }
    HTABLE_FOREACH_END();

    return xor;
}

HTable *htable_diff(HTable *table, HTable *other)
{
    HBucket *bucket;
    HTable *diff = htable_init();

    HTABLE_FOREACH_BUCKET(table, bucket) {
        if ( ! htable_has_key(other, &bucket->key)) {
            htable_put_next(diff, bucket);
        }
    }
    HTABLE_FOREACH_END();

    return diff;
}

HTable *htable_intersect(HTable *table, HTable *other)
{
    HBucket *bucket;
    HTable *intersection = htable_init();

    HTABLE_FOREACH_BUCKET(table, bucket) {
        if (htable_has_key(other, &bucket->key)) {
            htable_put_next(intersection, bucket);
        }
    }
    HTABLE_FOREACH_END();

    return intersection;
}

HTable *htable_merge(HTable *table, HTable *other)
{
    HBucket *bucket;
    HTable *merged = htable_clone(table);

    HTABLE_FOREACH_BUCKET(other, bucket) {
        htable_put(merged, &bucket->key, &bucket->value);
    }
    HTABLE_FOREACH_END();

    return merged;
}

HBucket *htable_last(HTable *table)
{
    if (table->size == 0) {
        return NULL;

    } else {
        HBucket *last = &table->buckets[table->next - 1];

        if ( ! HTABLE_IS_PACKED(table)) {
            while (BUCKET_DELETED(last)) {
                last--;
            }
        }

        return last;
    }
}

HBucket *htable_first(HTable *table)
{
    if (table->size == 0) {
        return NULL;

    } else {
        HBucket *first = table->buckets;

        if (table->min_deleted > 0) {
            while (BUCKET_DELETED(first)) {
                first++;
            }
        }

        return first;
    }
}

void htable_reverse(HTable *table)
{
    pack_table(table);
    {
        HBucket *a = table->buckets;
        HBucket *b = table->buckets + table->size - 1;

        for (; a < b; ++a, --b) {
            HBucket c = *a;
            *a = *b;
            *b = c;
        }
    }
    rehash_table(table);
}

HTable *htable_reversed(HTable *table)
{
    HTable *reversed = htable_init_ex(table->capacity);

    HBucket *src = NULL;
    HBucket *dst = reversed->buckets;

    const uint32_t mask = reversed->capacity - 1;

    HTABLE_FOREACH_BUCKET_REVERSED(table, src) {
        uint32_t *lookup = &reversed->lookup[BUCKET_HASH(src) & mask];

        COPY_BUCKET(dst, src);
        NEXT_BUCKET(dst) = *lookup;
        *lookup = reversed->next++;
        dst++;
    }
    HTABLE_FOREACH_END();

    reversed->size = table->size;
    return reversed;
}

void htable_to_array(HTable *table, zval *arr)
{
    zval *key;
    zval *val;

    array_init_size(arr, table->size);

    HTABLE_FOREACH_KEY_VALUE(table, key, val) {
        array_set_zval_key(Z_ARR_P(arr), key, val);
    }
    HTABLE_FOREACH_END();
}

HashTable *htable_pairs_to_php_ht(HTable *table)
{
    HashTable *ht;

    zval *key;
    zval *val;

    zval pair;

    uint32_t pos;

    ALLOC_HASHTABLE(ht);
    zend_hash_init(ht, table->size, NULL, ZVAL_PTR_DTOR, 0);

    HTABLE_FOREACH(table, pos, key, val) {

        array_init_size(&pair, 2);

        add_next_index_zval(&pair, key);
        add_next_index_zval(&pair, val);

        Z_TRY_ADDREF_P(key);
        Z_TRY_ADDREF_P(val);

        zend_hash_next_index_insert(ht, &pair);
    }
    HTABLE_FOREACH_END();

    return ht;
}

int htable_serialize(HTable *table, unsigned char **buffer, size_t *length, zend_serialize_data *data)
{
    php_serialize_data_t serialize_data = (php_serialize_data_t) data;
    PHP_VAR_SERIALIZE_INIT(serialize_data);

    if (table->size == 0) {
        SERIALIZE_SET_ZSTR(ZSTR_EMPTY_ALLOC());

    } else {

        zval *key, *value;

        smart_str buf = {0};

        HTABLE_FOREACH_KEY_VALUE(table, key, value) {
            php_var_serialize(&buf, key, &serialize_data);
            php_var_serialize(&buf, value, &serialize_data);
        }
        HTABLE_FOREACH_END();

        smart_str_0(&buf);

        SERIALIZE_SET_ZSTR(buf.s);
        zend_string_release(buf.s);
    }

    PHP_VAR_SERIALIZE_DESTROY(serialize_data);
    return SUCCESS;
}

int htable_unserialize(HTable *table, const unsigned char *buffer, size_t length, zend_unserialize_data *data)
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

        htable_put(table, key, value);
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
