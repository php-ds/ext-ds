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

static inline void vector_reallocate(Vector *vector, zend_long capacity)
{
    REALLOC_ZVAL_BUFFER(vector->buffer, capacity);
    vector->capacity = capacity;
}

Vector *vector_init_ex(zend_long capacity)
{
    Vector *vector = ecalloc(1, sizeof(Vector));

    // Make sure that capacity is valid.
    capacity = MAX(capacity, VECTOR_MIN_CAPACITY);

    vector->buffer   = ALLOC_ZVAL_BUFFER(capacity);
    vector->capacity = capacity;
    vector->size     = 0;

    return vector;
}

static Vector *vector_from_buffer_ex(
    zval *buffer,
    zend_long size,
    zend_long capacity
) {
    Vector *vector   = ecalloc(1, sizeof(Vector));
    vector->buffer   = buffer;
    vector->capacity = capacity;
    vector->size     = size;

    return vector;
}

Vector *vector_from_buffer(zval *buffer, zend_long length)
{
    zend_long capacity = length;

    if (capacity < VECTOR_MIN_CAPACITY) {
        capacity = VECTOR_MIN_CAPACITY;
        REALLOC_ZVAL_BUFFER(buffer, capacity);

    } else if (length < capacity >> 2) {
        capacity = capacity >> 1;
        REALLOC_ZVAL_BUFFER(buffer, capacity);
    }

    return vector_from_buffer_ex(buffer, length, capacity);
}

Vector *vector_init()
{
    return vector_init_ex(VECTOR_MIN_CAPACITY);
}

zend_object *vector_create_object_ex(Vector *vector)
{
    VectorObj *obj = ecalloc(1, sizeof(VectorObj));
    zend_object_std_init(&obj->std, vector_ce);
    obj->std.handlers = &vector_object_handlers;
    obj->vector = vector;
    return &obj->std;
}

zend_object *vector_create_object(zend_class_entry *ce)
{
    return vector_create_object_ex(vector_init());
}

void vector_user_allocate(Vector *vector, zend_long capacity)
{
    if (capacity > vector->capacity) {
        vector_reallocate(vector, capacity);
    }
}

Vector *vector_create_copy(Vector *vector)
{
    if (VECTOR_IS_EMPTY(vector)) {
        return vector_init();

    } else {
        zend_long size     = vector->size;
        zend_long capacity = vector->capacity;

        Vector *copy = vector_init_ex(capacity);

        copy->buffer   = ALLOC_ZVAL_BUFFER(capacity);
        copy->capacity = capacity;
        copy->size     = size;

        COPY_ZVAL_BUFFER(copy->buffer, vector->buffer, size);

        return copy;
    }
}

zend_object *vector_create_clone(Vector *vector)
{
    return vector_create_object_ex(vector_create_copy(vector));
}

static inline void vector_increase_capacity(Vector *vector)
{
    vector_reallocate(vector, vector->capacity + (vector->capacity >> 1));
}

static inline void vector_ensure_capacity(Vector *vector, zend_long capacity)
{
    if (capacity > vector->capacity) {
        zend_long boundary = vector->capacity + (vector->capacity >> 1);
        vector_reallocate(vector, MAX(capacity, boundary));
    }
}

static inline void vector_check_compact(Vector *vector)
{
    if (vector->size < vector->capacity >> 2) {

        if (vector->capacity >> 1 > VECTOR_MIN_CAPACITY) {
            vector_reallocate(vector, vector->capacity >> 1);
        }
    }
}

void vector_remove(Vector *vector, zend_long index, zval *return_value)
{
    if (index_out_of_range(index, vector->size)) {
        return;
    }

    if (index == vector->size - 1) {
        vector_pop(vector, return_value);

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

        vector_check_compact(vector);
    }
}

zval *vector_get(Vector *vector, zend_long index)
{
    if (index_out_of_range(index, vector->size)) {
        return NULL;
    }

    return vector->buffer + index;
}

static inline void increase_capacity_if_full(Vector *vector)
{
    if (vector->size == vector->capacity) {
        vector_increase_capacity(vector);
    }
}

void vector_clear(Vector *vector)
{
    zval *pos = vector->buffer;
    zval *end = pos + vector->size;

    for (; pos != end; ++pos) {
        zval_ptr_dtor(pos);
    }

    vector->size = 0;
    vector_reallocate(vector, VECTOR_MIN_CAPACITY);
}

void vector_set(Vector *vector, zend_long index, zval *value)
{
    if ( ! index_out_of_range(index, vector->size)) {
        zval *current = vector->buffer + index;
        zval_ptr_dtor(current);
        ZVAL_COPY(current, value);
    }
}

void vector_to_array(Vector *vector, zval *return_value)
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

static inline zend_long vector_find_index(Vector *vector, zval *value)
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

void vector_find(Vector *vector, zval *value, zval *return_value)
{
    zend_long index = vector_find_index(vector, value);

    if (index >= 0) {
        ZVAL_LONG(return_value, index);
        return;
    }

    ZVAL_FALSE(return_value);
}

bool vector_contains(Vector *vector, zval *value)
{
    return vector_find_index(vector, value) != FAILURE;
}

bool vector_contains_va(Vector *vector, VA_PARAMS)
{
    if (argc == 0) return false;

    while (argc--) {
        if ( ! vector_contains(vector, argv++)) return false;
    }

    return true;
}

void vector_join(Vector *vector, char *str, size_t len, zval *return_value)
{
    zend_string *s;
    s = join_zval_buffer(vector->buffer, VECTOR_SIZE(vector), str, len);
    ZVAL_STR(return_value, s);
}

void vector_insert_va(Vector *vector, zend_long index, VA_PARAMS)
{
    if ( ! index_out_of_range(index, vector->size + 1) && argc > 0) {
        zend_long len;
        zval *src;
        zval *dst;
        zval *end;

        vector_ensure_capacity(vector, vector->size + argc);

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

void vector_insert(Vector *vector, zend_long index, zval *value)
{
    vector_insert_va(vector, index, 1, value);
}

void vector_push(Vector *vector, zval *value)
{
    increase_capacity_if_full(vector);
    ZVAL_COPY(&vector->buffer[vector->size], value);
    vector->size++;
}

void vector_push_va(Vector *vector, VA_PARAMS)
{
    if (argc == 1) {
        vector_push(vector, argv);
        return;
    }

    if (argc > 0) {
        zval *src, *dst, *end;

        vector_ensure_capacity(vector, vector->size + argc);

        src = argv;
        dst = &vector->buffer[vector->size];
        end = dst + argc;

        while (dst != end) {
            ZVAL_COPY(dst++, src++);
        }

        vector->size += argc;
    }
}

void vector_unshift(Vector *vector, zval *value)
{
    vector_insert(vector, 0, value);
}

void vector_unshift_va(Vector *vector, VA_PARAMS)
{
    if (argc == 1) {
        vector_unshift(vector, argv);
        return;
    }

    if (argc > 0) {
        zval *dst, *src, *end;

        vector_ensure_capacity(vector, vector->size + argc);

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

void vector_sort_callback(Vector *vector)
{
    user_sort_zval_buffer(vector->buffer, vector->size);
}

void vector_sort(Vector *vector)
{
    sort_zval_buffer(vector->buffer, vector->size);
}

bool vector_isset(Vector *vector, zend_long index, int check_empty)
{
    if (index < 0 || index >= vector->size) {
        return 0;
    }

    return zval_isset(vector->buffer + index, check_empty);
}

bool vector_index_exists(Vector *vector, zend_long index)
{
    return index >= 0 && index < vector->size;
}

static int iterator_add(zend_object_iterator *iterator, void *puser)
{
    vector_push((Vector *) puser, iterator->funcs->get_current_data(iterator));
    return SUCCESS;
}

static inline void add_traversable_to_vector(Vector *vector, zval *obj)
{
    spl_iterator_apply(obj, iterator_add, (void*) vector);
}

static inline void add_array_to_vector(Vector *vector, HashTable *array)
{
    zval *value;
    vector_ensure_capacity(vector, vector->size + array->nNumOfElements);

    ZEND_HASH_FOREACH_VAL(array, value) {
        vector_push(vector, value);
    }
    ZEND_HASH_FOREACH_END();
}

void vector_rotate(Vector *vector, zend_long r)
{
    zval *a, *b, *c;

    zend_long n = vector->size;

         // Negative rotation should rotate in the opposite direction
         if (r < 0) r = n - (llabs(r) % n);
    else if (r > n) r = r % n;

    // There's no need to rotate if the sequence won't be affected.
    if (r == 0 || r == n) return;

    a = vector->buffer;
    b = a + r;
    c = a + n;

    reverse_zval_range(a, b);
    reverse_zval_range(b, c);
    reverse_zval_range(a, c);
}

void vector_push_all(Vector *vector, zval *values)
{
    if ( ! values) {
        return;
    }

    if (Z_TYPE_P(values) == IS_ARRAY) {
        add_array_to_vector(vector, Z_ARRVAL_P(values));
        return;
    }

    if (is_traversable(values)) {
        add_traversable_to_vector(vector, values);
        return;
    }

    ARRAY_OR_TRAVERSABLE_REQUIRED();
}

void vector_pop(Vector *vector, zval *return_value)
{
    if (VECTOR_IS_EMPTY(vector)) {
        NOT_ALLOWED_WHEN_EMPTY();
        return;

    } else {
        zval *value = &vector->buffer[--vector->size];

        if (return_value) {
            ZVAL_COPY(return_value, value);
        }

        zval_ptr_dtor(value);
        vector_check_compact(vector);
    }
}

void vector_shift(Vector *vector, zval *return_value)
{
    zval *first = vector->buffer;

    if (VECTOR_IS_EMPTY(vector)) {
        NOT_ALLOWED_WHEN_EMPTY();
        return;
    }

    ZVAL_COPY(return_value, first);
    zval_ptr_dtor(first);

    memmove(first, first + 1, sizeof(zval) * (--vector->size));

    vector_check_compact(vector);
}

zval *vector_get_last(Vector *vector)
{
    if (VECTOR_IS_EMPTY(vector)) {
        NOT_ALLOWED_WHEN_EMPTY();
        return NULL;
    }

    return &vector->buffer[vector->size - 1];
}

zval *vector_get_first(Vector *vector)
{
    if (VECTOR_IS_EMPTY(vector)) {
        NOT_ALLOWED_WHEN_EMPTY();
        return NULL;
    }

    return &vector->buffer[0];
}

void vector_reverse(Vector *vector)
{
    reverse_zval_range(vector->buffer, vector->buffer + vector->size);
}

Vector *vector_reversed(Vector *vector)
{
    zval *src;
    zval *buf = ALLOC_ZVAL_BUFFER(vector->capacity);
    zval *dst = buf;

    VECTOR_FOREACH_REVERSED(vector, src) {
        ZVAL_COPY(dst, src);
        dst++;
    }
    VECTOR_FOREACH_END();

    return vector_from_buffer_ex(buf, vector->size, vector->capacity);
}

void vector_map(Vector *vector, zval *obj, FCI_PARAMS)
{
    zval *value;
    zval *buf = ALLOC_ZVAL_BUFFER(vector->size);
    zval *pos = buf;

    VECTOR_FOREACH(vector, value) {
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
    VECTOR_FOREACH_END();

    ZVAL_VECTOR(obj, vector_from_buffer_ex(buf, vector->size, vector->capacity));
}

void vector_filter(Vector *vector, zval *obj)
{
    if (VECTOR_IS_EMPTY(vector)) {
        ZVAL_NEW_VECTOR(obj);
        return;

    } else {
        zval *value;
        zval *buf = ALLOC_ZVAL_BUFFER(vector->size);
        zval *pos = buf;

        zend_long size = 0;

        VECTOR_FOREACH(vector, value) {
            if (zend_is_true(value)) {
                ZVAL_COPY(pos++, value);
                size++;
            }
        }
        VECTOR_FOREACH_END();

        ZVAL_VECTOR(obj, vector_from_buffer_ex(buf, size, vector->size));
    }
}

void vector_filter_callback(Vector *vector, zval *obj, FCI_PARAMS)
{
    if (VECTOR_IS_EMPTY(vector)) {
        ZVAL_NEW_VECTOR(obj);
        return;

    } else {
        zval *value;
        zval *buf = ALLOC_ZVAL_BUFFER(vector->size);
        zval *pos = buf;

        VECTOR_FOREACH(vector, value) {
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
            } else if (zend_is_true(&retval)) {
                ZVAL_COPY(pos++, value);
            }
        }
        VECTOR_FOREACH_END();

        ZVAL_VECTOR(obj, vector_from_buffer_ex(buf, (pos - buf), vector->size));
    }
}

void vector_reduce(Vector *vector, zval *initial, zval *return_value, FCI_PARAMS)
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

void vector_slice(Vector *vector, zend_long index, zend_long length, zval *obj)
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

        ZVAL_VECTOR(obj, vector_from_buffer(buffer, length));
    }
}

void vector_destroy(Vector *vector)
{
    vector_clear(vector);
    efree(vector->buffer);
    efree(vector);
}

int vector_serialize(zval *object, unsigned char **buffer, size_t *length, zend_serialize_data *data)
{
    Vector *vector = Z_VECTOR_P(object);

    php_serialize_data_t serialize_data = (php_serialize_data_t) data;
    PHP_VAR_SERIALIZE_INIT(serialize_data);

    if (VECTOR_IS_EMPTY(vector)) {
        SERIALIZE_SET_ZSTR(ZSTR_EMPTY_ALLOC());

    } else {
        zval *value;
        smart_str buf = {0};

        VECTOR_FOREACH(vector, value) {
            php_var_serialize(&buf, value, &serialize_data);
        }
        VECTOR_FOREACH_END();

        smart_str_0(&buf);
        SERIALIZE_SET_ZSTR(buf.s);
        zend_string_release(buf.s);
    }

    PHP_VAR_SERIALIZE_DESTROY(serialize_data);
    return SUCCESS;
}

int vector_unserialize(zval *obj, zend_class_entry *ce, const unsigned char *buffer, size_t length, zend_unserialize_data *data)
{
    Vector *vector = vector_init();

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

        vector_push(vector, value);
    }

    if (*(++pos) != '\0') {
        goto error;
    }

    ZVAL_VECTOR(obj, vector);
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    return SUCCESS;

error:
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    UNSERIALIZE_ERROR();
    return FAILURE;
}
