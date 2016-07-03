#ifndef DS_DEQUE_H
#define DS_DEQUE_H

#include "../common.h"

/**
 * An internal zval deque that can also be used by other data structures.
 */
typedef struct _ds_deque_t {
    zval      *buffer;
    zend_long  capacity;
    zend_long  head;
    zend_long  tail;
} ds_deque_t;

#define DEQUE_MIN_CAPACITY 8 // Must be a power of 2

#define DEQUE_SIZE(d) (((d)->tail - (d)->head) & ((d)->capacity - 1))
#define DEQUE_IS_EMPTY(d) ((d)->head == (d)->tail)

#define DEQUE_FOREACH(deque, value)                         \
do {                                                        \
    zval *_buffer   = (deque)->buffer;                      \
    zend_long _tail = (deque)->tail;                        \
    zend_long _mask = (deque)->capacity - 1;                \
    zend_long _head = (deque)->head;                        \
                                                            \
    for (; _head != _tail; _head = (_head + 1) & _mask) {   \
        value = _buffer + _head;


#define DEQUE_FOREACH_REVERSED(deque, value)                \
do {                                                        \
    zval *_buffer   = (deque)->buffer;                      \
    zend_long _head = (deque)->head;                        \
    zend_long _mask = (deque)->capacity - 1;                \
    zend_long _tail = ((deque)->tail - 1) & _mask;          \
                                                            \
    for (; _tail != _head; _tail = (_tail - 1) & _mask) {   \
        value = &_buffer[_tail];

#define DEQUE_FOREACH_END() \
    } \
} while (0)

/**
 * Creates a copy of a deque.
 */
ds_deque_t *deque_create_copy(ds_deque_t *src);

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
ds_deque_t *deque_from_buffer(zval *buffer, zend_long size);

/**
 * Removes all values from a deque and reallocates its buffer to the minimum
 * capacity.
 */
void deque_clear(ds_deque_t *deque);

/**
 * Clears and frees a deque and its buffer.
 */
void deque_destroy(ds_deque_t *deque);

/**
 * Ensures that a deque's buffer has at least a specified amount of capacity.
 */
void deque_user_allocate(ds_deque_t *deque, zend_long capacity);

/**
 * Sets the value in a deque by index.
 */
void deque_set(ds_deque_t *deque, zend_long index, zval *value);

/**
 * Copies the last value of a deque into a return value then removes it.
 */
void deque_pop(ds_deque_t *deque, zval *return_value);

/**
 * Copies the first value of a deque into a return value then removes it.
 */
void deque_shift(ds_deque_t *deque, zval *return_value);

/**
 * Attempts to find a value in a deque's sequence (strict). The return value
 * will be IS_LONG representing the position of the value in the sequence,
 * or IS_FALSE if the value could not be found.
 */
void deque_find(ds_deque_t *deque, zval *value, zval *return_value);

/**
 * Removes a value from a deque by index, causing all successive values to
 * shift one position to the left.
 */
void deque_remove(ds_deque_t *deque, zend_long index, zval *return_value);

/**
 * Inserts a single value into a position in a deque,
 * where 0 >= position <= size.
 */
// void deque_insert(deque *deque, zend_long index, zval *value);

/**
 * Inserts zero or more values into a position in a deque,
 * where 0 >= position <= size.
 */
void deque_insert_va(ds_deque_t *deque, zend_long index, VA_PARAMS);

/**
 * Unshifts (prepend) one value into a ds_deque_t.
 */
// void deque_unshift(ds_deque_t *deque, zval *value);

/**
 * Unshifts (prepend) zero or more values into a deque.
 */
void deque_unshift_va(ds_deque_t *deque, VA_PARAMS);

/**
 * Returns whether zero or more values are in a deque.
 * The result will be false if no values are given.
 */
bool deque_contains_va(ds_deque_t *deque, VA_PARAMS);

/**
 * Pushes one value onto the end of a deque's sequence.
 */
void deque_push(ds_deque_t *deque, zval *value);

/**
 * Pushes zero or more values onto the end of a deque's sequence.
 */
void deque_push_va(ds_deque_t *deque, VA_PARAMS);

/**
 * Pushes all values from either a PHP array or other iterable onto a deque.
 */
void deque_push_all(ds_deque_t *deque, zval *values);

/**
 * Returns a pointer to the value at a given inden in a deque. NULL if invalid.
 */
zval *deque_get(ds_deque_t *deque, zend_long index);

/**
 * Returns a pointer to the last value of a deque, or NULL if invalid.
 */
zval *deque_get_last(ds_deque_t *deque);

/**
 * Returns a pointer to the last value of a deque, or NULL if invalid.
 */
zval *deque_get_first(ds_deque_t *deque);

/**
 * Returns true if the value at an index is set for a given deque.
 * Used to determine the result of isset() and empty().
 */
bool deque_isset(ds_deque_t *deque, zend_long index, int check_empty);

/**
 * Joins each value of a deque together using an optional 'glue'. The returned
 * zval* will be of type IS_STRING. If no glue is to be used, pass NULL and 0.
 * Similar to implode.
 */
void deque_join(ds_deque_t *deque, char *str, size_t len, zval *return_value);

/**
 * Creates a new deque using the result of applying a user functions to each
 * value of a given deque.
 */
ds_deque_t *deque_map(ds_deque_t *deque, FCI_PARAMS);

/**
 * Creates a new deque containing all the values of a given deque that are
 * considered true according to `zend_is_true`.
 */
ds_deque_t *deque_filter(ds_deque_t *deque);

/**
 * Creates a new deque containing all the values of a given deque that are
 * considered true according to a user function.
 */
ds_deque_t *deque_filter_callback(ds_deque_t *deque, FCI_PARAMS);

/**
 * Creates a new deque object using a slice of a given deque. Behaves exactly
 * like array_slice, but keys are not preserved.
 */
ds_deque_t *deque_slice(ds_deque_t *deque, zend_long index, zend_long length);

/**
 * Reduces all values of a deque to a single value, starting with an optional
 * initial value (IS_NULL if the pointer is NULL).
 */
void deque_reduce(ds_deque_t *deque, zval *initial, zval *return_value, FCI_PARAMS);

/**
 * Rotates the values of a deque a given number of times (can be negative).
 */
void deque_rotate(ds_deque_t *deque, zend_long rotations);

/**
 * Sorts a deque in place using a user function.
 */
void deque_sort_callback(ds_deque_t *deque);

/**
 * Sorts a deque in place.
 */
void deque_sort(ds_deque_t *deque);

/**
 * Reverses a deque in place.
 */
void deque_reverse(ds_deque_t *deque);

/**
 * Creates a reversed copy of a deque.
 */
ds_deque_t *deque_reversed(ds_deque_t *deque);

/**
 * Adds all values of a deque to a new array.
 */
void deque_to_array(ds_deque_t *deque, zval *return_value);

#endif
