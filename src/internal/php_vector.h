#ifndef PHP_DS_VECTOR_H
#define PHP_DS_VECTOR_H

#include "../common.h"

/**
 * An internal zval vector that can be used by other data structures.
 */
typedef struct _Vector {
    zval       *buffer;
    zend_long   capacity;  // Buffer length
    zend_long   size;      // Number of values in the buffer
} Vector;

/**
 * Custom PHP object wrapped around an internal vector
 */
typedef struct _VectorObj {
    zend_object   std;
    Vector       *vector;
} VectorObj;

#define VECTOR_MIN_CAPACITY 10 // Does not have to be a power of 2

#define Z_VECTOR(z)   ((VectorObj*)(Z_OBJ(z)))->vector
#define Z_VECTOR_P(z) Z_VECTOR(*z)
#define THIS_VECTOR() Z_VECTOR_P(getThis())

#define ZVAL_VECTOR(z, v)  ZVAL_OBJ(z, vector_create_object_ex(v))
#define ZVAL_NEW_VECTOR(z) ZVAL_OBJ(z, vector_create_object_ex(vector_init()))

#define RETURN_VECTOR(vector) \
do { \
    ZVAL_VECTOR(return_value, vector); \
    return; \
} while(0)

#define VECTOR_SIZE(v)     ((v)->size)
#define VECTOR_IS_EMPTY(v) (VECTOR_SIZE(v) == 0)

#define VECTOR_FOREACH(vector, value) \
do {                                  \
    zval *_pos = vector->buffer;      \
    zval *_end = _pos + vector->size; \
    for (; _pos < _end; ++_pos) {     \
        value = _pos;

#define VECTOR_FOREACH_REVERSED(vector, value) \
do {                                           \
    zval *_end = vector->buffer;               \
    zval *_pos = _end + vector->size - 1;      \
    for (; _pos >= _end; --_pos) {             \
        value = _pos;

#define VECTOR_FOREACH_END() \
    } \
} while (0)

/**
 * Creates a new zend_object using an existing Vector.
 */
zend_object *vector_create_object_ex(Vector *vector);

/**
 * Creates a new Vector zend_object.
 */
zend_object *vector_create_object(zend_class_entry *ce);

/**
 * Creates an object clone of a Vector.
 */
zend_object *vector_create_clone(Vector *vector);

/**
 * Creates a copy of a Vector.
 */
Vector *vector_create_copy(Vector *src);

/**
 * Creates a new empty Vector.
 */
Vector *vector_init();

/**
 * Creates a new Vector with initial capacity.
 */
Vector *vector_init_ex(zend_long capacity);

/**
 * Creates a new Vector using a buffer of zval values of a given size.
 */
Vector *vector_from_buffer(zval *buffer, zend_long size);

/**
 * Removes all values from a Vector and reallocates its buffer to the minimum
 * capacity.
 */
void vector_clear(Vector *vector);

/**
 * Clears and frees a Vector and its buffer.
 */
void vector_destroy(Vector *vector);

/**
 * Ensures that a Vector's buffer has at least a specified amount of capacity.
 */
void vector_user_allocate(Vector *vector, zend_long capacity);

/**
 * Sets the value in a Vector by index.
 */
void vector_set(Vector *vector, zend_long index, zval *value);

/**
 * Copies the last value of a Vector into a return value then removes it.
 */
void vector_pop(Vector *vector, zval *return_value);

/**
 * Copies the first value of a Vector into a return value then removes it.
 */
void vector_shift(Vector *vector, zval *return_value);

/**
 * Attempts to find a value in a Vector's sequence (strict). The return value
 * will be IS_LONG representing the position of the value in the sequence,
 * or IS_FALSE if the value could not be found.
 */
void vector_find(Vector *vector, zval *value, zval *return_value);

/**
 * Removes a value from a Vector by index, causing all successive values to
 * shift one position to the left.
 */
void vector_remove(Vector *vector, zend_long index, zval *return_value);

/**
 * Inserts a single value into a position in a Vector,
 * where 0 >= position <= size.
 */
void vector_insert(Vector *vector, zend_long index, zval *value);

/**
 * Inserts zero or more values into a position in a Vector,
 * where 0 >= position <= size.
 */
void vector_insert_va(Vector *vector, zend_long index, VA_PARAMS);

/**
 * Unshifts (prepend) one value into a Vector.
 */
void vector_unshift(Vector *vector, zval *value);

/**
 * Unshifts (prepend) zero or more values into a Vector.
 */
void vector_unshift_va(Vector *vector, VA_PARAMS);

/**
 * Returns whether a value is in a Vector.
 */
bool vector_contains(Vector *vector, zval *value);

/**
 * Returns whether zero or more values are in a Vector.
 * The result will be false if no values are given.
 */
bool vector_contains_va(Vector *vector, VA_PARAMS);

/**
 * Pushes one value onto the end of a Vector's sequence.
 */
void vector_push(Vector *vector, zval *value);

/**
 * Pushes zero or more values onto the end of a Vector's sequence.
 */
void vector_push_va(Vector *vector, VA_PARAMS);

/**
 * Pushes all values from either a PHP array or other iterable onto a Vector.
 */
void vector_push_all(Vector *vector, zval *values);

/**
 * Returns a pointer to the value at a given inden in a Vector. NULL if invalid.
 */
zval *vector_get(Vector *vector, zend_long index);

/**
 * Returns a pointer to the last value of a Vector, or NULL if invalid.
 */
zval *vector_get_last(Vector *vector);

/**
 * Returns a pointer to the last value of a Vector, or NULL if invalid.
 */
zval *vector_get_first(Vector *vector);

/**
 * Creates a new Vector using the result of applying a user functions to each
 * value of a given Vector. The new Vector will be written to a given zval obj.
 */
void vector_map(Vector *vector, zval *obj, FCI_PARAMS);

/**
 * Reduces all values of a Vector to a single value, starting with an optional
 * initial value (IS_NULL if the pointer is NULL).
 */
void vector_reduce(Vector *vector, zval *initial, zval *return_value, FCI_PARAMS);

/**
 * Creates a new Vector object using a slice of a given Vector. Behaves exactly
 * like array_slice, but keys are not preserved.
 */
void vector_slice(Vector *vector, zend_long index, zend_long length, zval *obj);

/**
 * Reverses a Vector in place.
 */
void vector_reverse(Vector *vector);

/**
 * Creates a reversed copy of a Vector.
 */
Vector *vector_reversed(Vector *vector);

/**
 * Rotates the values of a Vector a given number of times (can be negative).
 */
void vector_rotate(Vector *vector, zend_long rotations);

/**
 * Joins each value of a Vector together using an optional 'glue'. The returned
 * zval* will be of type IS_STRING. If no glue is to be used, pass NULL and 0.
 * Similar to implode.
 */
void vector_join(Vector *vector, char *str, size_t len, zval *return_value);

/**
 * Sorts a Vector in place.
 */
void vector_sort(Vector *vector);

/**
 * Sorts a Vector in place using a user function.
 */
void vector_sort_callback(Vector *vector);

/**
 * Creates a new Vector containing all the values of a given Vector that are
 * considered true according to `zend_is_true`.
 */
void vector_filter(Vector *vector, zval *obj);

/**
 * Creates a new Vector containing all the values of a given Vector that are
 * considered true according to a user function.
 */
void vector_filter_callback(Vector *vector, zval *obj, FCI_PARAMS);

/**
 * Adds all values of a Vector to a new array.
 */
void vector_to_array(Vector *vector, zval *return_value);

/**
 * Returns true if an index is valid for a given Vector.
 */
bool vector_index_exists(Vector *vector, zend_long index);

/**
 * Returns true if the value at an index is set for a given Vector.
 * Used to determine the result of isset() and empty().
 */
bool vector_isset(Vector *vector, zend_long index, int check_empty);

/**
 * Serializses a Vector object.
 */
int vector_serialize(zval *object, unsigned char **buffer, size_t *length, zend_serialize_data *data);

/**
 * Unserializes a string as a Vector object.
 */
int vector_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t length, zend_unserialize_data *data);

#endif
