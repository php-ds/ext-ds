#include <php.h>
#include "buffer.h"

zend_class_entry *ds_buffer_ce;

static zend_object_handlers ds_buffer_handlers;

ds_buffer_t *ds_buffer(uint32_t capacity)
{
    php_printf("buffer: allocate capacity of %d\n", capacity);

    ds_buffer_t *buffer = ecalloc(1, sizeof(ds_buffer_t));
    zend_object *object = (zend_object *) buffer;

    zend_object_std_init((zend_object *) buffer, ds_buffer_ce);

    object->handlers = &ds_buffer_handlers;
    buffer->data     = ecalloc(capacity, sizeof(zval));
    buffer->len      = capacity;

    return buffer;
}

static void ds_buffer_free_object(zend_object *obj)
{
    ds_buffer_t *buffer = (ds_buffer_t*) obj;

    zend_object_std_dtor(obj);
    efree(buffer->data);
}

void ds_buffer_to_array(zval *arr, ds_buffer_t *buffer, uint32_t len)
{
    zval *pos = buffer->data;
    zval *end = buffer->data + len;

    array_init_size(arr, len);

    for (; pos != end; ++pos) {
        add_next_index_zval(arr, pos);
        Z_TRY_ADDREF_P(pos);
    }
}

zval *ds_buffer_get(ds_buffer_t *buffer, uint32_t offset)
{
    return &buffer->data[offset];
}

void ds_buffer_set(ds_buffer_t *buffer, uint32_t offset, zval *value)
{
    ZVAL_COPY(&buffer->data[offset], value);
}

void ds_buffer_realloc(ds_buffer_t *buffer, uint32_t capacity)
{
    php_printf("buffer: increase capacity to %d\n", capacity);

    buffer->data = erealloc(buffer->data, capacity * sizeof(zval));
    buffer->len  = capacity;
}

ds_buffer_t *ds_buffer_create_copy(ds_buffer_t *src)
{
    php_printf("buffer: create copy!\n");

    uint32_t i;
    ds_buffer_t *dst = ds_buffer(src->len);

    for (i = 0; i < src->len; i++) {
        ZVAL_COPY(dst->data + i, src->data + i);
    }

    return dst;
}

static void ds_buffer_iterator_dtor(zend_object_iterator *iterator)
{
    zval_ptr_dtor(&iterator->data);
}

static int ds_buffer_iterator_valid(zend_object_iterator *iterator)
{
    ds_buffer_iterator_t *iter = (ds_buffer_iterator_t *) iterator;

    return iter->pos < iter->len ? SUCCESS : FAILURE;
}

static zval *ds_buffer_iterator_get_current_data(zend_object_iterator *iterator)
{
    ds_buffer_iterator_t *iter = (ds_buffer_iterator_t *) iterator;

    return ds_buffer_get(DS_ZVAL_GET_BUFFER(&iterator->data), iter->pos + iter->offset);
}

static void ds_buffer_iterator_get_current_key(zend_object_iterator *iterator, zval *key)
{
    ds_buffer_iterator_t *iter = (ds_buffer_iterator_t *) iterator;

    ZVAL_LONG(key, iter->pos);
}

static void ds_buffer_iterator_move_forward(zend_object_iterator *iterator)
{
    ds_buffer_iterator_t *iter = (ds_buffer_iterator_t *) iterator;

    iter->pos++;
}

static void ds_buffer_iterator_rewind(zend_object_iterator *iterator)
{
    ds_buffer_iterator_t *iter = (ds_buffer_iterator_t *) iterator;

    iter->pos = 0;
}

static zend_object_iterator_funcs ds_buffer_iterator_functions = {
    ds_buffer_iterator_dtor,
    ds_buffer_iterator_valid,
    ds_buffer_iterator_get_current_data,
    ds_buffer_iterator_get_current_key,
    ds_buffer_iterator_move_forward,
    ds_buffer_iterator_rewind
};

zend_object_iterator *ds_buffer_iterator(ds_buffer_t *buffer, uint32_t offset, uint32_t len)
{
    ds_buffer_iterator_t *iter = ecalloc(1, sizeof(ds_buffer_iterator_t));

    zend_iterator_init(&iter->intern);

    iter->intern.funcs  = &ds_buffer_iterator_functions;
    iter->offset        = offset;
    iter->len           = len;
    iter->pos           = 0;

    ZVAL_OBJ(&iter->intern.data, (zend_object *) buffer);
    Z_TRY_ADDREF(iter->intern.data);

    return (zend_object_iterator *) iter;
}

static HashTable *ds_buffer_get_gc(zval *obj, zval **gc_data, int *gc_count)
{
    ds_buffer_t *buffer = DS_ZVAL_GET_BUFFER(obj);

    *gc_data  = buffer->data;
    *gc_count = buffer->len;

    return NULL;
}

void ds_register_buffer()
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "__ds_buffer_internal", NULL);
    ds_buffer_ce = zend_register_internal_class(&ce);

    memcpy(&ds_buffer_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    ds_buffer_handlers.get_gc   = ds_buffer_get_gc;
    ds_buffer_handlers.free_obj = ds_buffer_free_object;
}
