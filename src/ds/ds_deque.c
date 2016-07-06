#include "../common.h"

#include "../php/iterators/php_deque_iterator.h"
#include "../php/handlers/php_deque_handlers.h"
#include "../php/classes/php_deque_ce.h"

#include "ds_deque.h"

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

    return MAX(DS_DEQUE_MIN_CAPACITY, n);
}

ds_deque_t *ds_deque_ex(zend_long capacity)
{
    ds_deque_t *deque = ecalloc(1, sizeof(ds_deque_t));

    if (capacity < DS_DEQUE_MIN_CAPACITY) {
        capacity = DS_DEQUE_MIN_CAPACITY;
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

ds_deque_t *ds_deque()
{
    return ds_deque_ex(DS_DEQUE_MIN_CAPACITY);
}

static inline bool index_out_of_range(zend_long index, zend_long max)
{
    if (index < 0 || index >= max) {
        INDEX_OUT_OF_RANGE(index, max);
        return true;
    }

    return false;
}

static ds_deque_t *ds_deque_from_buffer_ex(
    zval *buffer,
    zend_long size,
    zend_long capacity
) {
    ds_deque_t *deque = ecalloc(1, sizeof(ds_deque_t));

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

ds_deque_t *ds_deque_from_buffer(zval *buffer, zend_long size)
{
    return ds_deque_from_buffer_ex(buffer, size, capacity_for_size(size));
}

static inline void ds_deque_copy(ds_deque_t *src, ds_deque_t *dst)
{
    zend_long tail = src->tail;
    zend_long head = src->head;
    zend_long mask = src->capacity - 1;

    for (; head != tail; head = (head + 1) & mask) {
        ZVAL_COPY(&dst->buffer[head], &src->buffer[head]);
    }
}

ds_deque_t *ds_deque_create_copy(ds_deque_t *deque)
{
    ds_deque_t *cloned = ecalloc(1, sizeof(ds_deque_t));

    cloned->buffer   = ALLOC_ZVAL_BUFFER(deque->capacity);
    cloned->capacity = deque->capacity;
    cloned->head     = deque->head;
    cloned->tail     = deque->tail;

    ds_deque_copy(deque, cloned);
    return cloned;
}

static zval *create_buffer(ds_deque_t *deque, zend_long capacity)
{
    zval *src = deque->buffer;
    zval *dst = ALLOC_ZVAL_BUFFER(capacity);

    zend_long h = deque->head;
    zend_long t = deque->tail;

    // If the head comes before the tail, we know that the head hasn't
    // wrapped around. [_, h, ..., t, _]
    if (h < t) {
        memcpy(&dst[0], &src[h], DS_DEQUE_SIZE(deque) * sizeof(zval));

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
static void ds_deque_pack(ds_deque_t *deque)
{
    if (deque->head > 0) {
        zend_long size = DS_DEQUE_SIZE(deque);

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

static void ds_deque_reallocate(ds_deque_t *deque, zend_long size, zend_long capacity)
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

static inline void ds_deque_double_capacity(ds_deque_t *deque)
{
    // Can't use DS_DEQUE_SIZE here because it'll report 0 if head == tail.
    ds_deque_reallocate(deque, deque->capacity, deque->capacity << 1);
}

static inline void ds_deque_ensure_capacity(ds_deque_t *deque, zend_long size)
{
    if (size >= deque->capacity) {
        ds_deque_reallocate(deque, DS_DEQUE_SIZE(deque), capacity_for_size(size));
    }
}

void ds_deque_user_allocate(ds_deque_t *deque, zend_long size)
{
    // -1 because an extra slot will be allocated for the tail.
    ds_deque_ensure_capacity(deque, size - 1);
}

static inline void auto_truncate(ds_deque_t *deque)
{
    const zend_long size = DS_DEQUE_SIZE(deque);

    // Automatically truncate if the size of the deque is less than a quarter.
    if (size < deque->capacity >> 2) {

        // Truncate to half the capacity, but only if greater than the minimum.
        if ((deque->capacity >> 1) > DS_DEQUE_MIN_CAPACITY) {
            ds_deque_reallocate(deque, size, deque->capacity >> 1);
        }
    }
}

void ds_deque_clear(ds_deque_t *deque)
{
    zval *value;

    if (DS_DEQUE_IS_EMPTY(deque)) {
        return;
    }

    DS_DEQUE_FOREACH(deque, value) {
        zval_ptr_dtor(value);
    }
    DS_DEQUE_FOREACH_END();

    REALLOC_ZVAL_BUFFER(deque->buffer, DS_DEQUE_MIN_CAPACITY);
    deque->capacity = DS_DEQUE_MIN_CAPACITY;
    deque->head     = 0;
    deque->tail     = 0;
}

void ds_deque_destroy(ds_deque_t *deque)
{
    ds_deque_clear(deque);
    efree(deque->buffer);
}

/**
 *
 */
static inline zval *ds_deque_lookup_index(ds_deque_t *deque, zend_long index)
{
    return deque->buffer + ((deque->head + index) & (deque->capacity - 1));
}

zval *ds_deque_get(ds_deque_t *deque, zend_long index)
{
    if (index_out_of_range(index, DS_DEQUE_SIZE(deque))) {
        return NULL;
    }

    return ds_deque_lookup_index(deque, index);
}

void ds_deque_set(ds_deque_t *deque, zend_long index, zval *value)
{
    if ( ! index_out_of_range(index, DS_DEQUE_SIZE(deque))) {
        ZVAL_DTOR_COPY(ds_deque_lookup_index(deque, index), value);
    }
}

void ds_deque_reverse(ds_deque_t *deque)
{
    if (deque->head < deque->tail) {
        ds_reverse_zval_range(
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

ds_deque_t *ds_deque_reversed(ds_deque_t *deque)
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

        DS_DEQUE_FOREACH_REVERSED(deque, src) {
            ZVAL_COPY(dst, src);
            dst++;
        }
        DS_DEQUE_FOREACH_END();
    }

    return ds_deque_from_buffer_ex(buffer, DS_DEQUE_SIZE(deque), deque->capacity);
}

static inline void do_shift(ds_deque_t *deque, zval *return_value)
{
    ZVAL_COPY_DTOR(return_value, &deque->buffer[deque->head]);
    increment_head(deque);
    auto_truncate(deque);
}

static inline void do_pop(ds_deque_t *deque, zval *return_value)
{
    decrement_tail(deque);
    ZVAL_COPY_DTOR(return_value, &deque->buffer[deque->tail]);
    auto_truncate(deque);
}

void ds_deque_shift(ds_deque_t *deque, zval *return_value)
{
    if (DS_DEQUE_IS_EMPTY(deque)) {
        NOT_ALLOWED_WHEN_EMPTY();
        return;
    }

    do_shift(deque, return_value);
}

void ds_deque_pop(ds_deque_t *deque, zval *return_value)
{
    if (DS_DEQUE_IS_EMPTY(deque)) {
        NOT_ALLOWED_WHEN_EMPTY();
        return;
    }

    do_pop(deque, return_value);
}

void ds_deque_remove(ds_deque_t *deque, zend_long index, zval *return_value)
{
    if (index_out_of_range(index, DS_DEQUE_SIZE(deque))) {
        return;
    }

    // Basic shift if it's the first element in the sequence.
    if (index == 0) {
        do_shift(deque, return_value);

    // Basic pop if it's the last element in the sequence.
    } else if (index == DS_DEQUE_SIZE(deque) - 1) {
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

void ds_deque_unshift_va(ds_deque_t *deque, VA_PARAMS)
{
    ds_deque_ensure_capacity(deque, DS_DEQUE_SIZE(deque) + argc);

    while (argc--) {
        decrement_head(deque);
        ZVAL_COPY(&deque->buffer[deque->head], &argv[argc]);
    }
}

void ds_deque_push(ds_deque_t *deque, zval *value)
{
    ZVAL_COPY(&deque->buffer[deque->tail], value);
    increment_tail(deque);

    if (deque->head == deque->tail) {
        ds_deque_double_capacity(deque);
    }
}

void ds_deque_push_va(ds_deque_t *deque, VA_PARAMS)
{
    ds_deque_ensure_capacity(deque, DS_DEQUE_SIZE(deque) + argc);

    while (argc--) {
        ZVAL_COPY(&deque->buffer[deque->tail], argv++);
        increment_tail(deque);
    }
}

void ds_deque_insert_va(ds_deque_t *deque, zend_long index, VA_PARAMS)
{
    const zend_long size = DS_DEQUE_SIZE(deque);

    if (index_out_of_range(index, size + 1) || argc <= 0) {
        return;
    }

    // Basic unshift if inserting at the front.
    if (index == 0) {
        ds_deque_unshift_va(deque, argc, argv);

    // Basic push if inserting at the back.
    } else if (index == size) {
        ds_deque_push_va(deque, argc, argv);

    } else {
        zval *pos;

        // Make sure that we have enough room for the new values.
        ds_deque_ensure_capacity(deque, size + argc);

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

static inline zend_long ds_deque_find_index(ds_deque_t *deque, zval *value)
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

void ds_deque_join(ds_deque_t *deque, char *str, size_t len, zval *return_value)
{
    zend_string *s;
    ds_deque_pack(deque);
    s = ds_join_zval_buffer(deque->buffer, DS_DEQUE_SIZE(deque), str, len);
    ZVAL_STR(return_value, s);
}

void ds_deque_find(ds_deque_t *deque, zval *value, zval *return_value)
{
    zend_long index = ds_deque_find_index(deque, value);

    if (index >= 0) {
        ZVAL_LONG(return_value, index);
        return;
    }

    ZVAL_FALSE(return_value);
}

bool ds_deque_contains_va(ds_deque_t *deque, VA_PARAMS)
{
    if (argc == 0) {
        return 0;
    }

    while (argc--) {
        if (ds_deque_find_index(deque, argv++) == FAILURE) {
            return 0;
        }
    }

    return 1;
}

void ds_deque_rotate(ds_deque_t *deque, zend_long n)
{
    zval *buffer = deque->buffer;

    const zend_long mask = deque->capacity - 1;
    const zend_long size = DS_DEQUE_SIZE(deque);

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

void ds_deque_to_array(ds_deque_t *deque, zval *array)
{


    if (DS_DEQUE_IS_EMPTY(deque)) {
        array_init(array);
        return;

    } else {
        zval *value;
        array_init_size(array, DS_DEQUE_SIZE(deque));

        DS_DEQUE_FOREACH(deque, value) {
            add_next_index_zval(array, value);
            Z_TRY_ADDREF_P(value);
        }
        DS_DEQUE_FOREACH_END();
    }
}

int ds_deque_index_exists(ds_deque_t *deque, zend_long index)
{
    return index >= 0 && index < DS_DEQUE_SIZE(deque);
}

bool ds_deque_isset(ds_deque_t *deque, zend_long index, int check_empty)
{
    if (index < 0 || index >= DS_DEQUE_SIZE(deque)) {
        return false;
    }

    return ds_zval_isset(ds_deque_lookup_index(deque, index), check_empty);
}

zval *ds_deque_get_last(ds_deque_t *deque)
{
    if (DS_DEQUE_IS_EMPTY(deque)) {
        NOT_ALLOWED_WHEN_EMPTY();
        return NULL;
    }

    return deque->buffer + ((deque->tail - 1) & (deque->capacity - 1));
}

zval *ds_deque_get_first(ds_deque_t *deque)
{
    if (DS_DEQUE_IS_EMPTY(deque)) {
        NOT_ALLOWED_WHEN_EMPTY();
        return NULL;
    }

    return deque->buffer + deque->head;
}

static int iterator_add(zend_object_iterator *iterator, void *puser)
{
    ds_deque_t *deque = (ds_deque_t *) puser;
    zval *value = iterator->funcs->get_current_data(iterator);
    ds_deque_push(deque, value);

    return ZEND_HASH_APPLY_KEEP;
}

static void add_traversable_to_deque(ds_deque_t *deque, zval *obj)
{
    spl_iterator_apply(obj, iterator_add, deque);
}

static void add_array_to_deque(ds_deque_t *deque, HashTable *arr)
{
    zval *value;
    ZEND_HASH_FOREACH_VAL(arr, value) {
        ds_deque_push(deque, value);
    }
    ZEND_HASH_FOREACH_END();
}

void ds_deque_push_all(ds_deque_t *deque, zval *values)
{
    if ( ! values) {
        return;
    }

    if (ds_zval_is_array(values)) {
        add_array_to_deque(deque, Z_ARRVAL_P(values));
        return;
    }

    if (ds_zval_is_traversable(values)) {
        add_traversable_to_deque(deque, values);
        return;
    }

    ARRAY_OR_TRAVERSABLE_REQUIRED();
}

ds_deque_t *ds_deque_merge(ds_deque_t *deque, zval *values)
{
    if (values && (ds_zval_is_array(values) || ds_zval_is_traversable(values))) {
        ds_deque_t *merged = ds_deque_create_copy(deque);
        ds_deque_push_all(merged, values);
        return merged;
    }

    ARRAY_OR_TRAVERSABLE_REQUIRED();
    return NULL;
}

void ds_deque_sort_callback(ds_deque_t *deque)
{
    ds_deque_pack(deque);
    ds_user_sort_zval_buffer(deque->buffer, DS_DEQUE_SIZE(deque));
}

void ds_deque_sort(ds_deque_t *deque)
{
    ds_deque_pack(deque);
    ds_sort_zval_buffer(deque->buffer, DS_DEQUE_SIZE(deque));
}

ds_deque_t *ds_deque_map(ds_deque_t *deque, FCI_PARAMS)
{
    zval *src;
    zval *buf = ALLOC_ZVAL_BUFFER(deque->capacity);
    zval *dst = buf;

    DS_DEQUE_FOREACH(deque, src) {

        zval param;
        zval retval;

        ZVAL_COPY_VALUE(&param, src);

        fci.param_count = 1;
        fci.params      = &param;
        fci.retval      = &retval;

        if (zend_call_function(&fci, &fci_cache) == FAILURE || Z_ISUNDEF(retval)) {
            return NULL;
        } else {
            ZVAL_COPY_VALUE(dst, &retval);
        }

        dst++;
    }
    DS_DEQUE_FOREACH_END();

    return ds_deque_from_buffer_ex(buf, DS_DEQUE_SIZE(deque), deque->capacity);
}

ds_deque_t *ds_deque_filter_callback(ds_deque_t *deque, FCI_PARAMS)
{
    if (DS_DEQUE_IS_EMPTY(deque)) {
        return ds_deque();

    } else {
        zval param;
        zval retval;

        zval *src;
        zval *buf = ALLOC_ZVAL_BUFFER(deque->capacity);
        zval *dst = buf;

        DS_DEQUE_FOREACH(deque, src) {
            ZVAL_COPY_VALUE(&param, src);

            fci.param_count = 1;
            fci.params      = &param;
            fci.retval      = &retval;

            // Catch potential exceptions or other errors during comparison.
            if (zend_call_function(&fci, &fci_cache) == FAILURE || Z_ISUNDEF(retval)) {
                efree(buf);
                return NULL;
            }

            if (zend_is_true(&retval)) {
                ZVAL_COPY(dst++, src);
            }
        }

        DS_DEQUE_FOREACH_END();
        return ds_deque_from_buffer(buf, dst - buf);
    }
}

ds_deque_t *ds_deque_filter(ds_deque_t *deque)
{
    if (DS_DEQUE_IS_EMPTY(deque)) {
        return ds_deque();

    } else {
        zval *buf = ALLOC_ZVAL_BUFFER(deque->capacity);
        zval *dst = buf;
        zval *src = NULL;

        DS_DEQUE_FOREACH(deque, src) {
            if (zend_is_true(src)) {
                ZVAL_COPY(dst++, src);
            }
        }
        DS_DEQUE_FOREACH_END();

        return ds_deque_from_buffer(buf, dst - buf);
    }
}

void ds_deque_reduce(ds_deque_t *deque, zval *initial, zval *return_value, FCI_PARAMS)
{
    zval carry;
    zval *value;

    if (initial == NULL) {
        ZVAL_NULL(&carry);
    } else {
        ZVAL_COPY_VALUE(&carry, initial);
    }

    DS_DEQUE_FOREACH(deque, value) {
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
    DS_DEQUE_FOREACH_END();
    ZVAL_COPY(return_value, &carry);
}

static ds_deque_t *ds_deque_do_slice(ds_deque_t *deque, zend_long index, zend_long length)
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

    return ds_deque_from_buffer_ex(buffer, length, capacity);
}

ds_deque_t *ds_deque_slice(ds_deque_t *deque, zend_long index, zend_long length)
{
    ds_normalize_slice_args(&index, &length, DS_DEQUE_SIZE(deque));

    if (length == 0) {
        return ds_deque();
    }

    return ds_deque_do_slice(deque, index, length);
}
