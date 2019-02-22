#ifndef HAVE_DS_VECTOR_H
#define HAVE_DS_VECTOR_H

#include <php.h>

/**
 * zval* -> ds_vector_t*
 */
#define Z_DS_VECTOR_P(z) Z_OBJ_P(z)

/**
 * zval* at the start of a vector's contiguous buffer.
 */
#define DS_VECTOR_INTERNAL(v) ((v)->properties_table)

/**
 * ds_vector_t* -> ds_buffer_t*
 */
#define DS_VECTOR_BUFFER(v) Z_OBJ_P(DS_VECTOR_INTERNAL(v))

/**
 * The number of items in a vector.
 */
#define DS_VECTOR_USED(v) DS_BUFFER_USED(DS_VECTOR_BUFFER(v))

/**
 * The capacity of a vector.
 */
#define DS_VECTOR_SIZE(v) DS_BUFFER_SIZE(DS_VECTOR_BUFFER(v))

/**
 * Replaces the existing vector buffer.
 */
#define DS_VECTOR_SET_BUFFER(v, b) ZVAL_OBJ(DS_VECTOR_INTERNAL(v), b)

/**
 * Minimum buffer capacity of an allocated vector.
 */
#define DS_VECTOR_MIN_ALLOC 8

/**
 * Vector object.
 */
typedef zend_object ds_vector_t;

/**
 * Creates a new, empty ds_vector_t.
 */
ds_vector_t *ds_vector();

/**
 * Sets the value of a given vector at a given offset.
 */
void ds_vector_set(ds_vector_t *obj, zend_long offset, zval *value);

/**
 * Returns the value of a given vector at a given offset.
 */
zval *ds_vector_get(ds_vector_t *obj, zend_long offset);

/**
 * Appends a value onto a given vector.
 */
void ds_vector_push(ds_vector_t *obj, zval *value);

/**
 * Appends one ore more values onto a given vector.
 */
void ds_vector_vpush(ds_vector_t *obj, zval *argv, zend_long argc);

/**
 * Register Vector class entry.
 */
void ds_register_vector();

#endif
