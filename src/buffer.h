#ifndef HAVE_DS_BUFFER_H
#define HAVE_DS_BUFFER_H

#include <php.h>

/**
 * Buffer Data Layout
 * =====================
 *
 * ds_buffer_t is a zend_object. We use the existing struct hack (which is used
 * for object properties) to store the buffer data. We allocate the buffer data
 * directly after the zend_object, preserving the one zval that is allocated as
 * part of the struct. This zval can be accessed as DS_BUFFER_INFO.
 *
 * We use DS_BUFFER_INFO to store 2 values:
 *   - The allocated length of the buffer.        => DS_BUFFER_SIZE
 *   - The number of used slots from the front.   => DS_BUFFER_USED
 *
 * The buffer data can be accessed using DS_BUFFER_DATA.
 */

/**
 * zval* -> ds_buffer_t*
 */
#define Z_DS_BUFFER_P(z) Z_OBJ_P(z)

/**
 * Capacity and used slots information.
 */
#define DS_BUFFER_INFO(b) ((b)->properties_table)

/**
 * zval* at the start of a buffer's contiguous zval array.
 */
#define DS_BUFFER_DATA(b) ((b)->properties_table + 1)

/**
 * The number of zval's that we have allocated memory for in this buffer.
 */
#define DS_BUFFER_SIZE(b) Z_NEXT_P(DS_BUFFER_INFO(b))

/**
 * The number of slots used (from the front of the buffer).
 */
#define DS_BUFFER_USED(b) Z_LVAL_P(DS_BUFFER_INFO(b))

/**
 * Determines whether a buffer has used all of its allocated slots.
 */
#define DS_BUFFER_IS_FULL(b) (DS_BUFFER_USED(b) == DS_BUFFER_SIZE(b))

/**
 * Maximum capacity of a buffer.
 *
 * TODO: We should calculate this in such a way that we can check if a capacity
 *       exceeds the maximum without having to check for an overflow. We do not
 *       yet know the growth factors and minimum capacities of all structures
 *       that might use a buffer, so we should come back to this later on.
 *
 *       Alternatively, would it be enough to check whether the target capacity
 *       is less than the current capacity? With a maximum growth factor of 2,
 *       uint32_t would overflow and continue to not greater than the current
 *       capacity. There could therefore be no need for a hard maximum capacity?
 */
#define DS_BUFFER_MAX_CAPACITY ((UINT32_MAX >> 1) - 1)

/**
 * Allocation length for the object including data for a given capacity.
 */
#define DS_BUFFER_GET_ALLOC_SIZE(capacity) \
    (sizeof(zend_object) + sizeof(zval) * capacity)

/**
 * Perform a task for each value in the given buffer.
 */
#define DS_BUFFER_FOREACH(buffer, var, task) do { \
    ds_buffer_t *_buf = buffer; \
    zval        *_pos = DS_BUFFER_DATA(_buf); \
    zval        *_end = DS_BUFFER_DATA(_buf) + DS_BUFFER_USED(_buf); \
    \
    for (; _pos < _end; ++_pos) { \
        var = _pos; task; \
    } \
} while (0)

/**
 * Buffer object.
 */
typedef zend_object ds_buffer_t;

/**
 * Buffer iterator.
 */
typedef struct ds_buffer_iterator {
    zend_object_iterator intern;
    zend_long offset;               // Buffer offset
    zend_long len;                  // Length of the traversal
    zend_long pos;                  // Position of the current iteration
} ds_buffer_iterator_t;

/**
 * Creates a new buffer using a given capacity.
 */
ds_buffer_t *ds_buffer(zend_long capacity);

/**
 * Returns the value at a given offset.
 */
zval *ds_buffer_get(ds_buffer_t *obj, zend_long offset);

/**
 * Sets the value at a given offset.
 */
void ds_buffer_set(ds_buffer_t *obj, zend_long offset, zval *value);

/**
 * Create a copy of the given buffer.
 */
ds_buffer_t *ds_buffer_create_copy(ds_buffer_t *src);

/**
 * Reallocates the buffer to a given capacity.
 */
ds_buffer_t *ds_buffer_realloc(ds_buffer_t *obj, zend_long capacity);

/**
 * Sets the array representation of a given buffer.
 */
void ds_buffer_to_array(zval *arr, ds_buffer_t *obj, zend_long len);

/**
 * Creates a new buffer iterator.
 */
zend_object_iterator *ds_buffer_iterator(ds_buffer_t *obj, zend_long offset, zend_long len);

/**
 * Registers buffer class entry.
 */
void ds_register_buffer();

#endif
