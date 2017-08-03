#include "../php/iterators/php_ds_deque_iterator.h"
#include "../php/handlers/php_ds_deque_handlers.h"
#include "../php/classes/php_ds_deque_ce.h"
#include "../php/parameters.h"

#include "ds_deque.h"
#include "ds_map.h"

#define ds_deque_increment_head(_d) (_d)->head = ((_d)->head + 1) & ((_d)->capacity - 1)
#define ds_deque_decrement_head(_d) (_d)->head = ((_d)->head - 1) & ((_d)->capacity - 1)
#define ds_deque_increment_tail(_d) (_d)->tail = ((_d)->tail + 1) & ((_d)->capacity - 1)
#define ds_deque_decrement_tail(_d) (_d)->tail = ((_d)->tail - 1) & ((_d)->capacity - 1)

static inline void ds_deque_memmove(
    ds_deque_t *deque,
    zend_long   dst,
    zend_long   src,
    zend_long   length
) {
    memmove(&deque->buffer[dst], &deque->buffer[src], length * sizeof(zval));
}

/**
 * Translates a given number to an acceptable deque capacity >= n.
 */
static zend_long ds_deque_get_capacity_for_size(zend_long n)
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

ds_deque_t *ds_deque_ex(zend_long capacity)
{
    ds_deque_t *deque = ecalloc(1, sizeof(ds_deque_t));

    capacity = MAX(DS_DEQUE_MIN_CAPACITY, capacity);

    deque->buffer   = ALLOC_ZVAL_BUFFER(capacity);
    deque->capacity = capacity;
    deque->head     = 0;
    deque->tail     = 0;
    deque->size     = 0;

    return deque;
}

ds_deque_t *ds_deque()
{
    return ds_deque_ex(DS_DEQUE_MIN_CAPACITY);
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
    zend_long capacity = ds_deque_get_capacity_for_size(size);
    return ds_deque_from_buffer_ex(buffer, size, capacity);
}

ds_deque_t *ds_deque_clone(ds_deque_t *deque)
{
    zval *source;
    zval *buffer = ALLOC_ZVAL_BUFFER(deque->capacity);
    zval *target = buffer;

    DS_DEQUE_FOREACH(deque, source) {
        ZVAL_COPY(target, source);
        target++;
    }
    DS_DEQUE_FOREACH_END();

    return ds_deque_from_buffer_ex(buffer, deque->size, deque->capacity);
}


static inline bool ds_deque_valid_position(ds_deque_t *deque, zend_long index)
{
    if (index < 0 || index >= deque->size) {
        INDEX_OUT_OF_RANGE(index, deque->size);
        return false;
    }

    return true;
}

// Makes sure that the deque's head is at index 0.
void ds_deque_reset_head(ds_deque_t *deque)
{
    if (deque->head == 0) {
        return;
    }

    if (deque->head < deque->tail) {
        ds_deque_memmove(deque, 0, deque->head, deque->size);

    } else {
        zend_long h = deque->head;
        zend_long t = deque->tail;
        zend_long r = deque->capacity - h; // Number of values on the right.

        // Check if there's enough room to push the left partition forward and
        // the wrapped values (right partition) to the front.
        if (r < (h - t)) {
            ds_deque_memmove(deque, r, 0, t);
            ds_deque_memmove(deque, 0, h, r);

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
    ds_deque_reset_head(deque);
    REALLOC_ZVAL_BUFFER(deque->buffer, capacity);

    deque->capacity = capacity;
    deque->head     = 0;
    deque->tail     = deque->size;
}

static inline void ds_deque_double_capacity(ds_deque_t *deque)
{
    ds_deque_reallocate(deque, deque->capacity * 2);
}

static inline void ds_deque_allocate_capacity_for_size(ds_deque_t *deque, zend_long size)
{
    if (size >= deque->capacity) {
        ds_deque_reallocate(deque, ds_deque_get_capacity_for_size(size));
    }
}

void ds_deque_allocate(ds_deque_t *deque, zend_long capacity)
{
    // -1 because an extra slot will be allocated for the tail.
    ds_deque_allocate_capacity_for_size(deque, capacity - 1);
}

static inline void ds_deque_auto_truncate(ds_deque_t *deque)
{
    // Automatically truncate if the size of the deque drops to a quarter.
    if (deque->size <= deque->capacity / 4) {
        if (deque->capacity / 2 >= DS_DEQUE_MIN_CAPACITY) {
            ds_deque_reallocate(deque, deque->capacity / 2);
        }
    }
}

static void ds_deque_clear_buffer(ds_deque_t *deque)
{
    zval *value;

    DS_DEQUE_FOREACH(deque, value) {
        DTOR_AND_UNDEF(value);
    }
    DS_DEQUE_FOREACH_END();

    deque->head = 0;
    deque->tail = 0;
    deque->size = 0;
}

void ds_deque_clear(ds_deque_t *deque)
{
    if (deque->size > 0) {
        ds_deque_clear_buffer(deque);

        if (deque->capacity > DS_DEQUE_MIN_CAPACITY) {
            REALLOC_ZVAL_BUFFER(deque->buffer, DS_DEQUE_MIN_CAPACITY);
            deque->capacity = DS_DEQUE_MIN_CAPACITY;
        }
    }
}

void ds_deque_free(ds_deque_t *deque)
{
    ds_deque_clear(deque);
    efree(deque->buffer);
    efree(deque);
}

/**
 * Translates a positional index into a buffer pointer.
 */
static inline zend_long ds_deque_lookup_index(ds_deque_t *deque, zend_long index)
{
    return (deque->head + index) & (deque->capacity - 1);
}

/**
 * Translates a positional index into a buffer index.
 */
static inline zval *ds_deque_lookup(ds_deque_t *deque, zend_long index)
{
    return deque->buffer + ds_deque_lookup_index(deque, index);
}

zval *ds_deque_get(ds_deque_t *deque, zend_long index)
{
    if ( ! ds_deque_valid_position(deque, index)) {
        return NULL;
    }

    return ds_deque_lookup(deque, index);
}

void ds_deque_set(ds_deque_t *deque, zend_long index, zval *value)
{
    if (ds_deque_valid_position(deque, index)) {
        zval *ptr = ds_deque_lookup(deque, index);
        zval_ptr_dtor(ptr);
        ZVAL_COPY(ptr, value);
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
        zend_long head = deque->head;
        zend_long tail = deque->tail;
        zend_long mask = deque->capacity - 1;

        while (head != tail) {
            tail = (tail - 1) & mask;
            SWAP_ZVAL(
                deque->buffer[head],
                deque->buffer[tail]
            );
            head = (head + 1) & mask;
        }
    }
}

ds_deque_t *ds_deque_reversed(ds_deque_t *deque)
{
    zval *src;
    zval *buf = ALLOC_ZVAL_BUFFER(deque->capacity);
    zval *dst = &buf[deque->size - 1];

    DS_DEQUE_FOREACH(deque, src) {
        ZVAL_COPY(dst, src);
        dst--;
    }
    DS_DEQUE_FOREACH_END();

    return ds_deque_from_buffer_ex(buf, deque->size, deque->capacity);
}


void ds_deque_shift(ds_deque_t *deque, zval *return_value)
{
    SET_AS_RETURN_AND_UNDEF(&deque->buffer[deque->head]);
    ds_deque_increment_head(deque);

    deque->size--;
    ds_deque_auto_truncate(deque);
}

void ds_deque_shift_throw(ds_deque_t *deque, zval *return_value)
{
    if (deque->size == 0) {
        NOT_ALLOWED_WHEN_EMPTY();
        return;
    }

    ds_deque_shift(deque, return_value);
}

void ds_deque_pop(ds_deque_t *deque, zval *return_value)
{
    ds_deque_decrement_tail(deque);
    SET_AS_RETURN_AND_UNDEF(&deque->buffer[deque->tail]);

    deque->size--;
    ds_deque_auto_truncate(deque);
}

void ds_deque_pop_throw(ds_deque_t *deque, zval *return_value)
{
    if (deque->size == 0) {
        NOT_ALLOWED_WHEN_EMPTY();
        return;
    }

    ds_deque_pop(deque, return_value);
}

void ds_deque_remove(ds_deque_t *deque, zend_long index, zval *return_value)
{
    if ( ! ds_deque_valid_position(deque, index)) {
        return;
    }

    // Basic shift if it's the first element in the sequence.
    if (index == 0) {
        ds_deque_shift(deque, return_value);
        return;
    }

    // Basic pop if it's the last element in the sequence.
    if (index == deque->size - 1) {
        ds_deque_pop(deque, return_value);
        return;
    }

    // Translate the positional index to a buffer index.
    index = ds_deque_lookup_index(deque, index);

    // Copy the value into the return value, then clear it.
    SET_AS_RETURN_AND_UNDEF(&deque->buffer[index]);

    if (index < deque->tail) {
        // Shift all values between the index and the tail.
        ds_deque_memmove(deque, index, index + 1, deque->tail - index);
        deque->tail--;

    } else {
        // Index comes after tail, and we know at this point that the index
        // is valid, so it must be after the head which has wrapped around.

        // Unshift all values between the head and the index.
        ds_deque_memmove(deque, deque->head + 1, deque->head, index - deque->head);
        deque->head++;
    }

    deque->size--;
    ds_deque_auto_truncate(deque);
}

void ds_deque_unshift_va(ds_deque_t *deque, VA_PARAMS)
{
    ds_deque_allocate_capacity_for_size(deque, deque->size + argc);
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

    deque->size++;

    if (deque->size == deque->capacity) {
        ds_deque_double_capacity(deque);
    }
}

void ds_deque_push_va(ds_deque_t *deque, VA_PARAMS)
{
    ds_deque_allocate_capacity_for_size(deque, deque->size + argc);

    while (argc--) {
        ZVAL_COPY(&deque->buffer[deque->tail], argv++);
        ds_deque_increment_tail(deque);
        deque->size++;
    }
}

void ds_deque_insert_va(ds_deque_t *deque, zend_long position, VA_PARAMS)
{
    zval *dst;
    zend_long index;

    // Basic push if inserting at the back.
    if (position == deque->size) {
        ds_deque_push_va(deque, VA_ARGS);
        return;
    }

    // Basic unshift if inserting at the front.
    if (position == 0) {
        ds_deque_unshift_va(deque, VA_ARGS);
        return;
    }

    // Check that the insert position is not out of range.
    if ( ! ds_deque_valid_position(deque, position)) {
        return;
    }

    // No op if no values.
    if (argc <= 0) {
        return;
    }

    // Make sure that we have enough room for the new values.
    ds_deque_allocate_capacity_for_size(deque, deque->size + argc);

    // Translate the positional index to a buffer index.
    index = ds_deque_lookup_index(deque, position);

    if (index <= deque->tail) {
        // The deque is either contiguous or we're inserting in between the
        // start of the buffer and the tail, where the head has wrapped around.

        // Check for overflow, which is possible because the head won't
        // always be at the start of the buffer.
        if ((deque->tail + argc) > deque->capacity) {

            // There isn't enough free space to the right of the tail,
            // so move the entire sequence all the way to the left.
            ds_deque_memmove(deque, 0, deque->head, deque->size);

            index -= deque->head;
            deque->head = 0;
            deque->tail = deque->size;
        }

        // Move the subsequence after the insertion point to the right
        // to make room for the new values.
        ds_deque_memmove(deque, (index + argc), index, (deque->tail - index));
        deque->tail += argc;
        dst = &deque->buffer[index];

    } else {
        // We're inserting between a wrapped around head and the end of the
        // buffer, and it's guaranteed that there's enough free space.
        ds_deque_memmove(deque, (deque->head - argc), deque->head, (index - deque->head));
        deque->head -= argc;
        dst = &deque->buffer[index - argc];
    }

    deque->size += argc;

    // Copy the new values into place.
    while (argc--) {
        ZVAL_COPY(dst++, argv++);
    }
}

static zend_long ds_deque_find_index(ds_deque_t *deque, zval *value)
{
    zend_long head = deque->head;
    zend_long mask = deque->capacity - 1;

    zend_long index;

    for (index = 0; index < deque->size; index++, head++) {
        if (zend_is_identical(value, &deque->buffer[head & mask])) {
            return index;
        }
    }

    return FAILURE;
}

void ds_deque_join(ds_deque_t *deque, char *str, size_t len, zval *return_value)
{
    ds_deque_reset_head(deque);

    ZVAL_STR(
        return_value,
        ds_join_zval_buffer(deque->buffer, deque->size, str, len)
    );
}

void ds_deque_find(ds_deque_t *deque, zval *value, zval *return_value)
{
    zend_long index = ds_deque_find_index(deque, value);

    if (index >= 0) {
        ZVAL_LONG(return_value, index);
    } else {
        ZVAL_FALSE(return_value);
    }
}

bool ds_deque_contains_va(ds_deque_t *deque, VA_PARAMS)
{
    while (argc-- > 0) {
        if (ds_deque_find_index(deque, argv++) == FAILURE) {
            return false;
        }
    }

    return true;
}

void ds_deque_rotate(ds_deque_t *deque, zend_long r)
{
    if (deque->size < 2 || r == 0) {
        return;
    }

    // We can do a quick O(n) rotate if the values are contiguous in the buffer.
    if (deque->head < deque->tail) {
        zval *x = deque->buffer + deque->head;
        zval *y = deque->buffer + deque->tail;
        ds_rotate_zval_range(x, y, r);
        return;
    }

    if (r < 0) {
        for (r = llabs(r) % deque->size; r > 0; r--) {

            // Pop, unshift
            ds_deque_decrement_head(deque);
            ds_deque_decrement_tail(deque);

            // Tail is now at last value, head is before the first.
            SWAP_ZVAL(
                deque->buffer[deque->tail],
                deque->buffer[deque->head]
            );
        }
    } else {
        for (r = r % deque->size; r > 0; r--) {

            // Tail is one past the last value, head is at first value.
            SWAP_ZVAL(
                deque->buffer[deque->tail],
                deque->buffer[deque->head]
            );

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

    return ds_zval_isset(ds_deque_lookup(deque, index), check_empty);
}

zval *ds_deque_get_last(ds_deque_t *deque)
{
    return &deque->buffer[(deque->tail - 1) & (deque->capacity - 1)];
}

zval *ds_deque_get_last_throw(ds_deque_t *deque)
{
    if (deque->size == 0) {
        NOT_ALLOWED_WHEN_EMPTY();
        return NULL;
    }

    return ds_deque_get_last(deque);
}

zval *ds_deque_get_first(ds_deque_t *deque)
{
    return &deque->buffer[deque->head];
}

zval *ds_deque_get_first_throw(ds_deque_t *deque)
{
    if (deque->size == 0) {
        NOT_ALLOWED_WHEN_EMPTY();
        return NULL;
    }

    return ds_deque_get_first(deque);
}

static int iterator_add(zend_object_iterator *iterator, void *puser)
{
    ds_deque_push((ds_deque_t *) puser, iterator->funcs->get_current_data(iterator));
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

    if (ds_is_array(values)) {
        add_array_to_deque(deque, Z_ARRVAL_P(values));
        return;
    }

    if (ds_is_traversable(values)) {
        add_traversable_to_deque(deque, values);
        return;
    }

    ARRAY_OR_TRAVERSABLE_REQUIRED();
}

ds_deque_t *ds_deque_merge(ds_deque_t *deque, zval *values)
{
    if (values && (ds_is_array(values) || ds_is_traversable(values))) {
        ds_deque_t *merged = ds_deque_clone(deque);
        ds_deque_push_all(merged, values);
        return merged;
    }

    ARRAY_OR_TRAVERSABLE_REQUIRED();
    return NULL;
}

void ds_deque_sort_callback(ds_deque_t *deque)
{
    ds_deque_reset_head(deque);
    ds_user_sort_zval_buffer(deque->buffer, deque->size);
}

void ds_deque_sort(ds_deque_t *deque)
{
    ds_deque_reset_head(deque);
    ds_sort_zval_buffer(deque->buffer, deque->size);
}

void ds_deque_apply(ds_deque_t *deque, FCI_PARAMS)
{
    zval *value;
    zval retval;

    DS_DEQUE_FOREACH(deque, value) {
        fci.param_count = 1;
        fci.params      = value;
        fci.retval      = &retval;

        if (zend_call_function(&fci, &fci_cache) == FAILURE || Z_ISUNDEF(retval)) {
            return;
        }

        zval_ptr_dtor(value);
        ZVAL_COPY_VALUE(value, &retval);
    }

    DS_DEQUE_FOREACH_END();
}

ds_deque_t *ds_deque_map(ds_deque_t *deque, FCI_PARAMS)
{
    zval retval;
    zval *value;
    zval *buffer = ALLOC_ZVAL_BUFFER(deque->capacity);
    zval *target = buffer;

    DS_DEQUE_FOREACH(deque, value) {
        fci.param_count = 1;
        fci.params      = value;
        fci.retval      = &retval;

        if (zend_call_function(&fci, &fci_cache) == FAILURE || Z_ISUNDEF(retval)) {

            // Release the values copied into the buffer on failure.
            for (; target > buffer; target--) {
                zval_ptr_dtor(target - 1);
            }

            zval_ptr_dtor(&retval);
            efree(buffer);
            return NULL;
        }

        ZVAL_COPY(target++, &retval);
        zval_ptr_dtor(&retval);
    }
    DS_DEQUE_FOREACH_END();

    return ds_deque_from_buffer_ex(buffer, deque->size, deque->capacity);
}

ds_deque_t *ds_deque_filter_callback(ds_deque_t *deque, FCI_PARAMS)
{
    if (deque->size == 0) {
        return ds_deque();

    } else {
        zval retval;
        zval *value;
        zval *buffer = ALLOC_ZVAL_BUFFER(deque->capacity);
        zval *target = buffer;

        DS_DEQUE_FOREACH(deque, value) {
            fci.param_count = 1;
            fci.params      = value;
            fci.retval      = &retval;

            // Catch potential exceptions or other errors during comparison.
            if (zend_call_function(&fci, &fci_cache) == FAILURE || Z_ISUNDEF(retval)) {

                // Release the values copied into the buffer on failure.
                for (; target > buffer; target--) {
                    zval_ptr_dtor(target - 1);
                }

                zval_ptr_dtor(&retval);
                efree(buffer);
                return NULL;
            }

            // Copy the value into the buffer if the callback returned true.
            if (zend_is_true(&retval)) {
                ZVAL_COPY(target++, value);
            }

            zval_ptr_dtor(&retval);
        }

        DS_DEQUE_FOREACH_END();
        return ds_deque_from_buffer(buffer, target - buffer);
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
    zval *value;
    zval carry;
    zval params[2];

    if (initial == NULL) {
        ZVAL_NULL(&carry);
    } else {
        ZVAL_COPY_VALUE(&carry, initial);
    }

    DS_DEQUE_FOREACH(deque, value) {
        ZVAL_COPY_VALUE(&params[0], &carry);
        ZVAL_COPY_VALUE(&params[1], value);

        fci.param_count = 2;
        fci.params      = params;
        fci.retval      = &carry;

        if (zend_call_function(&fci, &fci_cache) == FAILURE || Z_ISUNDEF(carry)) {
            zval_ptr_dtor(&carry);
            ZVAL_NULL(return_value);
            return;
        }

        Z_TRY_DELREF_P(&carry);
    }
    DS_DEQUE_FOREACH_END();
    ZVAL_COPY(return_value, &carry);
}

ds_deque_t *ds_deque_slice(ds_deque_t *deque, zend_long index, zend_long length)
{
    ds_normalize_slice_args(&index, &length, deque->size);

    if (length == 0) {
        return ds_deque();

    } else {
        zend_long capacity = ds_deque_get_capacity_for_size(length);

        zval *buffer = ALLOC_ZVAL_BUFFER(capacity);

        zval *src = deque->buffer;   // Read from the source deque's buffer
        zval *dst = buffer;          // Pointer to allocated destination buffer
        zval *end = buffer + length; // Position at which to stop writing.

        zend_long head = deque->head + index;
        zend_long mask = deque->capacity - 1;

        for (; dst < end; dst++, head++) {
            ZVAL_COPY(dst, &src[head & mask]);
        }

        return ds_deque_from_buffer_ex(buffer, length, capacity);
    }
}

void ds_deque_sum(ds_deque_t *deque, zval *return_value)
{
    zval *value;

    ZVAL_LONG(return_value, 0);

    DS_DEQUE_FOREACH(deque, value) {
        DS_ADD_TO_SUM(value, return_value);
    }
    DS_DEQUE_FOREACH_END();
}

ds_map_t *ds_deque_group_by(ds_deque_t *deque, zval *key)
{
    zval *value;

    // This table will associate the groups with their values.
    ds_htable_t *groups = ds_htable();

    // Attempt to parse the key as a callable.
    SETUP_CALLABLE_VARS();
    bool use_callable = zend_fcall_info_init(
        key, IS_CALLABLE_CHECK_SILENT, &fci, &fci_cache, NULL, NULL) == SUCCESS;

    DS_DEQUE_FOREACH(deque, value) {
        zval group;
        int status;

        if ( ! use_callable) {
            status = ds_read_dimension_or_property(value, key, &group);
        } else {
            fci.param_count = 1;
            fci.params      = value;
            fci.retval      = &group;
            status          = zend_call_function(&fci, &fci_cache);
        }

        // Free and exit if either the access or callable failed.
        if (status == FAILURE || Z_ISUNDEF(group)) {
            zval_ptr_dtor(&group);
            ds_htable_free(groups);
            return NULL;

        } else {
            ds_deque_t *values;
            ds_htable_bucket_t *bucket;

            // Find or create a bucket for the determined group.
            if (ds_htable_lookup_or_create(groups, &group, &bucket)) {
                values = Z_DS_DEQUE(bucket->value);
            } else {
                values = ds_deque();
                ZVAL_DS_DEQUE(&bucket->value, values);
            }

            // Push the value into the group.
            ds_deque_push(values, value);
            zval_ptr_dtor(&group);
        }
    }
    DS_DEQUE_FOREACH_END();
    return ds_map_ex(groups);
}

bool ds_deque_each(ds_deque_t *deque, FCI_PARAMS)
{
    zval retval;
    zval *value;

    DS_DEQUE_FOREACH(deque, value) {
        fci.param_count = 1;
        fci.params      = value;
        fci.retval      = &retval;

        // Catch potential exceptions or other errors during comparison.
        // Break iteration if the return value was false.
        if (zend_call_function(&fci, &fci_cache) == FAILURE
                || Z_ISUNDEF(retval)
                || Z_TYPE(retval) == IS_FALSE) {

            zval_ptr_dtor(&retval);
            return false;
        }
    }
    DS_DEQUE_FOREACH_END();

    zval_ptr_dtor(&retval);
    return true;
}

zend_long ds_deque_partition(ds_deque_t *deque)
{
    zval *value;
    zval *buffer = ALLOC_ZVAL_BUFFER(deque->size);

    zval *head = buffer;
    zval *tail = buffer + deque->size - 1;

    DS_DEQUE_FOREACH(deque, value) {
        if (zend_is_true(value)) {
            ZVAL_COPY_VALUE(head++, value);
        } else {
            ZVAL_COPY_VALUE(tail--, value);
        }
    }
    DS_DEQUE_FOREACH_END();

    // We have to reverse the failed values because we added them from the back.
    ds_reverse_zval_range(tail + 1, buffer + deque->size);

    efree(deque->buffer);
    deque->buffer = buffer;
    deque->head   = 0;
    deque->tail   = deque->size;

    return head - buffer;
}

zend_long ds_deque_partition_callback(ds_deque_t *deque, FCI_PARAMS)
{
    zval retval;
    zval *value;
    zval *buffer = ALLOC_ZVAL_BUFFER(deque->capacity);

    zval *head = buffer;
    zval *tail = buffer + deque->size - 1;

    DS_DEQUE_FOREACH(deque, value) {
        fci.param_count = 1;
        fci.params      = value;
        fci.retval      = &retval;

        // Catch potential exceptions or other errors during comparison.
        if (zend_call_function(&fci, &fci_cache) == FAILURE || Z_ISUNDEF(retval)) {
            zval_ptr_dtor(&retval);
            efree(buffer);
            return 0;
        }

        if (zend_is_true(&retval)) {
            ZVAL_COPY_VALUE(head++, value);
        } else {
            ZVAL_COPY_VALUE(tail--, value);
        }

        zval_ptr_dtor(&retval);
    }
    DS_DEQUE_FOREACH_END();

    // We have to reverse the failed values because we added them from the back.
    ds_reverse_zval_range(tail + 1, buffer + deque->size);

    efree(deque->buffer);
    deque->buffer = buffer;
    deque->head   = 0;
    deque->tail   = deque->size;

    return head - buffer;
}

ds_deque_t *ds_deque_pluck(ds_deque_t *deque, zval *key)
{
    zval value;
    zval *container;
    ds_deque_t *result = ds_deque_ex(deque->capacity);

    DS_DEQUE_FOREACH(deque, container) {
        if (ds_read_dimension_or_property(container, key, &value) == FAILURE) {
            zval_ptr_dtor(&value);
            ds_deque_free(result);
            return NULL;
        }

        ds_deque_push(result, &value);
        zval_ptr_dtor(&value);
    }
    DS_DEQUE_FOREACH_END();
    return result;
}
