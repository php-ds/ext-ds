#include "../common.h"
#include "../iterators/php_vector_iterator.h"
#include "../handlers/php_vector_handlers.h"
#include "../classes/php_ce_vector.h"
#include "php_vector.h"

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

static ds_vector_t *ds_vector_from_buffer_ex(
    zval *buffer,
    zend_long size,
    zend_long capacity
) {
    ds_vector_t *vector   = ecalloc(1, sizeof(ds_vector_t));
    vector->buffer   = buffer;
    vector->capacity = capacity;
    vector->size     = size;

    return vector;
}

ds_vector_t *ds_vector_from_buffer(zval *buffer, zend_long length)
{
    zend_long capacity = length;

    if (capacity < DS_VECTOR_MIN_CAPACITY) {
        capacity = DS_VECTOR_MIN_CAPACITY;
        REALLOC_ZVAL_BUFFER(buffer, capacity);

    } else if (length < capacity >> 2) {
        capacity = capacity >> 1;
        REALLOC_ZVAL_BUFFER(buffer, capacity);
    }

    return ds_vector_from_buffer_ex(buffer, length, capacity);
}

ds_vector_t *ds_vector()
{
    return ds_vector_ex(DS_VECTOR_MIN_CAPACITY);
}

zend_object *php_ds_vector_ex(ds_vector_t *vector)
{
    php_ds_vector_t *obj = ecalloc(1, sizeof(php_ds_vector_t));
    zend_object_std_init(&obj->std, ds_vector_ce);
    obj->std.handlers = &php_ds_vector_handlers;
    obj->vector = vector;
    return &obj->std;
}

zend_object *php_ds_vector(zend_class_entry *ce)
{
    return php_ds_vector_ex(ds_vector());
}

void ds_vector_user_allocate(ds_vector_t *vector, zend_long capacity)
{
    if (capacity > vector->capacity) {
        ds_vector_reallocate(vector, capacity);
    }
}

ds_vector_t *ds_vector_create_copy(ds_vector_t *vector)
{
    if (DS_VECTOR_IS_EMPTY(vector)) {
        return ds_vector();

    } else {
        zend_long size     = vector->size;
        zend_long capacity = vector->capacity;

        ds_vector_t *copy = ds_vector_ex(capacity);

        copy->buffer   = ALLOC_ZVAL_BUFFER(capacity);
        copy->capacity = capacity;
        copy->size     = size;

        COPY_ZVAL_BUFFER(copy->buffer, vector->buffer, size);

        return copy;
    }
}

zend_object *ds_vector_create_clone(ds_vector_t *vector)
{
    return php_ds_vector_ex(ds_vector_create_copy(vector));
}

static inline void ds_vector_increase_capacity(ds_vector_t *vector)
{
    ds_vector_reallocate(vector, vector->capacity + (vector->capacity >> 1));
}

static inline void ds_vector_ensure_capacity(ds_vector_t *vector, zend_long capacity)
{
    if (capacity > vector->capacity) {
        zend_long boundary = vector->capacity + (vector->capacity >> 1);
        ds_vector_reallocate(vector, MAX(capacity, boundary));
    }
}

static inline void ds_vector_check_compact(ds_vector_t *vector)
{
    if (vector->size < vector->capacity >> 2) {

        if (vector->capacity >> 1 > DS_VECTOR_MIN_CAPACITY) {
            ds_vector_reallocate(vector, vector->capacity >> 1);
        }
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

void ds_vector_clear(ds_vector_t *vector)
{
    zval *pos = vector->buffer;
    zval *end = pos + vector->size;

    for (; pos != end; ++pos) {
        zval_ptr_dtor(pos);
    }

    vector->size = 0;
    ds_vector_reallocate(vector, DS_VECTOR_MIN_CAPACITY);
}

void ds_vector_set(ds_vector_t *vector, zend_long index, zval *value)
{
    if ( ! index_out_of_range(index, vector->size)) {
        zval *current = vector->buffer + index;
        zval_ptr_dtor(current);
        ZVAL_COPY(current, value);
    }
}

void ds_vector_to_array(ds_vector_t *vector, zval *return_value)
{
    zend_long n = vector->size;

    if (n == 0) {
        array_init(return_value);

    } else {
        zval *pos = vector->buffer;
        zval *end = pos + n;

        array_init_size(return_value, n);

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
    if (argc == 0) return false;

    while (argc--) {
        if ( ! ds_vector_contains(vector, argv++)) return false;
    }

    return true;
}

void ds_vector_join(ds_vector_t *vector, char *str, size_t len, zval *return_value)
{
    zend_string *s;
    s = join_zval_buffer(vector->buffer, DS_VECTOR_SIZE(vector), str, len);
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
    ZVAL_COPY(&vector->buffer[vector->size], value);
    vector->size++;
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
    user_sort_zval_buffer(vector->buffer, vector->size);
}

void ds_vector_sort(ds_vector_t *vector)
{
    sort_zval_buffer(vector->buffer, vector->size);
}

bool ds_vector_isset(ds_vector_t *vector, zend_long index, int check_empty)
{
    if (index < 0 || index >= vector->size) {
        return 0;
    }

    return zval_isset(vector->buffer + index, check_empty);
}

bool ds_vector_index_exists(ds_vector_t *vector, zend_long index)
{
    return index >= 0 && index < vector->size;
}

static int iterator_add(zend_object_iterator *iterator, void *puser)
{
    ds_vector_push((ds_vector_t *) puser, iterator->funcs->get_current_data(iterator));
    return SUCCESS;
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

    reverse_zval_range(a, b);
    reverse_zval_range(b, c);
    reverse_zval_range(a, c);
}

void ds_vector_push_all(ds_vector_t *vector, zval *values)
{
    if ( ! values) {
        return;
    }

    if (is_array(values)) {
        add_array_to_vector(vector, Z_ARRVAL_P(values));
        return;
    }

    if (is_traversable(values)) {
        add_traversable_to_vector(vector, values);
        return;
    }

    ARRAY_OR_TRAVERSABLE_REQUIRED();
}

void ds_vector_merge(ds_vector_t *vector, zval *values, zval *obj)
{
    if ( ! values) {
        return;
    }

    if (is_array(values) || is_traversable(values)) {
        ds_vector_t *merged = ds_vector_create_copy(vector);
        ds_vector_push_all(merged, values);
        ZVAL_DS_VECTOR(obj, merged);
        return;
    }

    ARRAY_OR_TRAVERSABLE_REQUIRED();
}

void ds_vector_pop(ds_vector_t *vector, zval *return_value)
{
    if (DS_VECTOR_IS_EMPTY(vector)) {
        NOT_ALLOWED_WHEN_EMPTY();
        return;

    } else {
        zval *value = &vector->buffer[--vector->size];

        if (return_value) {
            ZVAL_COPY(return_value, value);
        }

        zval_ptr_dtor(value);
        ds_vector_check_compact(vector);
    }
}

void ds_vector_shift(ds_vector_t *vector, zval *return_value)
{
    zval *first = vector->buffer;

    if (DS_VECTOR_IS_EMPTY(vector)) {
        NOT_ALLOWED_WHEN_EMPTY();
        return;
    }

    ZVAL_COPY(return_value, first);
    zval_ptr_dtor(first);

    memmove(first, first + 1, sizeof(zval) * (--vector->size));

    ds_vector_check_compact(vector);
}

zval *ds_vector_get_last(ds_vector_t *vector)
{
    if (DS_VECTOR_IS_EMPTY(vector)) {
        NOT_ALLOWED_WHEN_EMPTY();
        return NULL;
    }

    return &vector->buffer[vector->size - 1];
}

zval *ds_vector_get_first(ds_vector_t *vector)
{
    if (DS_VECTOR_IS_EMPTY(vector)) {
        NOT_ALLOWED_WHEN_EMPTY();
        return NULL;
    }

    return &vector->buffer[0];
}

void ds_vector_reverse(ds_vector_t *vector)
{
    reverse_zval_range(vector->buffer, vector->buffer + vector->size);
}

ds_vector_t *ds_vector_reversed(ds_vector_t *vector)
{
    zval *src;
    zval *buf = ALLOC_ZVAL_BUFFER(vector->capacity);
    zval *dst = buf;

    DS_VECTOR_FOREACH_REVERSED(vector, src) {
        ZVAL_COPY(dst, src);
        dst++;
    }
    DS_VECTOR_FOREACH_END();

    return ds_vector_from_buffer_ex(buf, vector->size, vector->capacity);
}

void ds_vector_map(ds_vector_t *vector, zval *obj, FCI_PARAMS)
{
    zval *value;
    zval *buf = ALLOC_ZVAL_BUFFER(vector->size);
    zval *pos = buf;

    DS_VECTOR_FOREACH(vector, value) {
        zval param;
        zval retval;

        ZVAL_COPY_VALUE(&param, value);

        fci.param_count = 1;
        fci.params      = &param;
        fci.retval      = &retval;

        if (zend_call_function(&fci, &fci_cache) == FAILURE || Z_ISUNDEF(retval)) {
            efree(buf);
            ZVAL_NULL(obj);
            return;
        } else {
            ZVAL_COPY(pos, &retval);
        }

        pos++;
    }
    DS_VECTOR_FOREACH_END();

    ZVAL_DS_VECTOR(obj, ds_vector_from_buffer_ex(buf, vector->size, vector->capacity));
}

void ds_vector_filter(ds_vector_t *vector, zval *obj)
{
    if (DS_VECTOR_IS_EMPTY(vector)) {
        ZVAL_NEW_VECTOR(obj);
        return;

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

        ZVAL_DS_VECTOR(obj, ds_vector_from_buffer_ex(buf, size, vector->size));
    }
}

void ds_vector_filter_callback(ds_vector_t *vector, zval *obj, FCI_PARAMS)
{
    if (DS_VECTOR_IS_EMPTY(vector)) {
        ZVAL_NEW_VECTOR(obj);
        return;

    } else {
        zval param;
        zval retval;

        zval *src;
        zval *ptr = ALLOC_ZVAL_BUFFER(vector->size);
        zval *pos = ptr;

        DS_VECTOR_FOREACH(vector, src) {
            ZVAL_COPY_VALUE(&param, src);
            fci.param_count = 1;
            fci.params      = &param;
            fci.retval      = &retval;

            // Catch potential exceptions or other errors during comparison.
            if (zend_call_function(&fci, &fci_cache) == FAILURE) {
                efree(ptr);
                ZVAL_UNDEF(obj);
                return;
            }

            // Only push if the value is not falsey.
            if (zend_is_true(&retval)) {
                ZVAL_COPY(pos++, src);
            }
        }
        DS_VECTOR_FOREACH_END();
        ZVAL_DS_VECTOR(obj, ds_vector_from_buffer_ex(ptr, (pos - ptr), vector->size));
    }
}

void ds_vector_reduce(ds_vector_t *vector, zval *initial, zval *return_value, FCI_PARAMS)
{
    zval carry;

    zval *pos = vector->buffer;
    zval *end = pos + vector->size;

    if (initial == NULL) {
        ZVAL_NULL(&carry);
    } else {
        ZVAL_COPY_VALUE(&carry, initial);
    }

    for (; pos < end; ++pos) {
        zval params[2];
        zval retval;

        ZVAL_COPY_VALUE(&params[0], &carry);
        ZVAL_COPY_VALUE(&params[1], pos);

        fci.param_count = 2;
        fci.params      = params;
        fci.retval      = &retval;

        if (zend_call_function(&fci, &fci_cache) == FAILURE || Z_ISUNDEF(retval)) {
            ZVAL_NULL(return_value);
            return;
        } else {
            ZVAL_COPY_VALUE(&carry, &retval);
        }
    }

    ZVAL_COPY(return_value, &carry);
}

void ds_vector_slice(ds_vector_t *vector, zend_long index, zend_long length, zval *obj)
{
    normalize_slice_params(&index, &length, vector->size);

    if (length == 0) {
        ZVAL_NEW_VECTOR(obj);
        return;

    } else {
        zval *src, *dst, *end;
        zval *buffer = ALLOC_ZVAL_BUFFER(length);

        src = vector->buffer + index;
        dst = buffer;
        end = src + length;

        for (; src < end; ++src, ++dst) {
            ZVAL_COPY(dst, src);
        }

        ZVAL_DS_VECTOR(obj, ds_vector_from_buffer(buffer, length));
    }
}

void ds_vector_destroy(ds_vector_t *vector)
{
    ds_vector_clear(vector);
    efree(vector->buffer);
    efree(vector);
}

int ds_vector_serialize(zval *object, unsigned char **buffer, size_t *length, zend_serialize_data *data)
{
    ds_vector_t *vector = Z_DS_VECTOR_P(object);

    php_serialize_data_t serialize_data = (php_serialize_data_t) data;
    PHP_VAR_SERIALIZE_INIT(serialize_data);

    if (DS_VECTOR_IS_EMPTY(vector)) {
        SERIALIZE_SET_ZSTR(ZSTR_EMPTY_ALLOC());

    } else {
        zval *value;
        smart_str buf = {0};

        DS_VECTOR_FOREACH(vector, value) {
            php_var_serialize(&buf, value, &serialize_data);
        }
        DS_VECTOR_FOREACH_END();

        smart_str_0(&buf);
        SERIALIZE_SET_ZSTR(buf.s);
        zend_string_release(buf.s);
    }

    PHP_VAR_SERIALIZE_DESTROY(serialize_data);
    return SUCCESS;
}

int ds_vector_unserialize(zval *obj, zend_class_entry *ce, const unsigned char *buffer, size_t length, zend_unserialize_data *data)
{
    ds_vector_t *vector = ds_vector();

    php_unserialize_data_t unserialize_data = (php_unserialize_data_t) data;

    const unsigned char *pos = buffer;
    const unsigned char *max = buffer + length;

    PHP_VAR_UNSERIALIZE_INIT(unserialize_data);

    while (*pos != '}') {
        zval *value = var_tmp_var(&unserialize_data);

        if (php_var_unserialize(value, &pos, max, &unserialize_data)) {
            var_push_dtor(&unserialize_data, value);
        } else {
            goto error;
        }

        ds_vector_push(vector, value);
    }

    if (*(++pos) != '\0') {
        goto error;
    }

    ZVAL_DS_VECTOR(obj, vector);
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    return SUCCESS;

error:
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    UNSERIALIZE_ERROR();
    return FAILURE;
}
