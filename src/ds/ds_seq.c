#include "../common.h"

#include "../php/iterators/php_seq_iterator.h"
#include "../php/handlers/php_seq_handlers.h"
#include "../php/classes/php_seq_ce.h"

#include "ds_seq.h"

static inline void ds_seq_increment_head(ds_seq_t *seq)
{
    seq->head = (seq->head + 1) & (seq->capacity - 1);
}

static inline void ds_seq_decrement_head(ds_seq_t *seq)
{
    seq->head = (seq->head - 1) & (seq->capacity - 1);
}

static inline void ds_seq_increment_tail(ds_seq_t *seq)
{
    seq->tail = (seq->tail + 1) & (seq->capacity - 1);
}

static inline void ds_seq_decrement_tail(ds_seq_t *seq)
{
    seq->tail = (seq->tail - 1) & (seq->capacity - 1);
}

static inline void ds_seq_memmove(
    ds_seq_t *seq,
    zend_long   dst,
    zend_long   src,
    zend_long   length
) {
    memmove(&seq->buffer[dst], &seq->buffer[src], length * sizeof(zval));
}

static inline zend_long ds_seq_get_capacity_for_size(zend_long size)
{
    return (zend_long) ds_next_power_of_2((uint32_t) size, DS_SEQ_MIN_CAPACITY);
}

ds_seq_t *ds_seq()
{
    ds_seq_t *seq = ecalloc(1, sizeof(ds_seq_t));

    seq->buffer   = ds_allocate_zval_buffer(DS_SEQ_MIN_CAPACITY);
    seq->capacity = DS_SEQ_MIN_CAPACITY;
    seq->head     = 0;
    seq->tail     = 0;
    seq->size     = 0;

    return seq;
}

static ds_seq_t *ds_seq_preallocated(zend_long size)
{
    ds_seq_t *seq = ecalloc(1, sizeof(ds_seq_t));

    seq->capacity = ds_seq_get_capacity_for_size(size);
    seq->buffer   = ds_allocate_zval_buffer(seq->capacity);
    seq->head     = 0;
    seq->tail     = 0;
    seq->size     = 0;

    return seq;
}

static ds_seq_t *ds_seq_from_buffer(zval *buffer, zend_long capacity, zend_long size)
{
    ds_seq_t *seq = ecalloc(1, sizeof(ds_seq_t));

    seq->buffer   = buffer;
    seq->capacity = capacity;
    seq->head     = 0;
    seq->tail     = size;
    seq->size     = size;

    return seq;
}

ds_seq_t *ds_seq_clone(ds_seq_t *seq)
{
    zval *source;
    zval *buffer = ds_allocate_zval_buffer(seq->capacity);
    zval *target = buffer;

    DS_SEQ_FOREACH(seq, source) {
        ZVAL_COPY(target, source);
        target++;
    }
    DS_SEQ_FOREACH_END();

    return ds_seq_from_buffer(buffer, seq->capacity, seq->size);
}

void ds_seq_separate(ds_seq_t **seq)
{
    if ((*seq)->refs > 0) {
        (*seq)->refs--;
        *seq = ds_seq_clone(*seq);
    }
}

void ds_seq_release(ds_seq_t *seq)
{
    if (seq->refs > 0) {
        seq->refs--;
    } else {
        ds_seq_free(seq);
    }
}


static inline bool ds_seq_valid_position(ds_seq_t *seq, zend_long index)
{
    if (index < 0 || index >= seq->size) {
        INDEX_OUT_OF_RANGE(index, seq->size);
        return false;
    }

    return true;
}

// Makes sure that the seq's head is at index 0.
void ds_seq_reset_head(ds_seq_t *seq)
{
    if (seq->head == 0) {
        return;
    }

    if (seq->head < seq->tail) {
        ds_seq_memmove(seq, 0, seq->head, seq->size);

    } else {
        zend_long h = seq->head;
        zend_long t = seq->tail;
        zend_long r = seq->capacity - h; // Number of values on the right.

        // Check if there's enough room to push the left partition forward and
        // the wrapped values (right partition) to the front.
        if (r < (h - t)) {
            ds_seq_memmove(seq, r, 0, t);
            ds_seq_memmove(seq, 0, h, r);

        } else {
            // We don't have enough temporary space to work with, so create
            // a new buffer, copy to it, then replace the current buffer.
            zval *buffer = ds_allocate_zval_buffer(seq->capacity);

            memcpy(&buffer[0], &seq->buffer[h], r * sizeof(zval));
            memcpy(&buffer[r], &seq->buffer[0], t * sizeof(zval));

            FREE_AND_REPLACE(seq->buffer, buffer);
        }
    }

    seq->head = 0;
    seq->tail = seq->size;
}

static void ds_seq_reallocate(ds_seq_t *seq, zend_long capacity)
{
    ds_seq_reset_head(seq);

    seq->buffer   = ds_reallocate_zval_buffer(seq->buffer, capacity, seq->capacity, seq->size);
    seq->capacity = capacity;
    seq->head     = 0;
    seq->tail     = seq->size; // Could have been zero before if buffer was full.
}

static inline void ds_seq_double_capacity(ds_seq_t *seq)
{
    ds_seq_reallocate(seq, seq->capacity << 1);
}

void ds_seq_allocate(ds_seq_t *seq, zend_long size)
{
    zend_long capacity = ds_seq_get_capacity_for_size(size);

    // if (capacity == seq->capacity) {
    //     ds_seq_reallocate(seq, capacity << 1);
    // }

    if (capacity > seq->capacity) {
        ds_seq_reallocate(seq, capacity);
    }
}

static inline void ds_seq_auto_truncate(ds_seq_t *seq)
{
    // Automatically truncate if the size of the seq drops to a quarter of the capacity.
    if (seq->size <= seq->capacity / 4) {
        if (seq->capacity / 2 >= DS_SEQ_MIN_CAPACITY) {
            ds_seq_reallocate(seq, seq->capacity / 2);
        }
    }
}

void ds_seq_clear(ds_seq_t *seq)
{
    zval *val;

    DS_SEQ_FOREACH(seq, val) {
        zval_ptr_dtor(val);
    }
    DS_SEQ_FOREACH_END();

    seq->buffer   = ds_reallocate_zval_buffer(seq->buffer, DS_SEQ_MIN_CAPACITY, seq->capacity, 0);
    seq->head     = 0;
    seq->tail     = 0;
    seq->size     = 0;
    seq->capacity = DS_SEQ_MIN_CAPACITY;
}

void ds_seq_free(ds_seq_t *seq)
{
    zval *val;

    DS_SEQ_FOREACH(seq, val) {
        zval_ptr_dtor(val);
    }
    DS_SEQ_FOREACH_END();

    efree(seq->buffer);
    efree(seq);
}

/**
 * Translates a positional index into a buffer pointer.
 */
static inline zend_long ds_seq_lookup_index(ds_seq_t *seq, zend_long index)
{
    return (seq->head + index) & (seq->capacity - 1);
}

/**
 * Translates a positional index into a buffer index.
 */
static inline zval *ds_seq_lookup(ds_seq_t *seq, zend_long index)
{
    return seq->buffer + ds_seq_lookup_index(seq, index);
}

zval *ds_seq_get(ds_seq_t *seq, zend_long index)
{
    if ( ! ds_seq_valid_position(seq, index)) {
        return NULL;
    }

    return ds_seq_lookup(seq, index);
}

void ds_seq_set(ds_seq_t *seq, zend_long index, zval *value)
{
    if (ds_seq_valid_position(seq, index)) {
        zval *ptr = ds_seq_lookup(seq, index);
        zval_ptr_dtor(ptr);
        ZVAL_COPY(ptr, value);
    }
}

void ds_seq_reverse(ds_seq_t *seq)
{
    if (seq->head < seq->tail) {
        ds_reverse_zval_range(
            seq->buffer + seq->head,
            seq->buffer + seq->tail
        );

    } else {
        zend_long head = seq->head;
        zend_long tail = seq->tail;
        zend_long mask = seq->capacity - 1;

        while (head != tail) {
            tail = (tail - 1) & mask;
            SWAP_ZVAL(
                seq->buffer[head],
                seq->buffer[tail]
            );
            head = (head + 1) & mask;
        }
    }
}

ds_seq_t *ds_seq_reversed(ds_seq_t *seq)
{
    zval *src;
    zval *buf = ds_allocate_zval_buffer(seq->capacity);
    zval *dst = &buf[seq->size - 1];

    DS_SEQ_FOREACH(seq, src) {
        ZVAL_COPY(dst, src);
        dst--;
    }
    DS_SEQ_FOREACH_END();

    return ds_seq_from_buffer(buf, seq->capacity, seq->size);
}


void ds_seq_shift(ds_seq_t *seq, zval *return_value)
{
    SET_AS_RETURN_AND_UNDEF(&seq->buffer[seq->head]);
    ds_seq_increment_head(seq);

    seq->size--;
    ds_seq_auto_truncate(seq);
}

void ds_seq_shift_throw(ds_seq_t *seq, zval *return_value)
{
    if (seq->size == 0) {
        NOT_ALLOWED_WHEN_EMPTY();
        return;
    }

    ds_seq_shift(seq, return_value);
}

void ds_seq_pop(ds_seq_t *seq, zval *return_value)
{
    ds_seq_decrement_tail(seq);
    SET_AS_RETURN_AND_UNDEF(&seq->buffer[seq->tail]);

    seq->size--;
    ds_seq_auto_truncate(seq);
}

void ds_seq_pop_throw(ds_seq_t *seq, zval *return_value)
{
    if (seq->size == 0) {
        NOT_ALLOWED_WHEN_EMPTY();
        return;
    }

    ds_seq_pop(seq, return_value);
}

void ds_seq_remove(ds_seq_t *seq, zend_long index, zval *return_value)
{
    if ( ! ds_seq_valid_position(seq, index)) {
        return;
    }

    // Basic shift if it's the first element in the sequence.
    if (index == 0) {
        ds_seq_shift(seq, return_value);
        return;
    }

    // Basic pop if it's the last element in the sequence.
    if (index == seq->size - 1) {
        ds_seq_pop(seq, return_value);
        return;
    }

    // Translate the positional index to a buffer index.
    index = ds_seq_lookup_index(seq, index);

    // Copy the value into the return value, then clear it.
    SET_AS_RETURN_AND_UNDEF(&seq->buffer[index]);

    if (index < seq->tail) {
        // Shift all values between the index and the tail.
        ds_seq_memmove(seq, index, index + 1, seq->tail - index);
        seq->tail--;

    } else {
        // Index comes after tail, and we know at this point that the index
        // is valid, so it must be after the head which has wrapped around.

        // Unshift all values between the head and the index.
        ds_seq_memmove(seq, seq->head + 1, seq->head, index - seq->head);
        seq->head++;
    }

    seq->size--;
    ds_seq_auto_truncate(seq);
}

void ds_seq_unshift_va(ds_seq_t *seq, VA_PARAMS)
{
    ds_seq_allocate(seq, seq->size + argc);
    seq->size += argc;

    while (argc--) {
        ds_seq_decrement_head(seq);
        ZVAL_COPY(&seq->buffer[seq->head], &argv[argc]);
    }
}

void ds_seq_push(ds_seq_t *seq, zval *value)
{
    if (seq->size == seq->capacity) {
        ds_seq_double_capacity(seq);
    }

    ZVAL_COPY(&seq->buffer[seq->tail], value);
    ds_seq_increment_tail(seq);
    seq->size++;
}

void ds_seq_push_va(ds_seq_t *seq, VA_PARAMS)
{
    ds_seq_allocate(seq, seq->size + argc);

    while (argc) {
        ZVAL_COPY(&seq->buffer[seq->tail], argv);
        ds_seq_increment_tail(seq);
        seq->size++;

        argc--;
        argv++;
    }
}

void ds_seq_insert_va(ds_seq_t *seq, zend_long position, VA_PARAMS)
{
    zval *dst;
    zend_long index;

    // Basic push if inserting at the back.
    if (position == seq->size) {
        ds_seq_push_va(seq, VA_ARGS);
        return;
    }

    // Basic unshift if inserting at the front.
    if (position == 0) {
        ds_seq_unshift_va(seq, VA_ARGS);
        return;
    }

    // Check that the insert position is not out of range.
    if ( ! ds_seq_valid_position(seq, position)) {
        return;
    }

    // No op if no values.
    if (argc <= 0) {
        return;
    }

    // Make sure that we have enough room for the new values.
    ds_seq_allocate(seq, seq->size + argc);

    // Translate the positional index to a buffer index.
    index = ds_seq_lookup_index(seq, position);

    if (index <= seq->tail) {
        // The seq is either contiguous or we're inserting in between the
        // start of the buffer and the tail, where the head has wrapped around.

        // Check for overflow, which is possible because the head won't
        // always be at the start of the buffer.
        if ((seq->tail + argc) > seq->capacity) {

            // There isn't enough free space to the right of the tail,
            // so move the entire sequence all the way to the left.
            ds_seq_memmove(seq, 0, seq->head, seq->size);

            index -= seq->head;
            seq->head = 0;
            seq->tail = seq->size;
        }

        // Move the subsequence after the insertion point to the right
        // to make room for the new values.
        ds_seq_memmove(seq, (index + argc), index, (seq->tail - index));
        seq->tail += argc;
        dst = &seq->buffer[index];

    } else {
        // We're inserting between a wrapped around head and the end of the
        // buffer, and it's guaranteed that there's enough free space.
        ds_seq_memmove(seq, (seq->head - argc), seq->head, (index - seq->head));
        seq->head -= argc;
        dst = &seq->buffer[index - argc];
    }

    seq->size += argc;

    // Copy the new values into place.
    while (argc--) {
        ZVAL_COPY(dst++, argv++);
    }
}

static zend_long ds_seq_find_index(ds_seq_t *seq, zval *value)
{
    zend_long head = seq->head;
    zend_long mask = seq->capacity - 1;

    zend_long index;

    for (index = 0; index < seq->size; index++, head++) {
        if (zend_is_identical(value, &seq->buffer[head & mask])) {
            return index;
        }
    }

    return FAILURE;
}

void ds_seq_join(ds_seq_t *seq, char *str, size_t len, zval *return_value)
{
    ds_seq_reset_head(seq);

    ZVAL_STR(
        return_value,
        ds_join_zval_buffer(seq->buffer, seq->size, str, len)
    );
}

void ds_seq_find(ds_seq_t *seq, zval *value, zval *return_value)
{
    zend_long index = ds_seq_find_index(seq, value);

    if (index >= 0) {
        ZVAL_LONG(return_value, index);
    } else {
        ZVAL_FALSE(return_value);
    }
}

bool ds_seq_contains_va(ds_seq_t *seq, VA_PARAMS)
{
    while (argc-- > 0) {
        if (ds_seq_find_index(seq, argv++) == FAILURE) {
            return false;
        }
    }

    return true;
}

void ds_seq_rotate(ds_seq_t *seq, zend_long n)
{
    if (seq->size < 2) {
        return;
    }

    if (n < 0) {
        for (n = llabs(n) % seq->size; n > 0; n--) {

            // Pop, unshift
            ds_seq_decrement_head(seq);
            ds_seq_decrement_tail(seq);

            // Tail is now at last value, head is before the first.
            SWAP_ZVAL(seq->buffer[seq->tail], seq->buffer[seq->head]);
        }
    } else if (n > 0) {
        for (n = n % seq->size; n > 0; n--) {

            // Tail is one past the last value, head is at first value.
            SWAP_ZVAL(seq->buffer[seq->tail], seq->buffer[seq->head]);

            // Shift, push
            ds_seq_increment_head(seq);
            ds_seq_increment_tail(seq);
        }
    }
}

void ds_seq_to_array(ds_seq_t *seq, zval *array)
{
    if (seq->size == 0) {
        array_init(array);
        return;

    } else {
        zval *value;
        array_init_size(array, seq->size);

        DS_SEQ_FOREACH(seq, value) {
            add_next_index_zval(array, value);
            Z_TRY_ADDREF_P(value);
        }
        DS_SEQ_FOREACH_END();
    }
}

bool ds_seq_index_exists(ds_seq_t *seq, zend_long index)
{
    return index >= 0 && index < seq->size;
}

bool ds_seq_isset(ds_seq_t *seq, zend_long index, int check_empty)
{
    if (!ds_seq_index_exists(seq, index)) {
        return false;
    }

    return ds_zval_isset(ds_seq_lookup(seq, index), check_empty);
}

zval *ds_seq_get_last(ds_seq_t *seq)
{
    return &seq->buffer[(seq->tail - 1) & (seq->capacity - 1)];
}

zval *ds_seq_get_last_throw(ds_seq_t *seq)
{
    if (seq->size == 0) {
        NOT_ALLOWED_WHEN_EMPTY();
        return NULL;
    }

    return ds_seq_get_last(seq);
}

zval *ds_seq_get_first(ds_seq_t *seq)
{
    return &seq->buffer[seq->head];
}

zval *ds_seq_get_first_throw(ds_seq_t *seq)
{
    if (seq->size == 0) {
        NOT_ALLOWED_WHEN_EMPTY();
        return NULL;
    }

    return ds_seq_get_first(seq);
}

static int iterator_add(zend_object_iterator *iterator, void *puser)
{
    ds_seq_push((ds_seq_t *) puser, iterator->funcs->get_current_data(iterator));
    return ZEND_HASH_APPLY_KEEP;
}

static void add_traversable_to_seq(ds_seq_t *seq, zval *obj)
{
    spl_iterator_apply(obj, iterator_add, seq);
}

static void add_array_to_seq(ds_seq_t *seq, HashTable *arr)
{
    zval *value;
    ZEND_HASH_FOREACH_VAL(arr, value) {
        ds_seq_push(seq, value);
    }
    ZEND_HASH_FOREACH_END();
}

void ds_seq_push_all(ds_seq_t *seq, zval *values)
{
    if ( ! values) {
        return;
    }

    if (ds_is_array(values)) {
        add_array_to_seq(seq, Z_ARRVAL_P(values));
        return;
    }

    if (ds_is_traversable(values)) {
        add_traversable_to_seq(seq, values);
        return;
    }

    ARRAY_OR_TRAVERSABLE_REQUIRED();
}

ds_seq_t *ds_seq_merge(ds_seq_t *seq, zval *values)
{
    if (values && (ds_is_array(values) || ds_is_traversable(values))) {
        ds_seq_t *merged = ds_seq_clone(seq);
        ds_seq_push_all(merged, values);
        return merged;
    }

    ARRAY_OR_TRAVERSABLE_REQUIRED();
    return NULL;
}

void ds_seq_sort_callback(ds_seq_t *seq)
{
    ds_seq_reset_head(seq);
    ds_user_sort_zval_buffer(seq->buffer, seq->size);
}

void ds_seq_sort(ds_seq_t *seq)
{
    ds_seq_reset_head(seq);
    ds_sort_zval_buffer(seq->buffer, seq->size);
}

void ds_seq_apply(ds_seq_t *seq, FCI_PARAMS)
{
    zval *value;
    zval retval;

    DS_SEQ_FOREACH(seq, value) {
        fci.param_count = 1;
        fci.params      = value;
        fci.retval      = &retval;

        if (zend_call_function(&fci, &fci_cache) == FAILURE || Z_ISUNDEF(retval)) {
            return;
        }

        zval_ptr_dtor(value);
        ZVAL_COPY_VALUE(value, &retval);
    }

    DS_SEQ_FOREACH_END();
}

ds_seq_t *ds_seq_map(ds_seq_t *seq, FCI_PARAMS)
{
    zval retval;
    zval *value;
    zval *buffer = ds_allocate_zval_buffer(seq->capacity);
    zval *target = buffer;

    DS_SEQ_FOREACH(seq, value) {
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
    DS_SEQ_FOREACH_END();

    return ds_seq_from_buffer(buffer, seq->capacity, seq->size);
}

ds_seq_t *ds_seq_filter_callback(ds_seq_t *seq, FCI_PARAMS)
{
    if (seq->size == 0) {
        return ds_seq();

    } else {
        zval retval;
        zval *val;
        zval *buf = ds_allocate_zval_buffer(seq->capacity);
        zval *dst = buf;

        DS_SEQ_FOREACH(seq, val) {
            fci.param_count = 1;
            fci.params      = val;
            fci.retval      = &retval;

            // Catch potential exceptions or other errors during comparison.
            if (zend_call_function(&fci, &fci_cache) == FAILURE || Z_ISUNDEF(retval)) {

                // Release the values copied into the buffer on failure.
                dst--;
                while (dst >= buf) {
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

        DS_SEQ_FOREACH_END();
        return ds_seq_from_buffer(buf, ds_seq_get_capacity_for_size(dst - buf), (dst - buf));
    }
}

ds_seq_t *ds_seq_filter(ds_seq_t *seq)
{
    if (seq->size == 0) {
        return ds_seq();

    } else {
        zval *buf = ds_allocate_zval_buffer(seq->capacity);
        zval *dst = buf;
        zval *src = NULL;

        DS_SEQ_FOREACH(seq, src) {
            if (zend_is_true(src)) {
                ZVAL_COPY(dst++, src);
            }
        }
        DS_SEQ_FOREACH_END();

        return ds_seq_from_buffer(buf, ds_seq_get_capacity_for_size(dst - buf), (dst - buf));
    }
}

void ds_seq_reduce(ds_seq_t *seq, zval *initial, zval *return_value, FCI_PARAMS)
{
    zval *value;
    zval carry;
    zval params[2];

    if (initial == NULL) {
        ZVAL_NULL(&carry);
    } else {
        ZVAL_COPY_VALUE(&carry, initial);
    }

    DS_SEQ_FOREACH(seq, value) {
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
    DS_SEQ_FOREACH_END();
    ZVAL_COPY(return_value, &carry);
}

ds_seq_t *ds_seq_slice(ds_seq_t *seq, zend_long index, zend_long length)
{
    ds_normalize_slice_args(&index, &length, seq->size);

    if (length == 0) {
        return ds_seq();

    } else {
        ds_seq_t *result = ds_seq_preallocated(length);

        for (; length > 0; length--) {
            ds_seq_push(result, ds_seq_lookup(seq, index++));
        }

        return result;
    }
}

void ds_seq_sum(ds_seq_t *seq, zval *return_value)
{
    zval *value;

    ZVAL_LONG(return_value, 0);

    DS_SEQ_FOREACH(seq, value) {
        DS_ADD_TO_SUM(value, return_value);
    }
    DS_SEQ_FOREACH_END();
}
