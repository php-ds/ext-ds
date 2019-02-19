#ifndef HAVE_DS_BUFFER_H
#define HAVE_DS_BUFFER_H

#include <php.h>

/**
 * zval* -> ds_buffer_t*
 */
#define DS_ZVAL_GET_BUFFER(z) ((ds_buffer_t *) (Z_OBJ_P(z)))

/**
 * Buffer object.
 */
typedef struct ds_buffer {
    zend_object std;
    zval *data;
    uint32_t len;
} ds_buffer_t;

/**
 * Buffer iterator.
 */
typedef struct ds_buffer_iterator {
    zend_object_iterator intern;
    uint32_t offset;
    uint32_t len;
    uint32_t pos;
} ds_buffer_iterator_t;

/**
 * Creates a new buffer using a given capacity.
 */
ds_buffer_t *ds_buffer(uint32_t capacity);

/**
 * Returns the value at a given offset.
 */
zval *ds_buffer_get(ds_buffer_t *buffer, uint32_t offset);

/**
 * Sets the value at a given offset.
 */
void ds_buffer_set(ds_buffer_t *buffer, uint32_t offset, zval *value);

/**
 * Create a copy of the given buffer.
 */
ds_buffer_t *ds_buffer_create_copy(ds_buffer_t *src);

/**
 * Reallocates the buffer to a given capacity.
 */
void ds_buffer_realloc(ds_buffer_t *obj, uint32_t capacity);

/**
 * Sets the array representation of a given buffer.
 */
void ds_buffer_to_array(zval *arr, ds_buffer_t *obj, uint32_t len);

/**
 * Creates a new buffer iterator.
 */
zend_object_iterator *ds_buffer_iterator(ds_buffer_t *buf, uint32_t start, uint32_t len);

/**
 * Registers buffer class entry.
 */
void ds_register_buffer();

#endif
