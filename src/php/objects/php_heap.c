#include "../handlers/php_heap_handlers.h"
#include "../classes/php_heap_ce.h"

#include "php_heap.h"

int ds_heap_user_compare(zval *a, zval *b)
{
    zval params[2];
    zval retval;

    ZVAL_COPY_VALUE(&params[0], a);
    ZVAL_COPY_VALUE(&params[1], b);

    DSG(user_compare_fci).param_count = 2;
    DSG(user_compare_fci).params      = params;
    DSG(user_compare_fci).retval      = &retval;

    if (zend_call_function(
            &DSG(user_compare_fci),
            &DSG(user_compare_fci_cache)) == SUCCESS) {
        int result = (int) zval_get_long(&retval);
        zval_ptr_dtor(&retval);
        return result;
    }

    return 0;
}

ds_heap_compare_func_t php_ds_heap_get_compare(php_ds_heap_t *obj)
{
    if (obj->has_comparator) {
        DSG(user_compare_fci)       = obj->fci;
        DSG(user_compare_fci_cache) = obj->fci_cache;
        return ds_heap_user_compare;
    }
    return ds_heap_default_compare;
}

zend_object *php_ds_heap_create_object_ex(ds_heap_t *heap)
{
    php_ds_heap_t *obj = zend_object_alloc(sizeof(php_ds_heap_t), php_ds_heap_ce);
    zend_object_std_init(&obj->std, php_ds_heap_ce);
    obj->std.handlers  = &php_heap_handlers;
    obj->heap           = heap;
    obj->has_comparator = false;
    return &obj->std;
}

zend_object *php_ds_heap_create_object(zend_class_entry *ce)
{
    return php_ds_heap_create_object_ex(ds_heap());
}

zend_object *php_ds_heap_create_clone(php_ds_heap_t *obj)
{
    obj->heap->refs++;

    php_ds_heap_t *clone = php_ds_heap_fetch_object(
        php_ds_heap_create_object_ex(obj->heap)
    );

    clone->has_comparator = obj->has_comparator;

    if (obj->has_comparator) {
        clone->fci       = obj->fci;
        clone->fci_cache = obj->fci_cache;

        // Add references to the callable.
        if (ZEND_FCI_INITIALIZED(clone->fci)) {
            Z_TRY_ADDREF(clone->fci.function_name);
        }
    }

    return &clone->std;
}

int php_ds_heap_serialize(zval *object, unsigned char **buffer, size_t *length, zend_serialize_data *data)
{
    php_ds_heap_t *obj = Z_DS_HEAP_P(object);
    ds_heap_t *heap = obj->heap;

    php_serialize_data_t serialize_data = (php_serialize_data_t) data;
    PHP_VAR_SERIALIZE_INIT(serialize_data);

    if (heap->size == 0) {
        SERIALIZE_SET_ZSTR(ZSTR_EMPTY_ALLOC());
    } else {
        smart_str buf = {0};

        // Serialize in heap-order (to preserve insertion order on unserialize).
        for (uint32_t i = 0; i < heap->size; i++) {
            php_var_serialize(&buf, &heap->nodes[i], &serialize_data);
        }

        smart_str_0(&buf);
        SERIALIZE_SET_ZSTR(buf.s);
        zend_string_release(buf.s);
    }

    PHP_VAR_SERIALIZE_DESTROY(serialize_data);
    return SUCCESS;
}

int php_ds_heap_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t length, zend_unserialize_data *data)
{
    ds_heap_t *heap = ds_heap();

    php_unserialize_data_t unserialize_data = (php_unserialize_data_t) data;

    const unsigned char *pos = buffer;
    const unsigned char *end = buffer + length;

    PHP_VAR_UNSERIALIZE_INIT(unserialize_data);

    while (pos != end) {
        zval *value = var_tmp_var(&unserialize_data);

        if (!php_var_unserialize(value, &pos, end, &unserialize_data)) {
            goto error;
        }

        ds_heap_push(heap, value, ds_heap_default_compare);
    }

    ZVAL_OBJ(object, php_ds_heap_create_object_ex(heap));
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    return SUCCESS;

error:
    ds_heap_free(heap);
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    UNSERIALIZE_ERROR();
    return FAILURE;
}
