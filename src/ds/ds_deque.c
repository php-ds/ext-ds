#include "../common.h"

#include "../php/iterators/php_deque_iterator.h"
#include "../php/handlers/php_deque_handlers.h"
#include "../php/classes/php_deque_ce.h"

#include "ds_deque.h"

#define ds_deque_increment_head(_d) (_d)->head = ((_d)->head + 1) & ((_d)->capacity - 1)
#define ds_deque_decrement_head(_d) (_d)->head = ((_d)->head - 1) & ((_d)->capacity - 1)
#define ds_deque_increment_tail(_d) (_d)->tail = ((_d)->tail + 1) & ((_d)->capacity - 1)
#define ds_deque_decrement_tail(_d) (_d)->tail = ((_d)->tail - 1) & ((_d)->capacity - 1)

static inline void _memmove(ds_deque_t *deque, zend_long dst, zend_long src, zend_long length)
{
    memmove(&deque->buffer[dst], &deque->buffer[src], length * sizeof(zval));
}

static zend_long ds_deque_required_capacity(zend_long n)
{
    // We have to allocate an extra element for the tail, so a requested power
    // of two would need to round to the next.
    // n--;
    n |= n >>  1;
    n |= n >>  2;
    n |= n >>  4;
    n |= n >>  8;
    n |= n >> 16;

    return MAX(DS_DEQUE_MIN_CAPACITY, n + 1);
}

ds_deque_t *ds_deque()
{
    ds_deque_t *deque = ecalloc(1, sizeof(ds_deque_t));

    deque->buffer   = ALLOC_ZVAL_BUFFER(DS_DEQUE_MIN_CAPACITY);
    deque->capacity = DS_DEQUE_MIN_CAPACITY;
    deque->head     = 0;
    deque->tail     = 0;
    deque->size     = 0;

    return deque;
}

static void ds_deque_copy(ds_deque_t *src, ds_deque_t *dst)
{
    zend_long tail = src->tail;
    zend_long head = src->head;
    zend_long mask = src->capacity - 1;

    for (; head != tail; head = (head + 1) & mask) {
        ZVAL_COPY(&dst->buffer[head], &src->buffer[head]);
    }
}

ds_deque_t *ds_deque_clone(ds_deque_t *deque)
{
    ds_deque_t *cloned = ecalloc(1, sizeof(ds_deque_t));

    cloned->buffer   = ALLOC_ZVAL_BUFFER(deque->capacity);
    cloned->capacity = deque->capacity;
    cloned->head     = deque->head;
    cloned->tail     = deque->tail;
    cloned->size     = deque->size;

    ds_deque_copy(deque, cloned);

    return cloned;
}

static ds_deque_t *ds_deque_from_buffer_ex(
    zval *buffer,
    zend_long size,
    zend_long capacity
) {
    ds_deque_t *deque = ecalloc(1, sizeof(ds_deque_t));

    deque->buffer   = buffer;
    deque->capacity = capacity;
    deque->head     = 0;
    deque->tail     = size;
    deque->size     = size;

    return deque;
}

ds_deque_t *ds_deque_from_buffer(zval *buffer, zend_long size)
{
    zend_long capacity = ds_deque_required_capacity(size);
    return ds_deque_from_buffer_ex(buffer, size, capacity);
}

static inline bool ds_deque_index_out_of_range(ds_deque_t *deque, zend_long index)
{
    if (index < 0 || index >= deque->size) {
        INDEX_OUT_OF_RANGE(index, deque->size);
        return true;
    }

    return false;
}

// Makes sure that the deque's head is at index 0.
static void ds_deque_align_to_zero(ds_deque_t *deque)
{
    if (deque->head == 0) {
        return;
    }

    if (deque->head < deque->tail) {
        _memmove(deque, 0, deque->head, deque->size);

    } else {
        zend_long h = deque->head;
        zend_long t = deque->tail;
        zend_long r = deque->capacity - deque->head;

        if (deque->size < deque->capacity / 2) {
            // There is enough room to move the left forward and insert the
            // right. No need to allocate and replace.
            _memmove(deque, r, 0, t);
            _memmove(deque, 0, h, r);

        } else {
            // We don't have enough temporary space to work with, so create
            // a new buffer, copy to it, then replace the current buffer.
            zval *buffer = ALLOC_ZVAL_BUFFER(deque->capacity);

            memcpy(&buffer[0], &deque->buffer[h], r * sizeof(zval));
            memcpy(&buffer[r], &deque->buffer[0], t * sizeof(zval));

            FREE_AND_REPLACE(deque->buffer, buffer);
        }
    }

    deque->head = 0;
    deque->tail = deque->size;
}

static void ds_deque_reallocate(ds_deque_t *deque, zend_long capacity)
{
    ds_deque_align_to_zero(deque);

    REALLOC_ZVAL_BUFFER(deque->buffer, capacity);

    deque->capacity = capacity;
    deque->head     = 0;
    deque->tail     = deque->size;
}

static inline void ds_deque_double_capacity(ds_deque_t *deque)
{
    ds_deque_reallocate(deque, deque->capacity * 2);
}

static inline void ds_deque_ensure_capacity(ds_deque_t *deque, zend_long size)
{
    if (size >= deque->capacity) {
        ds_deque_reallocate(deque, ds_deque_required_capacity(size));
    }
}

void ds_deque_allocate(ds_deque_t *deque, zend_long capacity)
{
    // -1 because an extra slot will be allocated for the tail.
    ds_deque_ensure_capacity(deque, capacity - 1);
}

static inline void auto_truncate(ds_deque_t *deque)
{
    // Automatically truncate if the size of the deque is less than a quarter.
    if (deque->size < deque->capacity / 4) {

        // Truncate to half the capacity, but only if greater than the minimum.
        if ((deque->capacity / 2) > DS_DEQUE_MIN_CAPACITY) {
            ds_deque_reallocate(deque, deque->capacity / 2);
        }
    }
}

void ds_deque_clear(ds_deque_t *deque)
{
    zval *value;

    if (deque->size == 0) {
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
    deque->size     = 0;
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
    if (ds_deque_index_out_of_range(deque, index)) {
        return NULL;
    }

    return ds_deque_lookup_index(deque, index);
}

void ds_deque_set(ds_deque_t *deque, zend_long index, zval *value)
{
    if ( ! ds_deque_index_out_of_range(deque, index)) {
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

    return ds_deque_from_buffer_ex(buffer, deque->size, deque->capacity);
}

static inline void _ds_deque_shift(ds_deque_t *deque, zval *return_value)
{
    ZVAL_COPY_DTOR(return_value, &deque->buffer[deque->head]);
    ds_deque_increment_head(deque);

    deque->size--;
    auto_truncate(deque);
}

static inline void _ds_deque_pop(ds_deque_t *deque, zval *return_value)
{
    ds_deque_decrement_tail(deque);
    ZVAL_COPY_DTOR(return_value, &deque->buffer[deque->tail]);

    deque->size--;
    auto_truncate(deque);
}

void ds_deque_shift(ds_deque_t *deque, zval *return_value)
{
    if (deque->size == 0) {
        NOT_ALLOWED_WHEN_EMPTY();
        return;
    }

    _ds_deque_shift(deque, return_value);
}

void ds_deque_pop(ds_deque_t *deque, zval *return_value)
{
    if (deque->size == 0) {
        NOT_ALLOWED_WHEN_EMPTY();
        return;
    }

    _ds_deque_pop(deque, return_value);
}

void ds_deque_remove(ds_deque_t *deque, zend_long index, zval *return_value)
{
    if (ds_deque_index_out_of_range(deque, index)) {
        return;
    }

    // Basic shift if it's the first element in the sequence.
    if (index == 0) {
        _ds_deque_shift(deque, return_value);

    // Basic pop if it's the last element in the sequence.
    } else if (index == deque->size - 1) {
        _ds_deque_pop(deque, return_value);

    } else {
        index = (deque->head + index) & (deque->capacity - 1); // Buffer index

        ZVAL_COPY_DTOR(return_value, &deque->buffer[index]);

        if (index < deque->tail) {
            // Index comes before the tail, so it must be between 0 and tail,
            // otherwise it would have wrapped around.

            // Shift all values between the index and the tail.
            _memmove(deque, index, index + 1, deque->tail - index);
            deque->tail--;

        } else {
            // Index comes after tail, and we know at this point that the index
            // is valid, so it ,ust be after the head which has wrapped around.

            // Unshift all values between the head and the index.
            _memmove(deque, deque->head + 1, deque->head, index - deque->head);
            deque->head++;
        }

        deque->size--;
        auto_truncate(deque);
    }
}

void ds_deque_unshift_va(ds_deque_t *deque, VA_PARAMS)
{
    ds_deque_ensure_capacity(deque, deque->size + argc);

    deque->size += argc;

    while (argc--) {
        ds_deque_decrement_head(deque);
        ZVAL_COPY(&deque->buffer[deque->head], &argv[argc]);
    }
}

void ds_deque_push(ds_deque_t *deque, zval *value)
{
    ZVAL_COPY(&deque->buffer[deque->tail], value);
    ds_deque_increment_tail(deque);

    if ((++deque->size) == deque->capacity) {
        ds_deque_double_capacity(deque);
    }
}

void ds_deque_push_va(ds_deque_t *deque, VA_PARAMS)
{
    ds_deque_ensure_capacity(deque, deque->size + argc);

    while (argc--) {
        ZVAL_COPY(&deque->buffer[deque->tail], argv++);
        ds_deque_increment_tail(deque);
        deque->size++;
    }
}

void ds_deque_insert_va(ds_deque_t *deque, zend_long position, VA_PARAMS)
{
    // Basic push if inserting at the back.
    if (position == deque->size) {
        ds_deque_push_va(deque, argc, argv);
    }

    // Check that the position is a valid insertion point.
    else if (ds_deque_index_out_of_range(deque, position)) {
        return;
    }

    // No op if for some reason there are no values.
    else if (argc <= 0) {
        return;
    }

    else {
        zval *dst;
        zend_long index;

        // Make sure that we have enough room for the new values.
        ds_deque_ensure_capacity(deque, deque->size + argc);

        // Convert the positional index to a buffer index.
        index = (deque->head + position) & (deque->capacity - 1);

        if (index < deque->tail) {
            // The deque is either contiguous or we're inserting in between the
            // start of the buffer and the tail and the head has wrapped around.

            // Check for overflow, which is possible because the head won't
            // always be at the start of the buffer.
            if ((deque->tail + argc) > deque->capacity) {

                // There isn't enough free space to the right of the tail,
                // so move the entire sequence all the way to the left.
                _memmove(deque, 0, deque->head, deque->size);

                index -= deque->head;
                deque->head = 0;
                deque->tail = deque->size;
            }

            // Move the subsequence after the insertion point to the right
            // to make room for the new values.
            _memmove(deque, (index + argc), index, (deque->tail - index));
            deque->tail += argc;
            dst = &deque->buffer[index];

        } else {
            // We're inserting between a wrapped around head and the end of the
            // buffer, and it's guaranteed that there's enough free space.
            _memmove(deque, (deque->head - argc), deque->head, (index - deque->head));
            deque->head -= argc;
            dst = &deque->buffer[index - argc];
        }

        deque->size += argc;

        // Copy the new values into place.
        while (argc--) {
            ZVAL_COPY(dst++, argv++);
        }
    }
}

static zend_long ds_deque_find_index(ds_deque_t *deque, zval *value)
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
    ds_deque_align_to_zero(deque);
    s = ds_join_zval_buffer(deque->buffer, deque->size, str, len);
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
    zval *buffer   = deque->buffer;

    if (n < 0) {
        for (n = llabs(n) % deque->size; n > 0; n--) {

            // Pop, unshift
            ds_deque_decrement_head(deque);
            ds_deque_decrement_tail(deque);

            // Tail is now at last value, head is before the first.
            SWAP_ZVAL(buffer[deque->tail], buffer[deque->head]);
        }
    } else if (n > 0) {
        for (n = n % deque->size; n > 0; n--) {

            // Tail is one past the last value, head is at first value.
            SWAP_ZVAL(buffer[deque->tail], buffer[deque->head]);

            // Shift, push
            ds_deque_increment_head(deque);
            ds_deque_increment_tail(deque);
        }
    }
}

void ds_deque_to_array(ds_deque_t *deque, zval *array)
{
    if (deque->size == 0) {
        array_init(array);
        return;

    } else {
        zval *value;
        array_init_size(array, deque->size);

        DS_DEQUE_FOREACH(deque, value) {
            add_next_index_zval(array, value);
            Z_TRY_ADDREF_P(value);
        }
        DS_DEQUE_FOREACH_END();
    }
}

int ds_deque_index_exists(ds_deque_t *deque, zend_long index)
{
    return index >= 0 && index < deque->size;
}

bool ds_deque_isset(ds_deque_t *deque, zend_long index, int check_empty)
{
    if (index < 0 || index >= deque->size) {
        return false;
    }

    return ds_zval_isset(ds_deque_lookup_index(deque, index), check_empty);
}

zval *ds_deque_get_last(ds_deque_t *deque)
{
    if (deque->size == 0) {
        NOT_ALLOWED_WHEN_EMPTY();
        return NULL;
    }

    return deque->buffer + ((deque->tail - 1) & (deque->capacity - 1));
}

zval *ds_deque_get_first(ds_deque_t *deque)
{
    if (deque->size == 0) {
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
        ds_deque_t *merged = ds_deque_clone(deque);
        ds_deque_push_all(merged, values);
        return merged;
    }

    ARRAY_OR_TRAVERSABLE_REQUIRED();
    return NULL;
}

void ds_deque_sort_callback(ds_deque_t *deque)
{
    ds_deque_align_to_zero(deque);
    ds_user_sort_zval_buffer(deque->buffer, deque->size);
}

void ds_deque_sort(ds_deque_t *deque)
{
    ds_deque_align_to_zero(deque);
    ds_sort_zval_buffer(deque->buffer, deque->size);
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

    return ds_deque_from_buffer_ex(buf, deque->size, deque->capacity);
}

ds_deque_t *ds_deque_filter_callback(ds_deque_t *deque, FCI_PARAMS)
{
    if (deque->size == 0) {
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
    if (deque->size == 0) {
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

static ds_deque_t *_ds_deque_slice(ds_deque_t *deque, zend_long index, zend_long length)
{
    zend_long capacity = ds_deque_required_capacity(length);

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
    ds_normalize_slice_args(&index, &length, deque->size);

    if (length == 0) {
        return ds_deque();
    }

    return _ds_deque_slice(deque, index, length);
}
