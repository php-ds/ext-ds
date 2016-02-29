#include "../common.h"
#include "../iterators/php_deque_iterator.h"
#include "../handlers/php_deque_handlers.h"
#include "../classes/php_ce_deque.h"
#include "php_deque.h"

#define IS_POWER_OF_2(n) (n && !(n & (n - 1)))

#define increment_head_ex(d, mask) (d)->head = ((d)->head + 1) & (mask)
#define decrement_head_ex(d, mask) (d)->head = ((d)->head - 1) & (mask)
#define increment_tail_ex(d, mask) (d)->tail = ((d)->tail + 1) & (mask)
#define decrement_tail_ex(d, mask) (d)->tail = ((d)->tail - 1) & (mask)

#define increment_head(d) increment_head_ex(d, (d)->capacity - 1)
#define decrement_head(d) decrement_head_ex(d, (d)->capacity - 1)
#define increment_tail(d) increment_tail_ex(d, (d)->capacity - 1)
#define decrement_tail(d) decrement_tail_ex(d, (d)->capacity - 1)

static inline zend_long capacity_for_size(zend_long n)
{
    // We have to allocate an extra element for the tail.
    // n--;

    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n++;

    return MAX(DEQUE_MIN_CAPACITY, n);
}

Deque *deque_init_ex(zend_long capacity)
{
    Deque *deque = ecalloc(1, sizeof(Deque));

    if (capacity < DEQUE_MIN_CAPACITY) {
        capacity = DEQUE_MIN_CAPACITY;
    } else {
        if ( ! IS_POWER_OF_2(capacity)) {
            capacity = capacity_for_size(capacity);
        }
    }

    deque->buffer   = ALLOC_ZVAL_BUFFER(capacity);
    deque->capacity = capacity;
    deque->head     = 0;
    deque->tail     = 0;

    return deque;
}

Deque *deque_init()
{
    return deque_init_ex(DEQUE_MIN_CAPACITY);
}

zend_object *deque_create_object_ex(Deque *deque)
{
    DequeObj *obj = ecalloc(1, sizeof(DequeObj));
    zend_object_std_init(&obj->std, deque_ce);
    obj->std.handlers = &deque_handlers;
    obj->deque = deque;
    return &obj->std;
}

zend_object *deque_create_object(zend_class_entry *ce)
{
    return deque_create_object_ex(deque_init());
}

static inline bool index_out_of_range(zend_long index, zend_long max)
{
    if (index < 0 || index >= max) {
        INDEX_OUT_OF_RANGE(index, max);
        return true;
    }

    return false;
}

static Deque *deque_from_buffer_ex(
    zval *buffer,
    zend_long size,
    zend_long capacity
) {
    Deque *deque = ecalloc(1, sizeof(Deque));

    if (size < capacity >> 2) {
        capacity = capacity >> 1;
        REALLOC_ZVAL_BUFFER(buffer, capacity);
    }

    deque->buffer   = buffer;
    deque->capacity = capacity;
    deque->head     = 0;
    deque->tail     = size;
    return deque;
}

Deque *deque_from_buffer(zval *buffer, zend_long size)
{
    return deque_from_buffer_ex(buffer, size, capacity_for_size(size));
}

static inline void deque_copy(Deque *src, Deque *dst)
{
    zend_long tail = src->tail;
    zend_long head = src->head;
    zend_long mask = src->capacity - 1;

    for (; head != tail; head = (head + 1) & mask) {
        ZVAL_COPY(&dst->buffer[head], &src->buffer[head]);
    }
}

Deque *deque_create_copy(Deque *deque)
{
    Deque *cloned = ecalloc(1, sizeof(Deque));

    cloned->buffer   = ALLOC_ZVAL_BUFFER(deque->capacity);
    cloned->capacity = deque->capacity;
    cloned->head     = deque->head;
    cloned->tail     = deque->tail;

    deque_copy(deque, cloned);
    return cloned;
}

zend_object *deque_create_clone(Deque *deque)
{
    return deque_create_object_ex(deque_create_copy(deque));
}

static zval *create_buffer(Deque *deque, zend_long capacity)
{
    zval *src = deque->buffer;
    zval *dst = ALLOC_ZVAL_BUFFER(capacity);

    zend_long h = deque->head;
    zend_long t = deque->tail;

    // If the head comes before the tail, we know that the head hasn't
    // wrapped around. [_, h, ..., t, _]
    if (h < t) {
        memcpy(&dst[0], &src[h], DEQUE_SIZE(deque) * sizeof(zval));

    } else {
        // The head of the deque has wrapped around,
        // [..., t, _, h, ...]

        // Number of buffer to the right of the head, including the head.
        zend_long r = deque->capacity - h;

        // Wrapped around, so copy the wrapped buffer, then the rest.
        memcpy(&dst[0], &src[h], r * sizeof(zval));
        memcpy(&dst[r], &src[0], t * sizeof(zval));
    }

    return dst;
}

// Makes sure that the deque's head is at index 0.
static void deque_pack(Deque *deque)
{
    if (deque->head > 0) {
        zend_long size = DEQUE_SIZE(deque);

        if (deque->head < deque->tail) {
            memmove(deque->buffer, deque->buffer + deque->head, size * sizeof(zval));

        } else {
            zend_long h = deque->head;
            zend_long t = deque->tail;
            zend_long r = deque->capacity - h;

            memmove(&deque->buffer[r], &deque->buffer[0], t * sizeof(zval));
            memmove(&deque->buffer[0], &deque->buffer[h], r * sizeof(zval));
        }

        deque->head = 0;
        deque->tail = size;
    }
}

static void deque_reallocate(Deque *deque, zend_long size, zend_long capacity)
{
    if (deque->head > deque->tail) {
        zval *buffer = create_buffer(deque, capacity);
        FREE_AND_REPLACE(deque->buffer, buffer);

    } else {
        if (deque->head != 0) {
            memmove(deque->buffer, deque->buffer + deque->head, size * sizeof(zval));
        }

        REALLOC_ZVAL_BUFFER(deque->buffer, capacity);
    }

    deque->capacity = capacity;
    deque->head = 0;
    deque->tail = size;
}

static inline void deque_double_capacity(Deque *deque)
{
    // Can't use DEQUE_SIZE here because it'll report 0 if head == tail.
    deque_reallocate(deque, deque->capacity, deque->capacity << 1);
}

static inline void deque_ensure_capacity(Deque *deque, zend_long size)
{
    if (size >= deque->capacity) {
        deque_reallocate(deque, DEQUE_SIZE(deque), capacity_for_size(size));
    }
}

void deque_user_allocate(Deque *deque, zend_long size)
{
    // -1 because an extra slot will be allocated for the tail.
    deque_ensure_capacity(deque, size - 1);
}

static inline void auto_truncate(Deque *deque)
{
    const zend_long size = DEQUE_SIZE(deque);

    // Automatically truncate if the size of the deque is less than a quarter.
    if (size < deque->capacity >> 2) {

        // Truncate to half the capacity, but only if greater than the minimum.
        if ((deque->capacity >> 1) > DEQUE_MIN_CAPACITY) {
            deque_reallocate(deque, size, deque->capacity >> 1);
        }
    }
}

void deque_clear(Deque *deque)
{
    zval *value;

    if (DEQUE_IS_EMPTY(deque)) {
        return;
    }

    DEQUE_FOREACH(deque, value) {
        zval_ptr_dtor(value);
    }
    DEQUE_FOREACH_END();

    REALLOC_ZVAL_BUFFER(deque->buffer, DEQUE_MIN_CAPACITY);
    deque->capacity = DEQUE_MIN_CAPACITY;
    deque->head     = 0;
    deque->tail     = 0;
}

void deque_destroy(Deque *deque)
{
    deque_clear(deque);
    efree(deque->buffer);
}

static inline zval *deque_ptr(Deque *deque, zend_long index)
{
    return deque->buffer + ((deque->head + index) & (deque->capacity - 1));
}

zval *deque_get(Deque *deque, zend_long index)
{
    if (index_out_of_range(index, DEQUE_SIZE(deque))) {
        return NULL;
    }

    return deque_ptr(deque, index);
}

void deque_set(Deque *deque, zend_long index, zval *value)
{
    if ( ! index_out_of_range(index, DEQUE_SIZE(deque))) {
        ZVAL_DTOR_COPY(deque_ptr(deque, index), value);
    }
}

void deque_reverse(Deque *deque)
{
    if (deque->head < deque->tail) {
        reverse_zval_range(
            deque->buffer + deque->head,
            deque->buffer + deque->tail
        );

    } else {
        zval *data = deque->buffer;

        zend_long mask = deque->capacity - 1;
        zend_long head = deque->head;
        zend_long tail = deque->tail & mask;

        while (head < tail) {
            tail = (tail - 1) & mask;
            SWAP_ZVAL(data[head], data[tail]);
            head = (head + 1) & mask;
        }
    }
}

Deque *deque_reversed(Deque *deque)
{
    zval *buffer = ALLOC_ZVAL_BUFFER(deque->capacity);

    if (deque->head < deque->tail) {
        zval *src = deque->buffer + deque->tail;
        zval *end = deque->buffer + deque->head;
        zval *dst = buffer;

        for (; src != end; ++dst) {
            ZVAL_COPY(dst, --src);
        }
    } else {
        zval *src;
        zval *dst = buffer;

        DEQUE_FOREACH_REVERSED(deque, src) {
            ZVAL_COPY(dst, src);
            dst++;
        }
        DEQUE_FOREACH_END();
    }

    return deque_from_buffer_ex(buffer, DEQUE_SIZE(deque), deque->capacity);
}

static inline void do_shift(Deque *deque, zval *return_value)
{
    ZVAL_COPY_DTOR(return_value, &deque->buffer[deque->head]);
    increment_head(deque);
    auto_truncate(deque);
}

static inline void do_pop(Deque *deque, zval *return_value)
{
    decrement_tail(deque);
    ZVAL_COPY_DTOR(return_value, &deque->buffer[deque->tail]);
    auto_truncate(deque);
}

void deque_shift(Deque *deque, zval *return_value)
{
    if (DEQUE_IS_EMPTY(deque)) {
        NOT_ALLOWED_WHEN_EMPTY();
        return;
    }

    do_shift(deque, return_value);
}

void deque_pop(Deque *deque, zval *return_value)
{
    if (DEQUE_IS_EMPTY(deque)) {
        NOT_ALLOWED_WHEN_EMPTY();
        return;
    }

    do_pop(deque, return_value);
}

void deque_remove(Deque *deque, zend_long index, zval *return_value)
{
    if (index_out_of_range(index, DEQUE_SIZE(deque))) {
        return;
    }

    // Basic shift if it's the first element in the sequence.
    if (index == 0) {
        do_shift(deque, return_value);

    // Basic pop if it's the last element in the sequence.
    } else if (index == DEQUE_SIZE(deque) - 1) {
        do_pop(deque, return_value);

    } else {
        zend_long h = deque->head;
        zend_long t = deque->tail;
        zend_long i = (h + index) & (deque->capacity - 1); // Buffer index

        zval *data = deque->buffer;
        ZVAL_COPY_DTOR(return_value, &data[i]);

        if (i < t) {
            // Index comes before the tail, so it must be between 0 and t,
            // otherwise it would have wrapped around.

            // Shift all values between the index and the tail.
            memmove(&data[i], &data[i + 1], sizeof(zval) * (t - i));
            deque->tail--;

        } else {
            // Index comes after tail, and we know at this point that the index
            // is valid, so it ,ust be after the head which has wrapped around.

            // Unshift all values between the head and the index.
            memmove(&data[h + 1], &data[h], sizeof(zval) * (i - h));
            deque->head++;
        }

        auto_truncate(deque);
    }
}

void deque_unshift_va(Deque *deque, VA_PARAMS)
{
    deque_ensure_capacity(deque, DEQUE_SIZE(deque) + argc);

    while (argc--) {
        decrement_head(deque);
        ZVAL_COPY(&deque->buffer[deque->head], &argv[argc]);
    }
}

void deque_push(Deque *deque, zval *value)
{
    ZVAL_COPY(&deque->buffer[deque->tail], value);
    increment_tail(deque);

    if (deque->head == deque->tail) {
        deque_double_capacity(deque);
    }
}

void deque_push_va(Deque *deque, VA_PARAMS)
{
    deque_ensure_capacity(deque, DEQUE_SIZE(deque) + argc);

    while (argc--) {
        ZVAL_COPY(&deque->buffer[deque->tail], argv++);
        increment_tail(deque);
    }
}

void deque_insert_va(Deque *deque, zend_long index, VA_PARAMS)
{
    const zend_long size = DEQUE_SIZE(deque);

    if (index_out_of_range(index, size + 1) || argc <= 0) {
        return;
    }

    // Basic unshift if inserting at the front.
    if (index == 0) {
        deque_unshift_va(deque, argc, argv);

    // Basic push if inserting at the back.
    } else if (index == size) {
        deque_push_va(deque, argc, argv);

    } else {
        zval *pos;

        // Make sure that we have enough room for the new values.
        deque_ensure_capacity(deque, size + argc);

        // Translate the positional index to a buffer index
        index = (deque->head + index) & (deque->capacity - 1);

        // Check if the buffer index is 0:
        // We can insert at either 0 or the last index of the buffer, as both
        // would produce the same sequence. We can minimize the amount of
        // movement required by using the smaller partition.
        if (index == 0 && deque->tail < (size >> 1)) {
            index = deque->capacity;
        }

        if (index < deque->tail) {
            // Doesn't matter if there are two partitions because we know we
            // can move in front of the tail, leaving room for the new values.
            pos = deque->buffer + index;
            memmove(pos + argc, pos, (deque->tail - index) * sizeof(zval));
            deque->tail += argc;

        } else {
            zval *head = &deque->buffer[deque->head];

            // Values have wrapped around and we know we're between the head
            // and the end of the buffer. Move wrapped partition to the left.
            pos = &deque->buffer[index - argc];
            memmove(head - argc, head, (index - deque->head) * sizeof(zval));
            deque->head -= argc;
        }

        for (; argc--; ++pos, ++argv) {
            ZVAL_COPY(pos, argv);
        }
    }
}

static inline zend_long deque_find_index(Deque *deque, zval *value)
{
    zend_long tail = deque->tail;
    zend_long head = deque->head;
    zend_long mask = deque->capacity - 1;

    zend_long index = 0;

    // Iterate through each item in the list, checking equality as well.
    while (head != tail) {
        if (zend_is_identical(value, &deque->buffer[head])) {
            return index;
        }

        head = (head + 1) & mask;
        index++;
    }

    return FAILURE;
}

void deque_join(Deque *deque, char *str, size_t len, zval *return_value)
{
    zend_string *s;
    deque_pack(deque);
    s = join_zval_buffer(deque->buffer, DEQUE_SIZE(deque), str, len);
    ZVAL_STR(return_value, s);
}

void deque_find(Deque *deque, zval *value, zval *return_value)
{
    zend_long index = deque_find_index(deque, value);

    if (index >= 0) {
        ZVAL_LONG(return_value, index);
        return;
    }

    ZVAL_FALSE(return_value);
}

bool deque_contains_va(Deque *deque, VA_PARAMS)
{
    if (argc == 0) {
        return 0;
    }

    while (argc--) {
        if (deque_find_index(deque, argv++) == FAILURE) {
            return 0;
        }
    }

    return 1;
}

void deque_rotate(Deque *deque, zend_long n)
{
    zval *buffer = deque->buffer;

    const zend_long mask = deque->capacity - 1;
    const zend_long size = DEQUE_SIZE(deque);

    if (n < 0) {
        for (n = llabs(n) % size; n > 0; n--) {

            // Pop, unshift
            decrement_head_ex(deque, mask);
            decrement_tail_ex(deque, mask);

            // Tail is now at last value, head is before the first.
            SWAP_ZVAL(buffer[deque->tail], buffer[deque->head]);
        }
    } else if (n > 0) {
        for (n = n % size; n > 0; n--) {

            // Tail is one past the last value, head is at first value.
            SWAP_ZVAL(buffer[deque->tail], buffer[deque->head]);

            // Shift, push
            increment_head_ex(deque, mask);
            increment_tail_ex(deque, mask);
        }
    }
}

void deque_to_array(Deque *deque, zval *array)
{


    if (DEQUE_IS_EMPTY(deque)) {
        array_init(array);
        return;

    } else {
        zval *value;
        array_init_size(array, DEQUE_SIZE(deque));

        DEQUE_FOREACH(deque, value) {
            add_next_index_zval(array, value);
            Z_TRY_ADDREF_P(value);
        }
        DEQUE_FOREACH_END();
    }
}

int deque_index_exists(Deque *deque, zend_long index)
{
    return index >= 0 && index < DEQUE_SIZE(deque);
}

bool deque_isset(Deque *deque, zend_long index, int check_empty)
{
    if (index < 0 || index >= DEQUE_SIZE(deque)) {
        return false;
    }

    return zval_isset(deque_ptr(deque, index), check_empty);
}

zval *deque_get_last(Deque *deque)
{
    if (DEQUE_IS_EMPTY(deque)) {
        NOT_ALLOWED_WHEN_EMPTY();
        return NULL;
    }

    return deque->buffer + ((deque->tail - 1) & (deque->capacity - 1));
}

zval *deque_get_first(Deque *deque)
{
    if (DEQUE_IS_EMPTY(deque)) {
        NOT_ALLOWED_WHEN_EMPTY();
        return NULL;
    }

    return deque->buffer + deque->head;
}

static int iterator_add(zend_object_iterator *iterator, void *puser)
{
    Deque *deque = (Deque *) puser;
    zval *value = iterator->funcs->get_current_data(iterator);
    deque_push(deque, value);

    return ZEND_HASH_APPLY_KEEP;
}

static void add_traversable_to_deque(Deque *deque, zval *obj)
{
    spl_iterator_apply(obj, iterator_add, deque);
}

static void add_array_to_deque(Deque *deque, HashTable *arr)
{
    zval *value;
    ZEND_HASH_FOREACH_VAL(arr, value) {
        deque_push(deque, value);
    }
    ZEND_HASH_FOREACH_END();
}

void deque_push_all(Deque *deque, zval *values)
{
    if ( ! values) {
        return;
    }

    if (is_array(values)) {
        add_array_to_deque(deque, Z_ARRVAL_P(values));
        return;
    }

    if (is_traversable(values)) {
        add_traversable_to_deque(deque, values);
        return;
    }

    ARRAY_OR_TRAVERSABLE_REQUIRED();
}

void deque_merge(Deque *deque, zval *values, zval *obj)
{
    if ( ! values) {
        return;
    }

    if ( ! is_array(values) && ! is_traversable(values)) {
        ARRAY_OR_TRAVERSABLE_REQUIRED();
        return;

    } else {
        Deque *merged = deque_create_copy(deque);
        deque_push_all(merged, values);
        ZVAL_DEQUE(obj, merged);
    }
}

void deque_sort_callback(Deque *deque)
{
    deque_pack(deque);
    user_sort_zval_buffer(deque->buffer, DEQUE_SIZE(deque));
}

void deque_sort(Deque *deque)
{
    deque_pack(deque);
    sort_zval_buffer(deque->buffer, DEQUE_SIZE(deque));
}

void deque_map(Deque *deque, zval *obj, FCI_PARAMS)
{
    zval *src;
    zval *buf = ALLOC_ZVAL_BUFFER(deque->capacity);
    zval *dst = buf;

    DEQUE_FOREACH(deque, src) {

        zval param;
        zval retval;

        ZVAL_COPY_VALUE(&param, src);

        fci.param_count = 1;
        fci.params      = &param;
        fci.retval      = &retval;

        if (zend_call_function(&fci, &fci_cache) == FAILURE || Z_ISUNDEF(retval)) {
            ZVAL_NULL(obj);
            return;
        } else {
            ZVAL_COPY_VALUE(dst, &retval);
        }

        dst++;
    }
    DEQUE_FOREACH_END();

    ZVAL_DEQUE(obj,
        deque_from_buffer_ex(buf, DEQUE_SIZE(deque), deque->capacity));
}

void deque_filter_callback(Deque *deque, zval *obj, FCI_PARAMS)
{
    zval param;
    zval retval;

    zval *buf, *src, *dst;

    if (DEQUE_IS_EMPTY(deque)) {
        ZVAL_NEW_DEQUE(obj);
        return;
    }

    buf = ALLOC_ZVAL_BUFFER(deque->capacity);
    dst = buf;

    DEQUE_FOREACH(deque, src) {
        ZVAL_COPY_VALUE(&param, src);

        fci.param_count = 1;
        fci.params      = &param;
        fci.retval      = &retval;

        if (zend_call_function(&fci, &fci_cache) == FAILURE || Z_ISUNDEF(retval)) {
            ZVAL_NULL(obj);
            return;
        } else if (zend_is_true(&retval)) {
            ZVAL_COPY(dst++, src);
        }
    }
    DEQUE_FOREACH_END();
    ZVAL_DEQUE(obj, deque_from_buffer(buf, dst - buf));
}

void deque_filter(Deque *deque, zval *obj)
{
    if (DEQUE_IS_EMPTY(deque)) {
        ZVAL_NEW_DEQUE(obj);
        return;

    } else {
        zval *buf = ALLOC_ZVAL_BUFFER(deque->capacity);
        zval *dst = buf;
        zval *src = NULL;

        DEQUE_FOREACH(deque, src) {
            if (zend_is_true(src)) {
                ZVAL_COPY(dst++, src);
            }
        }
        DEQUE_FOREACH_END();
        ZVAL_DEQUE(obj, deque_from_buffer(buf, dst - buf));
    }
}

void deque_reduce(Deque *deque, zval *initial, zval *return_value, FCI_PARAMS)
{
    zval carry;
    zval *value;

    if (initial == NULL) {
        ZVAL_NULL(&carry);
    } else {
        ZVAL_COPY_VALUE(&carry, initial);
    }

    DEQUE_FOREACH(deque, value) {
        zval params[2];
        zval retval;

        ZVAL_COPY_VALUE(&params[0], &carry);
        ZVAL_COPY_VALUE(&params[1], value);

        fci.param_count = 2;
        fci.params      = params;
        fci.retval      = &retval;

        if (zend_call_function(&fci, &fci_cache) == FAILURE) {
            ZVAL_NULL(return_value);
            return;
        } else {
            ZVAL_COPY_VALUE(&carry, &retval);
        }
    }
    DEQUE_FOREACH_END();
    ZVAL_COPY(return_value, &carry);
}

static Deque *deque_do_slice(Deque *deque, zend_long index, zend_long length)
{
    zend_long capacity = capacity_for_size(length);

    zval *buffer = ALLOC_ZVAL_BUFFER(capacity);

    zval *src = deque->buffer;   // Read from the source deque's buffer
    zval *dst = buffer;          // Pointer to allocated buffer, for writing
    zval *end = buffer + length; // Position at which to stop writing.

    // If the head comes before the tail, we don't need to mod because we
    // know that no values have wrapped around the end of the buffer.
    if (deque->head < deque->tail) {
        src += deque->head + index;

        for (; dst != end; ++src, ++dst) {
            ZVAL_COPY(dst, src);
        }
    } else {
        zend_long mask = capacity - 1;
        zend_long head = deque->head + index;

        for (; dst != end; ++head, ++dst) {
            ZVAL_COPY(dst, &src[head & mask]);
        }
    }

    return deque_from_buffer_ex(buffer, length, capacity);
}

void deque_slice(Deque *deque, zend_long index, zend_long length, zval *obj)
{
    normalize_slice_params(&index, &length, DEQUE_SIZE(deque));

    if (length == 0) {
        ZVAL_NEW_DEQUE(obj);
        return;
    }

    ZVAL_DEQUE(obj, deque_do_slice(deque, index, length));
}

int deque_serialize(zval *object, unsigned char **buffer, size_t *length, zend_serialize_data *data)
{
    Deque *deque = Z_DEQUE_P(object);

    php_serialize_data_t serialize_data = (php_serialize_data_t) data;
    PHP_VAR_SERIALIZE_INIT(serialize_data);

    if (DEQUE_IS_EMPTY(deque)) {
        SERIALIZE_SET_ZSTR(ZSTR_EMPTY_ALLOC());

    } else {

        zval *value;
        smart_str buf = {0};

        DEQUE_FOREACH(deque, value) {
            php_var_serialize(&buf, value, &serialize_data);
        }
        DEQUE_FOREACH_END();

        smart_str_0(&buf);
        SERIALIZE_SET_ZSTR(buf.s);
        zend_string_release(buf.s);
    }

    PHP_VAR_SERIALIZE_DESTROY(serialize_data);
    return SUCCESS;
}

int deque_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t length, zend_unserialize_data *data)
{
    Deque *deque = deque_init();

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

        deque_push(deque, value);
    }

    if (*(++pos) != '\0') {
        goto error;
    }

    ZVAL_DEQUE(object, deque);
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    return SUCCESS;

error:
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    UNSERIALIZE_ERROR();
    return FAILURE;
}
