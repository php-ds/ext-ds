#include <stdbool.h>
#include <php.h>
#include "vector.h"
#include "buffer.h"
#include "Zend/zend_types.h"

zend_class_entry *ds_vector_ce;

static zend_object_handlers ds_vector_handlers;

/**
 * Create a new, empty vector.
 */
ds_vector_t *ds_vector()
{
    ds_vector_t *obj = ecalloc(1, sizeof(ds_vector_t));
    obj->handlers = &ds_vector_handlers;

    zend_object_std_init(obj, ds_vector_ce);
    DS_VECTOR_SET_BUFFER(obj, ds_buffer(DS_VECTOR_MIN_ALLOC));

    return obj;
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
    zval_ptr_dtor(DS_VECTOR_INTERNAL((ds_vector_t *) obj));
}

/**
 * Copy-on-write.
 *
 * Determines if the given vector's buffer is shared, creating a copy if it is.
 */
static void ds_vector_separate(ds_vector_t *obj)
{
    ds_buffer_t *buffer = DS_VECTOR_BUFFER(obj);

    if (GC_REFCOUNT(buffer) == 1) {
        return;
    }

    GC_DELREF(buffer);
    DS_VECTOR_SET_BUFFER(obj, ds_buffer_create_copy(buffer));
}

/**
 * Increases the capacity of a given vector by a constant factor (if full).
 */
static void ds_vector_grow_if_full(ds_vector_t *obj)
{
    const ds_buffer_t *buffer = DS_VECTOR_BUFFER(obj);

    if (DS_BUFFER_IS_FULL(buffer)) {
        ds_buffer_t *current = DS_VECTOR_BUFFER(obj);
        ds_buffer_t *realloc = ds_buffer_realloc(current, DS_BUFFER_SIZE(current) << 1);

        DS_VECTOR_SET_BUFFER(obj, realloc);
    }
}

/**
 * Increases the capacity of the given vector's buffer, either to the next order
 * of capacity (ie. growth factor) or to accommodate a given count of new values.
 */
static void ds_vector_ensure_capacity_for(ds_vector_t *obj, zend_long count)
{
    ds_buffer_t *buffer = DS_VECTOR_BUFFER(obj);

    const zend_long used = DS_BUFFER_USED(buffer);
    const zend_long size = DS_BUFFER_SIZE(buffer);

    /* Check if we already have enough room for the incoming values. */
    if ((size - used) >= count) {
        return;
    }

    /* Avoid repeated allocations by using the next threshold if greater. */
    buffer = ds_buffer_realloc(buffer, MAX(size << 1, used + count));

    DS_VECTOR_SET_BUFFER(obj, buffer);
}

/**
 * Returns the value at a given offset.
 */
zval *ds_vector_get(ds_vector_t *obj, zend_long offset)
{
    return ds_buffer_get(DS_VECTOR_BUFFER(obj), offset);
}

/**
 * Sets the value at the given offset to a given value.
 */
void ds_vector_set(ds_vector_t *obj, zend_long offset, zval *value)
{
    ds_vector_separate(obj);
    ds_buffer_set(DS_VECTOR_BUFFER(obj), offset, value);
}

/**
 * Append a value to the given vector.
 */
void ds_vector_push(ds_vector_t *obj, zval *value)
{
    ds_vector_separate(obj);
    ds_vector_grow_if_full(obj);

    ds_buffer_set(DS_VECTOR_BUFFER(obj), DS_VECTOR_USED(obj), value);
    DS_VECTOR_USED(obj)++;
}

/**
 * Appends one or more values to the given vector.
 */
void ds_vector_vpush(ds_vector_t *obj, zval *argv, zend_long argc)
{
    ds_vector_separate(obj);
    ds_vector_ensure_capacity_for(obj, argc);

    do {
        ds_buffer_set(DS_VECTOR_BUFFER(obj), DS_VECTOR_USED(obj)++, argv++);
    } while (--argc);
}

/**
 * Debug info handler (var_dump).
 */
static HashTable *ds_vector_get_debug_info(zval *object, int *is_temp)
{
    zval arr;
    ds_vector_t *obj = Z_DS_VECTOR_P(object);

    ds_buffer_to_array(&arr, DS_VECTOR_BUFFER(obj), DS_VECTOR_USED(obj));

    *is_temp = 1;

    return Z_ARRVAL(arr);
}

/**
 * Array access read handler.
 */
static zval *ds_vector_read_dimension(zval *obj, zval *offset, int type, zval *rv)
{
    return ds_vector_get(Z_DS_VECTOR_P(obj), Z_LVAL_P(offset));
}

/**
 * Array access write handler.
 */
static void ds_vector_write_dimension(zval *obj, zval *offset, zval *value)
{
    ds_vector_t *vector = Z_DS_VECTOR_P(obj);

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
    *gc_data  = DS_VECTOR_INTERNAL(Z_DS_VECTOR_P(obj));
    *gc_count = 1;

    return NULL;
}

/**
 * Count elements handler.
 */
static int ds_vector_count_elements(zval *obj, zend_long *count)
{
    *count = DS_VECTOR_USED(Z_DS_VECTOR_P(obj));

    return SUCCESS;
}

/**
 * Create an iterator of this vector's buffer.
 */
zend_object_iterator *ds_vector_get_iterator(zend_class_entry *ce, zval *obj, int by_ref)
{
    ds_vector_t *vector = Z_DS_VECTOR_P(obj);
    ds_buffer_t *buffer = DS_VECTOR_BUFFER(vector);

    return ds_buffer_iterator(buffer, 0, DS_VECTOR_USED(vector));
}

/**
 * Register vector class entry.
 */
void ds_register_vector()
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "Ds", "Vector", NULL);
    ds_vector_ce = zend_register_internal_class(&ce);

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
