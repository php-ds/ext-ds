#include <php.h>
#include "vector.h"
#include "buffer.h"
#include "Zend/zend_types.h"

zend_class_entry *ds_vector_ce;

static zend_object_handlers ds_vector_handlers;

ds_vector_t *ds_vector(uint32_t capacity)
{
    ds_vector_t *vector = ecalloc(1, sizeof(ds_vector_t));
    zend_object *object = (zend_object *) vector;

    zend_object_std_init(object, ds_vector_ce);

    DS_VECTOR_SET_BUFFER(vector, ds_buffer(capacity));
    object->handlers = &ds_vector_handlers;
    vector->size     = 0;

    return vector;
}

static zend_object *ds_vector_create_object(zend_class_entry *ce)
{
    return (zend_object *) ds_vector(2);  // 2 is low, but easy to test for now.
}

static void ds_vector_free_object(zend_object *obj)
{
    ds_vector_t *vector = (ds_vector_t *) obj;

    zend_object_std_dtor(obj);
    zval_ptr_dtor(&vector->data);
}

static void ds_vector_separate(ds_vector_t *vector)
{
    ds_buffer_t *buffer = DS_VECTOR_BUFFER(vector);

    if (GC_REFCOUNT((zend_object *) buffer) == 1) {
        return;
    }

    GC_DELREF((zend_object *) buffer);
    DS_VECTOR_SET_BUFFER(vector, ds_buffer_create_copy(buffer));
}

zval *ds_vector_get(ds_vector_t *vector, zend_long offset)
{
    return ds_buffer_get(DS_VECTOR_BUFFER(vector), offset);
}

void ds_vector_set(ds_vector_t *vector, zend_long offset, zval *value)
{
    php_printf("vector: update offset %d\n", offset);
    ds_buffer_t *buffer;
    ds_vector_separate(vector);
    buffer = DS_VECTOR_BUFFER(vector);

    ds_buffer_set(buffer, offset, value);
    vector->size++;

    php_printf("vector: size is now %d\n", vector->size);
}

void ds_vector_push(ds_vector_t *vector, zval *value)
{
    php_printf("vector: push into offset %d\n", vector->size);

    ds_buffer_t *buffer;
    ds_vector_separate(vector);
    buffer = DS_VECTOR_BUFFER(vector);

    /* Check if we need to grow. */
    if (vector->size == buffer->len) {
        ds_buffer_realloc(buffer, buffer->len * 2 + 1);
    }

    ds_buffer_set(buffer, vector->size, value);
    vector->size++;

    php_printf("vector: size is now %d\n", vector->size);
}

zend_object_iterator *ds_vector_get_iterator(zend_class_entry *ce, zval *obj, int by_ref)
{
    ds_vector_t *vector = DS_ZVAL_GET_VECTOR(obj);
    ds_buffer_t *buffer = DS_VECTOR_BUFFER(vector);

    return ds_buffer_iterator(buffer, 0, vector->size);
}

static HashTable *ds_vector_get_debug_info(zval *obj, int *is_temp)
{
    zval arr;
    ds_vector_t *vector = DS_ZVAL_GET_VECTOR(obj);

    *is_temp = 1;

    ds_buffer_to_array(&arr, DS_VECTOR_BUFFER(vector), vector->size);

    return Z_ARRVAL(arr);
}

static zval *ds_vector_read_dimension(zval *obj, zval *offset, int type, zval *rv)
{
    return ds_vector_get(DS_ZVAL_GET_VECTOR(obj), Z_LVAL_P(offset));
}

static void ds_vector_write_dimension(zval *obj, zval *offset, zval *value)
{
    ds_vector_t *vector = DS_ZVAL_GET_VECTOR(obj);

    /* $v[] = ... */
    if (offset == NULL) {
        ds_vector_push(vector, value);
    } else {
        ds_vector_set(vector, Z_LVAL_P(offset), value);
    }
}

static HashTable *ds_vector_get_gc(zval *obj, zval **gc_data, int *gc_count)
{
    ds_vector_t *vector = DS_ZVAL_GET_VECTOR(obj);

    *gc_data  = &vector->data;
    *gc_count = 1;

    return NULL;
}

void ds_register_vector()
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Vector", NULL);
    ds_vector_ce = zend_register_internal_class(&ce);

    ds_vector_ce->create_object = ds_vector_create_object;
    ds_vector_ce->get_iterator  = ds_vector_get_iterator;

    memcpy(&ds_vector_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    ds_vector_handlers.get_gc          = ds_vector_get_gc;
    ds_vector_handlers.free_obj        = ds_vector_free_object;
    ds_vector_handlers.get_debug_info  = ds_vector_get_debug_info;
    ds_vector_handlers.read_dimension  = ds_vector_read_dimension;
    ds_vector_handlers.write_dimension = ds_vector_write_dimension;
}
