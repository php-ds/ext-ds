#include <php.h>
#include "vector.h"
#include "buffer.h"
#include "Zend/zend_types.h"

zend_class_entry *ds_vector_ce;

static zend_object_handlers ds_vector_handlers;

/**
 * Creates a new vector with a given capacity.
 */
static ds_vector_t *ds_vector_with_capacity(zend_long capacity)
{
    ds_vector_t *vector = ecalloc(1, sizeof(ds_vector_t));

    vector->handlers = &ds_vector_handlers;

    zend_object_std_init(vector, ds_vector_ce);

    DS_VECTOR_SET_BUFFER(vector, ds_buffer(capacity));
    DS_VECTOR_SIZE(vector) = 0;

    return vector;
}

/**
 * Create a new, empty vector.
 */
ds_vector_t *ds_vector()
{
    return ds_vector_with_capacity(0);
}

/**
 * Constructor.
 */
static ds_vector_t *ds_vector_create_object(zend_class_entry *ce)
{
    return ds_vector();
}

/**
 * Destructor.
 */
static void ds_vector_free_object(zend_object *obj)
{
    zval_ptr_dtor(DS_VECTOR_BUFFER_ZVAL((ds_vector_t *) obj));
}

/**
 * Copy-on-write.
 *
 * Determines if the given vector's buffer is shared, creating a copy if it is.
 */
static void ds_vector_separate(ds_vector_t *vector)
{
    ds_buffer_t *buffer = DS_VECTOR_BUFFER(vector);

    if (GC_REFCOUNT(buffer) == 1) {
        return;
    }

    GC_DELREF(buffer);
    DS_VECTOR_SET_BUFFER(vector, ds_buffer_create_copy(buffer));
}

/**
 * Returns the value at a given offset.
 */
zval *ds_vector_get(ds_vector_t *vector, zend_long offset)
{
    return ds_buffer_get(DS_VECTOR_BUFFER(vector), offset);
}

/**
 * Sets the value at the given offset to a given value.
 */
void ds_vector_set(ds_vector_t *vector, zend_long offset, zval *value)
{
    ds_vector_separate(vector);
    ds_buffer_set(DS_VECTOR_BUFFER(vector), offset, value);
}

/**
 * Increase capacity of the given vector's buffer.
 */
static void ds_vector_grow(ds_vector_t *vector)
{
    /* Capacity is double existing size. */
    zend_long capacity = MAX(DS_VECTOR_MIN_ALLOC, DS_VECTOR_SIZE(vector) << 1);

    /* Re-allocate the existing buffer. */
    ds_buffer_t *buffer = ds_buffer_realloc(DS_VECTOR_BUFFER(vector), capacity);

    /* Replace the vector's buffer. */
    DS_VECTOR_SET_BUFFER(vector, buffer);
}

/**
 * Append a value to the given vector.
 */
void ds_vector_push(ds_vector_t *vector, zval *value)
{
    ds_vector_separate(vector);

    /* Check if we need to grow.. */
    if (DS_VECTOR_SIZE(vector) == DS_BUFFER_SIZE(DS_VECTOR_BUFFER(vector))) {
        ds_vector_grow(vector);
    }

    ds_buffer_set(DS_VECTOR_BUFFER(vector), DS_VECTOR_SIZE(vector)++, value);
}

/**
 * Debug info handler (var_dump).
 */
static HashTable *ds_vector_get_debug_info(zval *obj, int *is_temp)
{
    zval arr;
    ds_vector_t *vector = DS_VECTOR_FROM_ZVAL(obj);

    *is_temp = 1;

    ds_buffer_to_array(&arr, DS_VECTOR_BUFFER(vector), DS_VECTOR_SIZE(vector));

    return Z_ARRVAL(arr);
}

/**
 * Array access read handler.
 */
static zval *ds_vector_read_dimension(zval *obj, zval *offset, int type, zval *rv)
{
    return ds_vector_get(DS_VECTOR_FROM_ZVAL(obj), Z_LVAL_P(offset));
}

/**
 * Array access write handler.
 */
static void ds_vector_write_dimension(zval *obj, zval *offset, zval *value)
{
    ds_vector_t *vector = DS_VECTOR_FROM_ZVAL(obj);

    /* $v[] = ... */
    if (offset == NULL) {
        ds_vector_push(vector, value);
    } else {
        ds_vector_set(vector, Z_LVAL_P(offset), value);
    }
}

/**
 * GC handler.
 */
static HashTable *ds_vector_get_gc(zval *obj, zval **gc_data, int *gc_count)
{
    *gc_data  = DS_VECTOR_BUFFER_ZVAL(DS_VECTOR_FROM_ZVAL(obj));
    *gc_count = 1;

    return NULL;
}

/**
 * Count elements handler.
 */
static int ds_vector_count_elements(zval *obj, zend_long *count)
{
    *count = DS_VECTOR_SIZE(DS_VECTOR_FROM_ZVAL(obj));

    return SUCCESS;
}

/**
 * Create an iterator of this vector's buffer.
 */
zend_object_iterator *ds_vector_get_iterator(zend_class_entry *ce, zval *obj, int by_ref)
{
    ds_vector_t *vector = DS_VECTOR_FROM_ZVAL(obj);
    ds_buffer_t *buffer = DS_VECTOR_BUFFER(vector);

    return ds_buffer_iterator(buffer, 0, DS_VECTOR_SIZE(vector));
}

/**
 * Register vector class entry.
 */
void ds_register_vector()
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Vector", NULL);
    ds_vector_ce = zend_register_internal_class(&ce);

    /* */
    ds_vector_ce->ce_flags     |= ZEND_ACC_FINAL;
    ds_vector_ce->create_object = ds_vector_create_object;
    ds_vector_ce->get_iterator  = ds_vector_get_iterator;

    memcpy(&ds_vector_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    ds_vector_handlers.get_gc          = ds_vector_get_gc;
    ds_vector_handlers.free_obj        = ds_vector_free_object;
    ds_vector_handlers.get_debug_info  = ds_vector_get_debug_info;
    ds_vector_handlers.read_dimension  = ds_vector_read_dimension;
    ds_vector_handlers.write_dimension = ds_vector_write_dimension;
    ds_vector_handlers.count_elements  = ds_vector_count_elements;
}
