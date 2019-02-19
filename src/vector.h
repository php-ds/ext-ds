#ifndef HAVE_DS_VECTOR_H
#define HAVE_DS_VECTOR_H

#include <php.h>

/**
 * zval* -> ds_vector_t*
 */
#define DS_ZVAL_GET_VECTOR(z) ((ds_vector_t *) (Z_OBJ_P(z)))

/**
 * ds_vector_t* -> ds_buffer_t*
 */
#define DS_VECTOR_BUFFER(v) ((ds_buffer_t *) (Z_OBJ(v->data)))

/**
 * Replaces the existing vector buffer without freeing existing data.
 */
#define DS_VECTOR_SET_BUFFER(v, b) ZVAL_OBJ(&v->data, (zend_object *) b)

/**
 * Vector object.
 */
typedef struct ds_vector {
    zend_object std;
    zval data;      // Copy-on-write buffer
    uint32_t size;  // Number of items in the vector
} ds_vector_t;

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
 * Register Vector class entry.
 */
void ds_register_vector();

#endif
