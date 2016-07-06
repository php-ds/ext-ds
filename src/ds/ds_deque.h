#ifndef DS_DEQUE_H
#define DS_DEQUE_H

#include "../common.h"

/**
 *
 */
typedef struct _ds_deque_t {
    zval      *buffer;
    zend_long  capacity;
    zend_long  head;
    zend_long  tail;
} ds_deque_t;

#define DS_DEQUE_MIN_CAPACITY 8 // Must be a power of 2

#define DS_DEQUE_SIZE(d)    (((d)->tail  - (d)->head) & ((d)->capacity - 1))
#define DS_DEQUE_IS_EMPTY(d) ((d)->head == (d)->tail)

#define DS_DEQUE_FOREACH(_d, _v)                            \
do {                                                        \
    ds_deque_t  *_deque   = _d;                             \
    zval        *_buffer  = _deque->buffer;                 \
    zend_long    _tail    = _deque->tail;                   \
    zend_long    _mask    = _deque->capacity - 1;           \
    zend_long    _head    = _deque->head;                   \
                                                            \
    for (; _head != _tail; _head = (_head + 1) & _mask) {   \
        _v = _buffer + _head;


#define DS_DEQUE_FOREACH_REVERSED(_d, _v)               \
do {                                                    \
    ds_deque_t  *_deque   = _d;                         \
    zval        *_buffer  = _deque->buffer;             \
    zend_long    _head    = _deque->head;               \
    zend_long    _tail    = _deque->tail;               \
    zend_long    _mask    = _deque->capacity - 1;       \
                                                        \
    while ((_tail = (_tail - 1) & _mask) != _head) {    \
        _v = _buffer + _tail;

#define DS_DEQUE_FOREACH_END() \
    } \
} while (0)

/**
 * Creates a copy of a deque.
 */
ds_deque_t *ds_deque_clone(ds_deque_t *src);

/**
 * Creates a new empty deque.
 */
ds_deque_t *ds_deque();

/**
 * Creates a new deque with initial capacity.
 */
ds_deque_t *ds_deque_ex(zend_long capacity);

/**
 * Creates a new deque using a buffer of zval values of a given size.
 */
ds_deque_t *ds_deque_from_buffer(zval *buffer, zend_long size);

/**
 * Removes all values from a deque and reallocates its buffer to the minimum
 * capacity.
 */
void ds_deque_clear(ds_deque_t *deque);

/**
 * Clears and frees a deque and its buffer.
 */
void ds_deque_destroy(ds_deque_t *deque);

/**
 * Ensures that a deque's buffer has at least a specified amount of capacity.
 */
void ds_deque_user_allocate(ds_deque_t *deque, zend_long capacity);

/**
 * Sets the value in a deque by index.
 */
void ds_deque_set(ds_deque_t *deque, zend_long index, zval *value);

/**
 * Copies the last value of a deque into a return value then removes it.
 */
void ds_deque_pop(ds_deque_t *deque, zval *return_value);

/**
 * Copies the first value of a deque into a return value then removes it.
 */
void ds_deque_shift(ds_deque_t *deque, zval *return_value);

/**
 * Attempts to find a value in a deque's sequence (strict). The return value
 * will be IS_LONG representing the position of the value in the sequence,
 * or IS_FALSE if the value could not be found.
 */
void ds_deque_find(ds_deque_t *deque, zval *value, zval *return_value);

/**
 * Removes a value from a deque by index, causing all successive values to
 * shift one position to the left.
 */
void ds_deque_remove(ds_deque_t *deque, zend_long index, zval *return_value);

/**
 * Inserts a single value into a position in a deque,
 * where 0 >= position <= size.
 */
// void ds_deque_insert(deque *deque, zend_long index, zval *value);

/**
 * Inserts zero or more values into a position in a deque,
 * where 0 >= position <= size.
 */
void ds_deque_insert_va(ds_deque_t *deque, zend_long index, VA_PARAMS);

/**
 * Unshifts (prepend) one value into a ds_deque_t.
 */
// void ds_deque_unshift(ds_deque_t *deque, zval *value);

/**
 * Unshifts (prepend) zero or more values into a deque.
 */
void ds_deque_unshift_va(ds_deque_t *deque, VA_PARAMS);

/**
 * Returns whether zero or more values are in a deque.
 * The result will be false if no values are given.
 */
bool ds_deque_contains_va(ds_deque_t *deque, VA_PARAMS);

/**
 * Pushes one value onto the end of a deque's sequence.
 */
void ds_deque_push(ds_deque_t *deque, zval *value);

/**
 * Pushes zero or more values onto the end of a deque's sequence.
 */
void ds_deque_push_va(ds_deque_t *deque, VA_PARAMS);

/**
 * Pushes all values from either a PHP array or other iterable onto a deque.
 */
void ds_deque_push_all(ds_deque_t *deque, zval *values);

/**
 * Returns a pointer to the value at a given inden in a deque. NULL if invalid.
 */
zval *ds_deque_get(ds_deque_t *deque, zend_long index);

/**
 * Returns a pointer to the last value of a deque, or NULL if invalid.
 */
zval *ds_deque_get_last(ds_deque_t *deque);

/**
 * Returns a pointer to the last value of a deque, or NULL if invalid.
 */
zval *ds_deque_get_first(ds_deque_t *deque);

/**
 * Returns true if the value at an index is set for a given deque.
 * Used to determine the result of isset() and empty().
 */
bool ds_deque_isset(ds_deque_t *deque, zend_long index, int check_empty);

/**
 * Joins each value of a deque together using an optional 'glue'. The returned
 * zval* will be of type IS_STRING. If no glue is to be used, pass NULL and 0.
 * Similar to implode.
 */
void ds_deque_join(ds_deque_t *deque, char *str, size_t len, zval *return_value);

/**
 * Creates a new deque using the result of applying a user functions to each
 * value of a given deque.
 */
ds_deque_t *ds_deque_map(ds_deque_t *deque, FCI_PARAMS);

/**
 * Creates a new deque containing all the values of a given deque that are
 * considered true according to `zend_is_true`.
 */
ds_deque_t *ds_deque_filter(ds_deque_t *deque);

/**
 * Creates a new deque containing all the values of a given deque that are
 * considered true according to a user function.
 */
ds_deque_t *ds_deque_filter_callback(ds_deque_t *deque, FCI_PARAMS);

/**
 * Creates a new deque object using a slice of a given deque. Behaves exactly
 * like array_slice, but keys are not preserved.
 */
ds_deque_t *ds_deque_slice(ds_deque_t *deque, zend_long index, zend_long length);


/**
 *
 */
ds_deque_t *ds_deque_merge(ds_deque_t *deque, zval *values);

/**
 * Reduces all values of a deque to a single value, starting with an optional
 * initial value (IS_NULL if the pointer is NULL).
 */
void ds_deque_reduce(ds_deque_t *deque, zval *initial, zval *return_value, FCI_PARAMS);

/**
 * Rotates the values of a deque a given number of times (can be negative).
 */
void ds_deque_rotate(ds_deque_t *deque, zend_long rotations);

/**
 * Sorts a deque in place using a user function.
 */
void ds_deque_sort_callback(ds_deque_t *deque);

/**
 * Sorts a deque in place.
 */
void ds_deque_sort(ds_deque_t *deque);

/**
 * Reverses a deque in place.
 */
void ds_deque_reverse(ds_deque_t *deque);

/**
 * Creates a reversed copy of a deque.
 */
ds_deque_t *ds_deque_reversed(ds_deque_t *deque);

/**
 * Adds all values of a deque to a new array.
 */
void ds_deque_to_array(ds_deque_t *deque, zval *return_value);

#endif
