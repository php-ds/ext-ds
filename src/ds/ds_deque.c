#include "../common.h"

#include "../php/iterators/php_deque_iterator.h"
#include "../php/handlers/php_deque_handlers.h"
#include "../php/classes/php_deque_ce.h"

#include "ds_deque.h"

static inline void ds_deque_increment_head(ds_deque_t *deque)
{
    deque->head = (deque->head + 1) & (deque->capacity - 1);
}

static inline void ds_deque_decrement_head(ds_deque_t *deque)
{
    deque->head = (deque->head - 1) & (deque->capacity - 1);
}

static inline void ds_deque_increment_tail(ds_deque_t *deque)
{
    deque->tail = (deque->tail + 1) & (deque->capacity - 1);
}

static inline void ds_deque_decrement_tail(ds_deque_t *deque)
{
    deque->tail = (deque->tail - 1) & (deque->capacity - 1);
}

static inline void ds_deque_memmove(
    ds_deque_t *deque,
    zend_long   dst,
    zend_long   src,
    zend_long   length
) {
    memmove(&deque->buffer[dst], &deque->buffer[src], length * sizeof(zval));
}

static inline zend_long ds_deque_get_capacity_for_size(zend_long size)
{
    return (zend_long) ds_next_power_of_2((uint32_t) size, DS_DEQUE_MIN_CAPACITY);
}

ds_deque_t *ds_deque()
{
    ds_deque_t *deque = ecalloc(1, sizeof(ds_deque_t));

    deque->buffer   = ds_allocate_zval_buffer(DS_DEQUE_MIN_CAPACITY);
    deque->capacity = DS_DEQUE_MIN_CAPACITY;
    deque->head     = 0;
    deque->tail     = 0;
    deque->size     = 0;

    return deque;
}

static ds_deque_t *ds_deque_preallocated(zend_long size)
{
    ds_deque_t *deque = ecalloc(1, sizeof(ds_deque_t));

    deque->capacity = ds_deque_get_capacity_for_size(size);
    deque->buffer   = ds_allocate_zval_buffer(deque->capacity);
    deque->head     = 0;
    deque->tail     = 0;
    deque->size     = 0;

    return deque;
}

static ds_deque_t *ds_deque_from_buffer(zval *buffer, zend_long capacity, zend_long size)
{
    ds_deque_t *deque = ecalloc(1, sizeof(ds_deque_t));

    deque->buffer   = buffer;
    deque->capacity = capacity;
    deque->head     = 0;
    deque->tail     = size;
    deque->size     = size;

    return deque;
}

ds_deque_t *ds_deque_clone(ds_deque_t *deque)
{
    zval *source;
    zval *buffer = ds_allocate_zval_buffer(deque->capacity);
    zval *target = buffer;

    DS_DEQUE_FOREACH(deque, source) {
        ZVAL_COPY(target, source);
        target++;
    }
    DS_DEQUE_FOREACH_END();

    return ds_deque_from_buffer(buffer, deque->capacity, deque->size);
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
            zval *buffer = ds_allocate_zval_buffer(deque->capacity);

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

    deque->buffer   = ds_reallocate_zval_buffer(deque->buffer, capacity, deque->capacity, deque->size);
    deque->capacity = capacity;
    deque->head     = 0;
    deque->tail     = deque->size; // Could have been zero before if buffer was full.
}

static inline void ds_deque_double_capacity(ds_deque_t *deque)
{
    ds_deque_reallocate(deque, deque->capacity << 1);
}

void ds_deque_allocate(ds_deque_t *deque, zend_long size)
{
    zend_long capacity = ds_deque_get_capacity_for_size(size);

    // if (capacity == deque->capacity) {
    //     ds_deque_reallocate(deque, capacity << 1);
    // }

    if (capacity > deque->capacity) {
        ds_deque_reallocate(deque, capacity);
    }
}

static inline void ds_deque_auto_truncate(ds_deque_t *deque)
{
    // Automatically truncate if the size of the deque drops to a quarter of the capacity.
    if (deque->size <= deque->capacity / 4) {
        if (deque->capacity / 2 >= DS_DEQUE_MIN_CAPACITY) {
            ds_deque_reallocate(deque, deque->capacity / 2);
        }
    }
}

void ds_deque_clear(ds_deque_t *deque)
{
    zval *val;

    DS_DEQUE_FOREACH(deque, val) {
        zval_ptr_dtor(val);
    }
    DS_DEQUE_FOREACH_END();

    deque->buffer   = ds_reallocate_zval_buffer(deque->buffer, DS_DEQUE_MIN_CAPACITY, deque->capacity, 0);
    deque->head     = 0;
    deque->tail     = 0;
    deque->size     = 0;
    deque->capacity = DS_DEQUE_MIN_CAPACITY;
}

void ds_deque_free(ds_deque_t *deque)
{
    zval *val;

    DS_DEQUE_FOREACH(deque, val) {
        zval_ptr_dtor(val);
    }
    DS_DEQUE_FOREACH_END();

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
    zval *buf = ds_allocate_zval_buffer(deque->capacity);
    zval *dst = &buf[deque->size - 1];

    DS_DEQUE_FOREACH(deque, src) {
        ZVAL_COPY(dst, src);
        dst--;
    }
    DS_DEQUE_FOREACH_END();

    return ds_deque_from_buffer(buf, deque->capacity, deque->size);
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
    ds_deque_allocate(deque, deque->size + argc);
    deque->size += argc;

    while (argc--) {
        ds_deque_decrement_head(deque);
        ZVAL_COPY(&deque->buffer[deque->head], &argv[argc]);
    }
}

void ds_deque_push(ds_deque_t *deque, zval *value)
{
    if (deque->size == deque->capacity) {
        ds_deque_double_capacity(deque);
    }

    ZVAL_COPY(&deque->buffer[deque->tail], value);
    ds_deque_increment_tail(deque);
    deque->size++;
}

void ds_deque_push_va(ds_deque_t *deque, VA_PARAMS)
{
    ds_deque_allocate(deque, deque->size + argc);

    while (argc) {
        ZVAL_COPY(&deque->buffer[deque->tail], argv);
        ds_deque_increment_tail(deque);
        deque->size++;

        argc--;
        argv++;
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
    ds_deque_allocate(deque, deque->size + argc);

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

void ds_deque_rotate(ds_deque_t *deque, zend_long n)
{
    if (deque->size < 2) {
        return;
    }

    if (n < 0) {
        for (n = llabs(n) % deque->size; n > 0; n--) {

            // Pop, unshift
            ds_deque_decrement_head(deque);
            ds_deque_decrement_tail(deque);

            // Tail is now at last value, head is before the first.
            SWAP_ZVAL(deque->buffer[deque->tail], deque->buffer[deque->head]);
        }
    } else if (n > 0) {
        for (n = n % deque->size; n > 0; n--) {

            // Tail is one past the last value, head is at first value.
            SWAP_ZVAL(deque->buffer[deque->tail], deque->buffer[deque->head]);

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
    zval *buffer = ds_allocate_zval_buffer(deque->capacity);
    zval *target = buffer;

    DS_DEQUE_FOREACH(deque, value) {
        fci.param_count = 1;
        fci.params      = value;
        fci.retval      = &retval;

        if (zend_call_function(&fci, &fci_cache) == FAILURE || Z_ISUNDEF(retval)) {

            // Release the values copied into the buffer on failure.
            target--;
            while (target > buffer) {
                zval_ptr_dtor(target--);
            }

            zval_ptr_dtor(&retval);
            efree(buffer);
            return NULL;
        }

        ZVAL_COPY(target++, &retval);
        zval_ptr_dtor(&retval);
    }
    DS_DEQUE_FOREACH_END();

    return ds_deque_from_buffer(buffer, deque->capacity, deque->size);
}

ds_deque_t *ds_deque_filter_callback(ds_deque_t *deque, FCI_PARAMS)
{
    if (deque->size == 0) {
        return ds_deque();

    } else {
        zval retval;
        zval *val;
        zval *buf = ds_allocate_zval_buffer(deque->capacity);
        zval *dst = buf;

        DS_DEQUE_FOREACH(deque, val) {
            fci.param_count = 1;
            fci.params      = val;
            fci.retval      = &retval;

            // Catch potential exceptions or other errors during comparison.
            if (zend_call_function(&fci, &fci_cache) == FAILURE || Z_ISUNDEF(retval)) {

                // Release the values copied into the buffer on failure.
                dst--;
                while (dst > buf) {
                    zval_ptr_dtor(dst--);
                }

                zval_ptr_dtor(&retval);
                efree(buf);
                return NULL;
            }

            // Copy the value into the buffer if the callback returned true.
            if (EXPECTED_BOOL_IS_TRUE(&retval)) {
                ZVAL_COPY(dst++, val);
            }

            zval_ptr_dtor(&retval);
        }

        DS_DEQUE_FOREACH_END();
        return ds_deque_from_buffer(buf, ds_deque_get_capacity_for_size(dst - buf), (dst - buf));
    }
}

ds_deque_t *ds_deque_filter(ds_deque_t *deque)
{
    if (deque->size == 0) {
        return ds_deque();

    } else {
        zval *buf = ds_allocate_zval_buffer(deque->capacity);
        zval *dst = buf;
        zval *src = NULL;

        DS_DEQUE_FOREACH(deque, src) {
            if (zend_is_true(src)) {
                ZVAL_COPY(dst++, src);
            }
        }
        DS_DEQUE_FOREACH_END();

        return ds_deque_from_buffer(buf, ds_deque_get_capacity_for_size(dst - buf), (dst - buf));
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
        ds_deque_t *result = ds_deque_preallocated(length);

        for (; length > 0; length--) {
            ds_deque_push(result, ds_deque_lookup(deque, index++));
        }

        return result;
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
