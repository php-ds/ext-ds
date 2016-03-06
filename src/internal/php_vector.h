#ifndef PHP_DS_VECTOR_H
#define PHP_DS_VECTOR_H

#include "../common.h"

/**
 * An internal zval vector that can be used by other data structures.
 */
typedef struct _ds_vector_t {
    zval       *buffer;
    zend_long   capacity;  // Buffer length
    zend_long   size;      // Number of values in the buffer
} ds_vector_t;

/**
 * Custom PHP object wrapped around an internal vector
 */
typedef struct _php_ds_vector_t {
    zend_object    std;
    ds_vector_t   *vector;
} php_ds_vector_t;

#define DS_VECTOR_MIN_CAPACITY 10 // Does not have to be a power of 2

#define Z_DS_VECTOR(z)   ((php_ds_vector_t*)(Z_OBJ(z)))->vector
#define Z_DS_VECTOR_P(z) Z_DS_VECTOR(*z)
#define THIS_DS_VECTOR() Z_DS_VECTOR_P(getThis())

#define ZVAL_DS_VECTOR(z, v)  ZVAL_OBJ(z, php_ds_vector_ex(v))
#define ZVAL_NEW_VECTOR(z) ZVAL_OBJ(z, php_ds_vector_ex(ds_vector()))

#define RETURN_DS_VECTOR(vector) \
do { \
    ZVAL_DS_VECTOR(return_value, vector); \
    return; \
} while(0)

#define DS_VECTOR_SIZE(v)     ((v)->size)
#define DS_VECTOR_IS_EMPTY(v) (DS_VECTOR_SIZE(v) == 0)

#define DS_VECTOR_FOREACH(vector, value) \
do {                                  \
    zval *_pos = vector->buffer;      \
    zval *_end = _pos + vector->size; \
    for (; _pos < _end; ++_pos) {     \
        value = _pos;

#define DS_VECTOR_FOREACH_REVERSED(vector, value) \
do {                                           \
    zval *_end = vector->buffer;               \
    zval *_pos = _end + vector->size - 1;      \
    for (; _pos >= _end; --_pos) {             \
        value = _pos;

#define DS_VECTOR_FOREACH_END() \
    } \
} while (0)

/**
 * Creates a new zend_object using an existing ds_vector_t.
 */
zend_object *php_ds_vector_ex(ds_vector_t *vector);

/**
 * Creates a new ds_vector_t zend_object.
 */
zend_object *php_ds_vector(zend_class_entry *ce);

/**
 * Creates an object clone of a ds_vector_t.
 */
zend_object *ds_vector_create_clone(ds_vector_t *vector);

/**
 * Creates a copy of a ds_vector_t.
 */
ds_vector_t *ds_vector_create_copy(ds_vector_t *src);

/**
 * Creates a new empty ds_vector_t.
 */
ds_vector_t *ds_vector();

/**
 * Creates a new ds_vector_t with initial capacity.
 */
ds_vector_t *ds_vector_ex(zend_long capacity);

/**
 * Creates a new ds_vector_t using a buffer of zval values of a given size.
 */
ds_vector_t *ds_vector_from_buffer(zval *buffer, zend_long size);

/**
 * Removes all values from a ds_vector_t and reallocates its buffer to the minimum
 * capacity.
 */
void ds_vector_clear(ds_vector_t *vector);

/**
 * Clears and frees a ds_vector_t and its buffer.
 */
void ds_vector_destroy(ds_vector_t *vector);

/**
 * Ensures that a ds_vector_t's buffer has at least a specified amount of capacity.
 */
void ds_vector_user_allocate(ds_vector_t *vector, zend_long capacity);

/**
 * Sets the value in a ds_vector_t by index.
 */
void ds_vector_set(ds_vector_t *vector, zend_long index, zval *value);

/**
 * Copies the last value of a ds_vector_t into a return value then removes it.
 */
void ds_vector_pop(ds_vector_t *vector, zval *return_value);

/**
 * Copies the first value of a ds_vector_t into a return value then removes it.
 */
void ds_vector_shift(ds_vector_t *vector, zval *return_value);

/**
 * Attempts to find a value in a ds_vector_t's sequence (strict). The return value
 * will be IS_LONG representing the position of the value in the sequence,
 * or IS_FALSE if the value could not be found.
 */
void ds_vector_find(ds_vector_t *vector, zval *value, zval *return_value);

/**
 * Removes a value from a ds_vector_t by index, causing all successive values to
 * shift one position to the left.
 */
void ds_vector_remove(ds_vector_t *vector, zend_long index, zval *return_value);

/**
 * Inserts a single value into a position in a ds_vector_t,
 * where 0 >= position <= size.
 */
void ds_vector_insert(ds_vector_t *vector, zend_long index, zval *value);

/**
 * Inserts zero or more values into a position in a ds_vector_t,
 * where 0 >= position <= size.
 */
void ds_vector_insert_va(ds_vector_t *vector, zend_long index, VA_PARAMS);

/**
 * Unshifts (prepend) one value into a ds_vector_t.
 */
void ds_vector_unshift(ds_vector_t *vector, zval *value);

/**
 * Unshifts (prepend) zero or more values into a ds_vector_t.
 */
void ds_vector_unshift_va(ds_vector_t *vector, VA_PARAMS);

/**
 * Returns whether a value is in a ds_vector_t.
 */
bool ds_vector_contains(ds_vector_t *vector, zval *value);

/**
 * Returns whether zero or more values are in a ds_vector_t.
 * The result will be false if no values are given.
 */
bool ds_vector_contains_va(ds_vector_t *vector, VA_PARAMS);

/**
 * Pushes one value onto the end of a ds_vector_t's sequence.
 */
void ds_vector_push(ds_vector_t *vector, zval *value);

/**
 * Pushes zero or more values onto the end of a ds_vector_t's sequence.
 */
void ds_vector_push_va(ds_vector_t *vector, VA_PARAMS);

/**
 * Pushes all values from either a PHP array or other iterable onto a ds_vector_t.
 */
void ds_vector_push_all(ds_vector_t *vector, zval *values);

/**
 * Returns a pointer to the value at a given inden in a ds_vector_t. NULL if invalid.
 */
zval *ds_vector_get(ds_vector_t *vector, zend_long index);

/**
 * Returns a pointer to the last value of a ds_vector_t, or NULL if invalid.
 */
zval *ds_vector_get_last(ds_vector_t *vector);

/**
 * Returns a pointer to the last value of a ds_vector_t, or NULL if invalid.
 */
zval *ds_vector_get_first(ds_vector_t *vector);

/**
 * Creates a new ds_vector_t using the result of applying a user functions to each
 * value of a given ds_vector_t. The new ds_vector_t will be written to a given zval obj.
 */
void ds_vector_map(ds_vector_t *vector, zval *obj, FCI_PARAMS);

/**
 *
 */
void ds_vector_merge(ds_vector_t *vector, zval *values, zval *obj);

/**
 * Reduces all values of a ds_vector_t to a single value, starting with an optional
 * initial value (IS_NULL if the pointer is NULL).
 */
void ds_vector_reduce(ds_vector_t *vector, zval *initial, zval *return_value, FCI_PARAMS);

/**
 * Creates a new ds_vector_t object using a slice of a given ds_vector_t. Behaves exactly
 * like array_slice, but keys are not preserved.
 */
void ds_vector_slice(ds_vector_t *vector, zend_long index, zend_long length, zval *obj);

/**
 * Reverses a ds_vector_t in place.
 */
void ds_vector_reverse(ds_vector_t *vector);

/**
 * Creates a reversed copy of a ds_vector_t.
 */
ds_vector_t *ds_vector_reversed(ds_vector_t *vector);

/**
 * Rotates the values of a ds_vector_t a given number of times (can be negative).
 */
void ds_vector_rotate(ds_vector_t *vector, zend_long rotations);

/**
 * Joins each value of a ds_vector_t together using an optional 'glue'. The returned
 * zval* will be of type IS_STRING. If no glue is to be used, pass NULL and 0.
 * Similar to implode.
 */
void ds_vector_join(ds_vector_t *vector, char *str, size_t len, zval *return_value);

/**
 * Sorts a ds_vector_t in place.
 */
void ds_vector_sort(ds_vector_t *vector);

/**
 * Sorts a ds_vector_t in place using a user function.
 */
void ds_vector_sort_callback(ds_vector_t *vector);

/**
 * Creates a new ds_vector_t containing all the values of a given ds_vector_t that are
 * considered true according to `zend_is_true`.
 */
void ds_vector_filter(ds_vector_t *vector, zval *obj);

/**
 * Creates a new ds_vector_t containing all the values of a given ds_vector_t that are
 * considered true according to a user function.
 */
void ds_vector_filter_callback(ds_vector_t *vector, zval *obj, FCI_PARAMS);

/**
 * Adds all values of a ds_vector_t to a new array.
 */
void ds_vector_to_array(ds_vector_t *vector, zval *return_value);

/**
 * Returns true if an index is valid for a given ds_vector_t.
 */
bool ds_vector_index_exists(ds_vector_t *vector, zend_long index);

/**
 * Returns true if the value at an index is set for a given ds_vector_t.
 * Used to determine the result of isset() and empty().
 */
bool ds_vector_isset(ds_vector_t *vector, zend_long index, int check_empty);

/**
 * Serializses a ds_vector_t object.
 */
int ds_vector_serialize(zval *object, unsigned char **buffer, size_t *length, zend_serialize_data *data);

/**
 * Unserializes a string as a ds_vector_t object.
 */
int ds_vector_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t length, zend_unserialize_data *data);

#endif
