// #include "../common.h"

#include "../php/iterators/php_vector_iterator.h"
#include "../php/handlers/php_vector_handlers.h"
#include "../php/classes/php_vector_ce.h"

#include "ds_vector.h"

static inline bool index_out_of_range(zend_long index, zend_long max)
{
    if (index < 0 || index >= max) {
        INDEX_OUT_OF_RANGE(index, max);
        return true;
    }
    return false;
}

static inline void ds_vector_reallocate(ds_vector_t *vector, zend_long capacity)
{
    REALLOC_ZVAL_BUFFER(vector->buffer, capacity);
    vector->capacity = capacity;
}

ds_vector_t *ds_vector_ex(zend_long capacity)
{
    ds_vector_t *vector = ecalloc(1, sizeof(ds_vector_t));

    // Make sure that capacity is valid.
    capacity = MAX(capacity, DS_VECTOR_MIN_CAPACITY);

    vector->buffer   = ALLOC_ZVAL_BUFFER(capacity);
    vector->capacity = capacity;
    vector->size     = 0;

    return vector;
}

ds_vector_t *ds_vector()
{
    return ds_vector_ex(DS_VECTOR_MIN_CAPACITY);
}

static ds_vector_t *ds_vector_from_buffer_ex(
    zval *buffer,
    zend_long size,
    zend_long capacity
) {
    ds_vector_t *vector = ecalloc(1, sizeof(ds_vector_t));
    vector->buffer      = buffer;
    vector->capacity    = capacity;
    vector->size        = size;

    return vector;
}

ds_vector_t *ds_vector_clone(ds_vector_t *vector)
{
    if (DS_VECTOR_IS_EMPTY(vector)) {
        return ds_vector();

    } else {
        ds_vector_t *clone = ecalloc(1, sizeof(ds_vector_t));

        clone->buffer   = ALLOC_ZVAL_BUFFER(vector->capacity);
        clone->capacity = vector->capacity;
        clone->size     = vector->size;

        COPY_ZVAL_BUFFER(clone->buffer, vector->buffer, vector->size);
        return clone;
    }
}

ds_vector_t *ds_vector_from_buffer(zval *buffer, zend_long size)
{
    zend_long capacity = size;

    if (size < DS_VECTOR_MIN_CAPACITY) {
        capacity = DS_VECTOR_MIN_CAPACITY;
        REALLOC_ZVAL_BUFFER(buffer, capacity);
    }

    return ds_vector_from_buffer_ex(buffer, size, capacity);
}

void ds_vector_allocate(ds_vector_t *vector, zend_long capacity)
{
    if (capacity > vector->capacity) {
        ds_vector_reallocate(vector, capacity);
    }
}

static inline void ds_vector_increase_capacity(ds_vector_t *vector)
{
    ds_vector_reallocate(vector, vector->capacity + (vector->capacity / 2));
}

static inline void ds_vector_ensure_capacity(ds_vector_t *vector, zend_long capacity)
{
    if (capacity > vector->capacity) {
        zend_long boundary = vector->capacity + (vector->capacity / 2);
        ds_vector_reallocate(vector, MAX(capacity, boundary));
    }
}

static inline void ds_vector_check_compact(ds_vector_t *vector)
{
    const zend_long c = vector->capacity;
    const zend_long n = vector->size;

    if (n < c / 4 && c / 2 > DS_VECTOR_MIN_CAPACITY) {
        ds_vector_reallocate(vector, c / 2);
    }
}

void ds_vector_remove(ds_vector_t *vector, zend_long index, zval *return_value)
{
    if (index_out_of_range(index, vector->size)) {
        return;
    }

    if (index == vector->size - 1) {
        ds_vector_pop(vector, return_value);

    } else {
        zval *pos = vector->buffer + index;

        if (return_value) {
            ZVAL_COPY(return_value, pos);
        }

        if ( ! Z_ISUNDEF_P(pos)) {
            zval_ptr_dtor(pos);
        }

        memmove(pos, pos + 1, sizeof(zval) * (vector->size - index));
        vector->size--;

        ds_vector_check_compact(vector);
    }
}

zval *ds_vector_get(ds_vector_t *vector, zend_long index)
{
    if (index_out_of_range(index, vector->size)) {
        return NULL;
    }

    return vector->buffer + index;
}

static inline void increase_capacity_if_full(ds_vector_t *vector)
{
    if (vector->size == vector->capacity) {
        ds_vector_increase_capacity(vector);
    }
}

static inline void ds_vector_clear_buffer(ds_vector_t *vector)
{
    zval *pos = vector->buffer;
    zval *end = vector->buffer + vector->size;

    for (; pos != end; ++pos) {
        DTOR_AND_UNDEF(pos);
    }

    vector->size = 0;
}

void ds_vector_clear(ds_vector_t *vector)
{
    if (vector->size > 0) {
        ds_vector_clear_buffer(vector);

        if (vector->capacity > DS_VECTOR_MIN_CAPACITY) {
            ds_vector_reallocate(vector, DS_VECTOR_MIN_CAPACITY);
        }
    }
}

void ds_vector_set(ds_vector_t *vector, zend_long index, zval *value)
{
    if ( ! index_out_of_range(index, vector->size)) {
        zval *ptr = vector->buffer + index;
        zval_ptr_dtor(ptr);
        ZVAL_COPY(ptr, value);
    }
}

/**
 *
 */
void ds_vector_to_array(ds_vector_t *vector, zval *return_value)
{
    zend_long size = vector->size;

    if (size == 0) {
        array_init(return_value);

    } else {
        zval *pos = vector->buffer;
        zval *end = pos + size;

        array_init_size(return_value, size);

        for (; pos != end; ++pos) {
            add_next_index_zval(return_value, pos);
            Z_TRY_ADDREF_P(pos);
        }
    }
}

static inline zend_long ds_vector_find_index(ds_vector_t *vector, zval *value)
{
    zval *pos = vector->buffer;
    zval *end = vector->buffer + vector->size;

    for (; pos != end; ++pos) {
        if (zend_is_identical(value, pos)) {
            return pos - vector->buffer;
        }
    }

    return FAILURE;
}

void ds_vector_find(ds_vector_t *vector, zval *value, zval *return_value)
{
    zend_long index = ds_vector_find_index(vector, value);

    if (index >= 0) {
        ZVAL_LONG(return_value, index);
        return;
    }

    ZVAL_FALSE(return_value);
}

bool ds_vector_contains(ds_vector_t *vector, zval *value)
{
    return ds_vector_find_index(vector, value) != FAILURE;
}

bool ds_vector_contains_va(ds_vector_t *vector, VA_PARAMS)
{
    while (argc-- > 0) {
        if ( ! ds_vector_contains(vector, argv++)) {
            return false;
        }
    }

    return true;
}

void ds_vector_join(ds_vector_t *vector, char *str, size_t len, zval *return_value)
{
    zend_string *s;
    s = ds_join_zval_buffer(vector->buffer, DS_VECTOR_SIZE(vector), str, len);
    ZVAL_STR(return_value, s);
}

void ds_vector_insert_va(ds_vector_t *vector, zend_long index, VA_PARAMS)
{
    if ( ! index_out_of_range(index, vector->size + 1) && argc > 0) {
        zend_long len;
        zval *src;
        zval *dst;
        zval *end;

        ds_vector_ensure_capacity(vector, vector->size + argc);

        src = argv;
        dst = vector->buffer + index;
        end = dst + argc;
        len = vector->size - index;

        if (len > 0) {
            memmove(end, dst, (vector->size - index) * sizeof(zval));
        }

        for (; dst != end; ++dst, ++src) {
            ZVAL_COPY(dst, src);
        }

        vector->size += argc;
    }
}

void ds_vector_insert(ds_vector_t *vector, zend_long index, zval *value)
{
    ds_vector_insert_va(vector, index, 1, value);
}

void ds_vector_push(ds_vector_t *vector, zval *value)
{
    increase_capacity_if_full(vector);
    ZVAL_COPY(&vector->buffer[vector->size++], value);
}

void ds_vector_push_va(ds_vector_t *vector, VA_PARAMS)
{
    if (argc == 1) {
        ds_vector_push(vector, argv);
        return;
    }

    if (argc > 0) {
        zval *src, *dst, *end;

        ds_vector_ensure_capacity(vector, vector->size + argc);

        src = argv;
        dst = &vector->buffer[vector->size];
        end = dst + argc;

        while (dst != end) {
            ZVAL_COPY(dst++, src++);
        }

        vector->size += argc;
    }
}

void ds_vector_unshift(ds_vector_t *vector, zval *value)
{
    ds_vector_insert(vector, 0, value);
}

void ds_vector_unshift_va(ds_vector_t *vector, VA_PARAMS)
{
    if (argc == 1) {
        ds_vector_unshift(vector, argv);
        return;
    }

    if (argc > 0) {
        zval *dst, *src, *end;

        ds_vector_ensure_capacity(vector, vector->size + argc);

        src = argv;
        dst = vector->buffer;
        end = dst + argc;

        memmove(end, dst, vector->size * sizeof(zval));

        while (dst != end) {
            ZVAL_COPY(dst++, src++);
        }

        vector->size += argc;
    }
}

void ds_vector_sort_callback(ds_vector_t *vector)
{
    ds_user_sort_zval_buffer(vector->buffer, vector->size);
}

void ds_vector_sort(ds_vector_t *vector)
{
    ds_sort_zval_buffer(vector->buffer, vector->size);
}

bool ds_vector_isset(ds_vector_t *vector, zend_long index, int check_empty)
{
    if (index < 0 || index >= vector->size) {
        return 0;
    }

    return ds_zval_isset(vector->buffer + index, check_empty);
}

bool ds_vector_index_exists(ds_vector_t *vector, zend_long index)
{
    return index >= 0 && index < vector->size;
}

static int iterator_add(zend_object_iterator *iterator, void *puser)
{
    ds_vector_push((ds_vector_t *) puser, iterator->funcs->get_current_data(iterator));
    return ZEND_HASH_APPLY_KEEP;
}

static inline void add_traversable_to_vector(ds_vector_t *vector, zval *obj)
{
    spl_iterator_apply(obj, iterator_add, (void*) vector);
}

static inline void add_array_to_vector(ds_vector_t *vector, HashTable *array)
{
    zval *value;
    ds_vector_ensure_capacity(vector, vector->size + array->nNumOfElements);

    ZEND_HASH_FOREACH_VAL(array, value) {
        ds_vector_push(vector, value);
    }
    ZEND_HASH_FOREACH_END();
}

void ds_vector_rotate(ds_vector_t *vector, zend_long r)
{
    zval *a, *b, *c;

    zend_long n = vector->size;

         // Negative rotation should rotate in the opposite direction
         if (r < 0) r = n - (llabs(r) % n);
    else if (r > n) r = r % n;

    // There's no need to rotate if the sequence won't be affected.
    if (r == 0 || r == n) return;

    a = vector->buffer; // Start of buffer
    b = a + r;          // Pivot
    c = a + n;          // End of buffer
                        // [a..b....c]

    ds_reverse_zval_range(a, b);
    ds_reverse_zval_range(b, c);
    ds_reverse_zval_range(a, c);
}

void ds_vector_push_all(ds_vector_t *vector, zval *values)
{
    if ( ! values) {
        return;
    }

    if (ds_is_array(values)) {
        add_array_to_vector(vector, Z_ARRVAL_P(values));
        return;
    }

    if (ds_is_traversable(values)) {
        add_traversable_to_vector(vector, values);
        return;
    }

    ARRAY_OR_TRAVERSABLE_REQUIRED();
}

ds_vector_t *ds_vector_merge(ds_vector_t *vector, zval *values)
{
    if (values && (ds_is_array(values) || ds_is_traversable(values))) {
        ds_vector_t *merged = ds_vector_clone(vector);
        ds_vector_push_all(merged, values);
        return merged;
    }

    ARRAY_OR_TRAVERSABLE_REQUIRED();
    return NULL;
}

void ds_vector_pop(ds_vector_t *vector, zval *return_value)
{
    zval *value = &vector->buffer[--vector->size];

    if (return_value) {
        ZVAL_COPY_VALUE(return_value, value);
        ZVAL_UNDEF(value);
    } else {
        DTOR_AND_UNDEF(value);
    }

    ds_vector_check_compact(vector);
}

void ds_vector_pop_throw(ds_vector_t *vector, zval *return_value)
{
    if (DS_VECTOR_IS_EMPTY(vector)) {
        NOT_ALLOWED_WHEN_EMPTY();
        return;
    }

    ds_vector_pop(vector, return_value);
}

void ds_vector_shift(ds_vector_t *vector, zval *return_value)
{
    zval *first = vector->buffer;

    if (return_value) {
        ZVAL_COPY_VALUE(return_value, first);
        ZVAL_UNDEF(first);
    } else {
        DTOR_AND_UNDEF(first);
    }

    vector->size--;
    memmove(first, first + 1, vector->size * sizeof(zval));
    ds_vector_check_compact(vector);
}

void ds_vector_shift_throw(ds_vector_t *vector, zval *return_value)
{
    if (DS_VECTOR_IS_EMPTY(vector)) {
        NOT_ALLOWED_WHEN_EMPTY();
        return;
    }

    ds_vector_shift(vector, return_value);
}

zval *ds_vector_get_last(ds_vector_t *vector)
{
    return &vector->buffer[vector->size - 1];
}

zval *ds_vector_get_last_throw(ds_vector_t *vector)
{
    if (DS_VECTOR_IS_EMPTY(vector)) {
        NOT_ALLOWED_WHEN_EMPTY();
        return NULL;
    }

    return ds_vector_get_last(vector);
}

zval *ds_vector_get_first(ds_vector_t *vector)
{
    return &vector->buffer[0];
}

zval *ds_vector_get_first_throw(ds_vector_t *vector)
{
    if (DS_VECTOR_IS_EMPTY(vector)) {
        NOT_ALLOWED_WHEN_EMPTY();
        return NULL;
    }

    return ds_vector_get_first(vector);
}

void ds_vector_reverse(ds_vector_t *vector)
{
    ds_reverse_zval_range(vector->buffer, vector->buffer + vector->size);
}

ds_vector_t *ds_vector_reversed(ds_vector_t *vector)
{
    zval *src;
    zval *buf = ALLOC_ZVAL_BUFFER(vector->capacity);
    zval *dst = &buf[vector->size - 1];

    DS_VECTOR_FOREACH(vector, src) {
        ZVAL_COPY(dst, src);
        dst--;
    }
    DS_VECTOR_FOREACH_END();

    return ds_vector_from_buffer_ex(buf, vector->size, vector->capacity);
}

void ds_vector_apply(ds_vector_t *vector, FCI_PARAMS)
{
    zval *value;
    zval retval;

    DS_VECTOR_FOREACH(vector, value) {

        fci.param_count = 1;
        fci.params      = value;
        fci.retval      = &retval;

        if (zend_call_function(&fci, &fci_cache) == FAILURE || Z_ISUNDEF(retval)) {
            return;
        }

        zval_ptr_dtor(value);
        ZVAL_COPY_VALUE(value, &retval);
    }
    DS_VECTOR_FOREACH_END();
}

ds_vector_t *ds_vector_map(ds_vector_t *vector, FCI_PARAMS)
{
    zval *value;
    zval *buffer = ALLOC_ZVAL_BUFFER(vector->size);
    zval *target = buffer;

    DS_VECTOR_FOREACH(vector, value) {
        zval param;
        zval retval;

        ZVAL_COPY_VALUE(&param, value);

        fci.param_count = 1;
        fci.params      = &param;
        fci.retval      = &retval;

        if (zend_call_function(&fci, &fci_cache) == FAILURE || Z_ISUNDEF(retval)) {
            efree(buffer);
            return NULL;
        }

        ZVAL_COPY_VALUE(target++, &retval);
    }
    DS_VECTOR_FOREACH_END();

    return ds_vector_from_buffer_ex(buffer, vector->size, vector->capacity);
}

ds_vector_t *ds_vector_filter(ds_vector_t *vector)
{
    if (DS_VECTOR_IS_EMPTY(vector)) {
        return ds_vector();

    } else {
        zval *value;
        zval *buf = ALLOC_ZVAL_BUFFER(vector->size);
        zval *pos = buf;

        zend_long size = 0;

        DS_VECTOR_FOREACH(vector, value) {
            if (zend_is_true(value)) {
                ZVAL_COPY(pos++, value);
                size++;
            }
        }
        DS_VECTOR_FOREACH_END();

        return ds_vector_from_buffer_ex(buf, size, vector->size);
    }
}

ds_vector_t *ds_vector_filter_callback(ds_vector_t *vector, FCI_PARAMS)
{
    if (DS_VECTOR_IS_EMPTY(vector)) {
        return ds_vector();

    } else {
        zval *value;
        zval *buffer = ALLOC_ZVAL_BUFFER(vector->size);
        zval *target = buffer;

        DS_VECTOR_FOREACH(vector, value) {
            zval param;
            zval retval;

            ZVAL_COPY_VALUE(&param, value);

            fci.param_count = 1;
            fci.params      = &param;
            fci.retval      = &retval;

            // Catch potential exceptions or other errors during comparison.
            if (zend_call_function(&fci, &fci_cache) == FAILURE || Z_ISUNDEF(retval)) {
                efree(buffer);
                return NULL;
            }

            //
            if (zend_is_true(&retval)) {
                ZVAL_COPY(target, value);
                target++;
            }
        }
        DS_VECTOR_FOREACH_END();

        return ds_vector_from_buffer_ex(buffer, (target - buffer), vector->size);
    }
}

void ds_vector_reduce(ds_vector_t *vector, zval *initial, zval *return_value, FCI_PARAMS)
{
    zval *value;
    zval carry;
    zval params[2];

    if (initial == NULL) {
        ZVAL_NULL(&carry);
    } else {
        ZVAL_COPY_VALUE(&carry, initial);
    }

    DS_VECTOR_FOREACH(vector, value) {
        ZVAL_COPY_VALUE(&params[0], &carry);
        ZVAL_COPY_VALUE(&params[1], value);

        fci.param_count = 2;
        fci.params      = params;
        fci.retval      = &carry;

        if (zend_call_function(&fci, &fci_cache) == FAILURE || Z_ISUNDEF(carry)) {
            ZVAL_NULL(return_value);
            return;
        }

        Z_TRY_DELREF_P(&carry);
    }
    DS_VECTOR_FOREACH_END();
    ZVAL_COPY(return_value, &carry);
}

ds_vector_t *ds_vector_slice(ds_vector_t *vector, zend_long index, zend_long length)
{
    ds_normalize_slice_args(&index, &length, vector->size);

    if (length == 0) {
        return ds_vector();

    } else {
        zval *src, *dst, *end;
        zval *buffer = ALLOC_ZVAL_BUFFER(length);

        src = vector->buffer + index;
        dst = buffer;
        end = src + length;

        for (; src < end; ++src, ++dst) {
            ZVAL_COPY(dst, src);
        }

        return ds_vector_from_buffer(buffer, length);
    }
}

void ds_vector_sum(ds_vector_t *vector, zval *return_value)
{
    zval *value;

    ZVAL_LONG(return_value, 0);

    DS_VECTOR_FOREACH(vector, value) {
        DS_ADD_TO_SUM(value, return_value);
    }
    DS_VECTOR_FOREACH_END();
}

void ds_vector_free(ds_vector_t *vector)
{
    ds_vector_clear_buffer(vector);
    efree(vector->buffer);
    efree(vector);
}
