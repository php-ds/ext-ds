#ifndef PHP_DS_DEQUE_H
#define PHP_DS_DEQUE_H

#include "../common.h"

/**
 * An internal zval deque that can also be used by other data structures.
 */
typedef struct _Deque {
    zval      *buffer;
    zend_long  capacity;
    zend_long  head;
    zend_long  tail;
} Deque;

/**
 * Custom PHP object wrapped around an internal deque
 */
typedef struct _DequeObj {
    zend_object  std;
    Deque       *deque;
} DequeObj;

#define DEQUE_MIN_CAPACITY 8 // Must be a power of 2

#define Z_DEQUE(z)   ((DequeObj*) Z_OBJ(z))->deque
#define Z_DEQUE_P(z) Z_DEQUE(*z)
#define THIS_DEQUE() Z_DEQUE_P(getThis())

#define ZVAL_DEQUE(z, d)  ZVAL_OBJ(z, deque_create_object_ex(d))
#define ZVAL_NEW_DEQUE(z) ZVAL_OBJ(z, deque_create_object_ex(deque_init()))

#define RETURN_DEQUE(deque) \
do { \
    ZVAL_DEQUE(return_value, deque); \
    return; \
} while(0)

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
 * Creates a new zend_object using an existing Deque.
 */
zend_object *deque_create_object_ex(Deque *deque);

/**
 * Creates a new Deque zend_object.
 */
zend_object *deque_create_object(zend_class_entry *ce);

/**
 * Creates an object clone of a Deque.
 */
zend_object *deque_create_clone(Deque *deque);

/**
 * Creates a copy of a Deque.
 */
Deque *deque_create_copy(Deque *src);

/**
 * Creates a new empty Deque.
 */
Deque *deque_init();

/**
 * Creates a new Deque with initial capacity.
 */
Deque *deque_init_ex(zend_long capacity);

/**
 * Creates a new Deque using a buffer of zval values of a given size.
 */
Deque *deque_from_buffer(zval *buffer, zend_long size);

/**
 * Removes all values from a Deque and reallocates its buffer to the minimum
 * capacity.
 */
void deque_clear(Deque *deque);

/**
 * Clears and frees a Deque and its buffer.
 */
void deque_destroy(Deque *deque);

/**
 * Ensures that a Deque's buffer has at least a specified amount of capacity.
 */
void deque_user_allocate(Deque *deque, zend_long capacity);

/**
 * Sets the value in a Deque by index.
 */
void deque_set(Deque *deque, zend_long index, zval *value);

/**
 * Copies the last value of a Deque into a return value then removes it.
 */
void deque_pop(Deque *deque, zval *return_value);

/**
 * Copies the first value of a Deque into a return value then removes it.
 */
void deque_shift(Deque *deque, zval *return_value);

/**
 * Attempts to find a value in a Deque's sequence (strict). The return value
 * will be IS_LONG representing the position of the value in the sequence,
 * or IS_FALSE if the value could not be found.
 */
void deque_find(Deque *deque, zval *value, zval *return_value);

/**
 * Removes a value from a Deque by index, causing all successive values to
 * shift one position to the left.
 */
void deque_remove(Deque *deque, zend_long index, zval *return_value);

/**
 * Inserts a single value into a position in a Deque,
 * where 0 >= position <= size.
 */
// void deque_insert(Deque *deque, zend_long index, zval *value);

/**
 * Inserts zero or more values into a position in a Deque,
 * where 0 >= position <= size.
 */
void deque_insert_va(Deque *deque, zend_long index, VA_PARAMS);

/**
 * Unshifts (prepend) one value into a Deque.
 */
// void deque_unshift(Deque *deque, zval *value);

/**
 * Unshifts (prepend) zero or more values into a Deque.
 */
void deque_unshift_va(Deque *deque, VA_PARAMS);

/**
 * Returns whether zero or more values are in a Deque.
 * The result will be false if no values are given.
 */
bool deque_contains_va(Deque *deque, VA_PARAMS);

/**
 * Pushes one value onto the end of a Deque's sequence.
 */
void deque_push(Deque *deque, zval *value);

/**
 * Pushes zero or more values onto the end of a Deque's sequence.
 */
void deque_push_va(Deque *deque, VA_PARAMS);

/**
 * Pushes all values from either a PHP array or other iterable onto a Deque.
 */
void deque_push_all(Deque *deque, zval *values);

/**
 * Returns a pointer to the value at a given inden in a Deque. NULL if invalid.
 */
zval *deque_get(Deque *deque, zend_long index);

/**
 * Returns a pointer to the last value of a Deque, or NULL if invalid.
 */
zval *deque_get_last(Deque *deque);

/**
 * Returns a pointer to the last value of a Deque, or NULL if invalid.
 */
zval *deque_get_first(Deque *deque);

/**
 * Returns true if the value at an index is set for a given Deque.
 * Used to determine the result of isset() and empty().
 */
bool deque_isset(Deque *deque, zend_long index, int check_empty);

/**
 * Joins each value of a Deque together using an optional 'glue'. The returned
 * zval* will be of type IS_STRING. If no glue is to be used, pass NULL and 0.
 * Similar to implode.
 */
void deque_join(Deque *deque, char *str, size_t len, zval *return_value);

/**
 * Creates a new Deque using the result of applying a user functions to each
 * value of a given Deque. The new Deque will be written to a given zval obj.
 */
void deque_map(Deque *deque, zval *obj, FCI_PARAMS);

/**
 *
 */
void deque_merge(Deque *deque, zval *values, zval *obj);

/**
 * Creates a new Deque containing all the values of a given Deque that are
 * considered true according to `zend_is_true`.
 */
void deque_filter(Deque *deque, zval *obj);

/**
 * Creates a new Deque containing all the values of a given Deque that are
 * considered true according to a user function.
 */
void deque_filter_callback(Deque *deque, zval *obj, FCI_PARAMS);

/**
 * Reduces all values of a Deque to a single value, starting with an optional
 * initial value (IS_NULL if the pointer is NULL).
 */
void deque_reduce(Deque *deque, zval *initial, zval *return_value, FCI_PARAMS);

/**
 * Creates a new Deque object using a slice of a given Deque. Behaves exactly
 * like array_slice, but keys are not preserved.
 */
void deque_slice(Deque *deque, zend_long index, zend_long length, zval *obj);

/**
 * Rotates the values of a Deque a given number of times (can be negative).
 */
void deque_rotate(Deque *deque, zend_long rotations);

/**
 * Sorts a Deque in place using a user function.
 */
void deque_sort_callback(Deque *deque);

/**
 * Sorts a Deque in place.
 */
void deque_sort(Deque *deque);

/**
 * Reverses a Deque in place.
 */
void deque_reverse(Deque *deque);

/**
 * Creates a reversed copy of a Deque.
 */
Deque *deque_reversed(Deque *deque);

/**
 * Adds all values of a Deque to a new array.
 */
void deque_to_array(Deque *deque, zval *return_value);

/**
 * Serializses a Deque object.
 */
int deque_serialize(zval *object, unsigned char **buffer, size_t *length, zend_serialize_data *data);

/**
 * Unserializes a string as a Deque object.
 */
int deque_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t length, zend_unserialize_data *data);

#endif
