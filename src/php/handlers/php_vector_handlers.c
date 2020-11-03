#include "php_common_handlers.h"
#include "php_vector_handlers.h"

#include "../objects/php_vector.h"
#include "../../ds/ds_vector.h"

zend_object_handlers php_vector_handlers;

static zval *php_ds_vector_read_dimension
#if PHP_VERSION_ID >= 80000
(zend_object *obj, zval *offset, int type, zval *return_value) {
    ds_vector_t *vector = php_ds_vector_fetch_object(obj)->vector;
#else
(zval *obj, zval *offset, int type, zval *return_value) {
    ds_vector_t *vector = Z_DS_VECTOR_P(obj);
#endif
    zval *value;

    // Dereference the offset if it's a reference.
    ZVAL_DEREF(offset);

    // `??`
    if (type == BP_VAR_IS) {
        if (Z_TYPE_P(offset) != IS_LONG || ! ds_vector_isset(vector, Z_LVAL_P(offset), 0)) {
            return &EG(uninitialized_zval);
        }
    }

    // Enforce strict integer index.
    if (Z_TYPE_P(offset) != IS_LONG) {
        INTEGER_INDEX_REQUIRED(offset);
        return NULL;
    }

    // Access the value at the given index.
    value = ds_vector_get(vector, Z_LVAL_P(offset));

    // If we're accessing by reference we have to create a reference.
    // This is for access like $deque[$a][$b] = $c
    if (value && type != BP_VAR_R) {
        ZVAL_MAKE_REF(value);
    }

    return value;
}

static void php_ds_vector_write_dimension
#if PHP_VERSION_ID >= 80000
(zend_object *obj, zval *offset, zval *value) {
    ds_vector_t *vector = php_ds_vector_fetch_object(obj)->vector;
#else
(zval *obj, zval *offset, zval *value) {
    ds_vector_t *vector = Z_DS_VECTOR_P(obj);
#endif
    if (offset == NULL) { /* $v[] = ... */
        ds_vector_push(vector, value);

    } else {
        ZVAL_DEREF(offset);

        if (Z_TYPE_P(offset) != IS_LONG) {
            INTEGER_INDEX_REQUIRED(offset);
        } else {
            ds_vector_set(vector, Z_LVAL_P(offset), value);
        }
    }
}

static int php_ds_vector_has_dimension
#if PHP_VERSION_ID >= 80000
(zend_object *obj, zval *offset, int check_empty) {
    ds_vector_t *vector = php_ds_vector_fetch_object(obj)->vector;
#else
(zval *obj, zval *offset, int check_empty) {
    ds_vector_t *vector = Z_DS_VECTOR_P(obj);
#endif
    if (Z_TYPE_P(offset) != IS_LONG) {
        return 0;
    }
    ZVAL_DEREF(offset);

    return ds_vector_isset(vector, Z_LVAL_P(offset), check_empty);
}

static void php_ds_vector_unset_dimension
#if PHP_VERSION_ID >= 80000
(zend_object *obj, zval *offset) {
    ds_vector_t *vector = php_ds_vector_fetch_object(obj)->vector;
#else
(zval *obj, zval *offset) {
    ds_vector_t *vector = Z_DS_VECTOR_P(obj);
#endif
    zend_long index = 0;
    ZVAL_DEREF(offset);

    if (Z_TYPE_P(offset) == IS_LONG) {               
        index = Z_LVAL_P(offset);

    } else {
        if (zend_parse_parameter(ZEND_PARSE_PARAMS_QUIET, 1, offset, "l", &index) == FAILURE) {
            return;
        }
    }

    if (ds_vector_index_exists(vector, index)) { // to avoid OutOfBounds
        ds_vector_remove(vector, index, NULL);
    }
}

static int php_ds_vector_count_elements
#if PHP_VERSION_ID >= 80000
(zend_object *obj, zend_long *count) {
    *count = php_ds_vector_fetch_object(obj)->vector->size; return SUCCESS;
#else 
(zval *obj, zend_long *count) {
    *count = Z_DS_VECTOR_P(obj)->size; return SUCCESS;
#endif 
}

static void php_ds_vector_free_object(zend_object *obj)
{
    php_ds_vector_t *vector = php_ds_vector_fetch_object(obj);
    ds_vector_free(vector->vector);
    zend_object_std_dtor(&vector->std);
}

static HashTable *php_ds_vector_get_debug_info
#if PHP_VERSION_ID >= 80000
(zend_object *obj, int *is_temp) {
    ds_vector_t *vector = php_ds_vector_fetch_object(obj)->vector;
#else
(zval *obj, int *is_temp) {
    ds_vector_t *vector = Z_DS_VECTOR_P(obj);
#endif    
    zval arr;
    *is_temp = 1;
    ds_vector_to_array(vector, &arr);
    return Z_ARRVAL(arr);
}

static zend_object *php_ds_vector_clone_obj
#if PHP_VERSION_ID >= 80000
(zend_object *obj) {
    return php_ds_vector_create_clone(php_ds_vector_fetch_object(obj)->vector);
#else 
(zval *obj) {
    return php_ds_vector_create_clone(Z_DS_VECTOR_P(obj));
#endif    
}

static HashTable *php_ds_vector_get_gc
#if PHP_VERSION_ID >= 80000
(zend_object *obj, zval **gc_data, int *gc_count) {
    ds_vector_t *vector = php_ds_vector_fetch_object(obj)->vector;
#else 
(zval *obj, zval **gc_data, int *gc_count) {
    ds_vector_t *vector = Z_DS_VECTOR_P(obj);
#endif    
    *gc_data  = vector->buffer;
    *gc_count = (int) vector->size;

    return NULL;
}

void php_register_vector_handlers()
{
    memcpy(&php_vector_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    php_vector_handlers.offset = XtOffsetOf(php_ds_vector_t, std);

    php_vector_handlers.dtor_obj         = zend_objects_destroy_object;
    php_vector_handlers.free_obj         = php_ds_vector_free_object;
    php_vector_handlers.get_gc           = php_ds_vector_get_gc;
    php_vector_handlers.clone_obj        = php_ds_vector_clone_obj;
    php_vector_handlers.cast_object      = php_ds_default_cast_object;
    php_vector_handlers.get_debug_info   = php_ds_vector_get_debug_info;
    php_vector_handlers.count_elements   = php_ds_vector_count_elements;
    php_vector_handlers.read_dimension   = php_ds_vector_read_dimension;
    php_vector_handlers.write_dimension  = php_ds_vector_write_dimension;
    php_vector_handlers.has_dimension    = php_ds_vector_has_dimension;
    php_vector_handlers.unset_dimension  = php_ds_vector_unset_dimension;
}
