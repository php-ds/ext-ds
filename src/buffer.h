#ifndef HAVE_DS_BUFFER_H
#define HAVE_DS_BUFFER_H

#include <php.h>

/**
 * zval* -> ds_buffer_t*
 */
#define DS_ZVAL_GET_BUFFER(z) ((ds_buffer_t *) (Z_OBJ_P(z)))

/**
 * Allocation length for the buffer struct and data.
 */
#define DS_BUFFER_ALLOC_SIZE(capacity) \
    (sizeof(ds_buffer_t) + sizeof(zval) * (capacity - 1))

/**
 * Buffer object.
 */
typedef struct ds_buffer {
    zend_object std;
    zend_long len;
    zval data[1];
} ds_buffer_t;

/**
 * Buffer iterator.
 */
typedef struct ds_buffer_iterator {
    zend_object_iterator intern;
    zend_long offset;
    zend_long len;
    zend_long pos;
} ds_buffer_iterator_t;

/**
 * Creates a new buffer using a given capacity.
 */
ds_buffer_t *ds_buffer(zend_long capacity);

/**
 * Returns the value at a given offset.
 */
zval *ds_buffer_get(ds_buffer_t *buffer, zend_long offset);

/**
 * Sets the value at a given offset.
 */
void ds_buffer_set(ds_buffer_t *buffer, zend_long offset, zval *value);

/**
 * Create a copy of the given buffer.
 */
ds_buffer_t *ds_buffer_create_copy(ds_buffer_t *src);

/**
 * Reallocates the buffer to a given capacity.
 */
ds_buffer_t *ds_buffer_realloc(ds_buffer_t *buffer, zend_long capacity);

/**
 * Sets the array representation of a given buffer.
 */
void ds_buffer_to_array(zval *arr, ds_buffer_t *obj, zend_long len);

/**
 * Creates a new buffer iterator.
 */
zend_object_iterator *ds_buffer_iterator(ds_buffer_t *buf, zend_long start, zend_long len);

/**
 * Registers buffer class entry.
 */
void ds_register_buffer();

#endif
