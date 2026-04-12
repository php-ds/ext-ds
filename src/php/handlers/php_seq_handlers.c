#include "php_common_handlers.h"
#include "php_seq_handlers.h"

#include "../objects/php_seq.h"
#include "../../ds/ds_seq.h"

zend_object_handlers php_seq_handlers;

static zval *php_ds_seq_read_dimension
(zend_object *obj, zval *offset, int type, zval *return_value) {
    ds_seq_t *seq = php_ds_seq_fetch_object(obj)->seq;
    zval *value;

    // Dereference the offset if it's a reference.
    ZVAL_DEREF(offset);

    // `??`
    if (type == BP_VAR_IS) {
        if (Z_TYPE_P(offset) != IS_LONG || ! ds_seq_isset(seq, Z_LVAL_P(offset), 0)) {
            return &EG(uninitialized_zval);
        }
    }

    // Enforce strict integer index.
    if (Z_TYPE_P(offset) != IS_LONG) {
        INTEGER_INDEX_REQUIRED(offset);
        return NULL;
    }

    // Access the value at the given index.
    value = ds_seq_get(seq, Z_LVAL_P(offset));

    // If we're accessing by reference we have to create a reference.
    // This is for access like $seq[$a][$b] = $c
    if (value && type != BP_VAR_R && type != BP_VAR_IS) {
        ZVAL_MAKE_REF(value);
    }

    return value;
}

static void php_ds_seq_write_dimension
(zend_object *obj, zval *offset, zval *value) {
    php_ds_seq_t *php_obj = php_ds_seq_fetch_object(obj);
    ds_seq_separate(&php_obj->seq);
    ds_seq_t *seq = php_obj->seq;
    if (offset == NULL) { /* $v[] = ... */
        ds_seq_push(seq, value);

    } else {
        ZVAL_DEREF(offset);

        if (Z_TYPE_P(offset) != IS_LONG) {
            INTEGER_INDEX_REQUIRED(offset);
        } else {
            ds_seq_set(seq, Z_LVAL_P(offset), value);
        }
    }
}

static int php_ds_seq_has_dimension
(zend_object *obj, zval *offset, int check_empty) {
    ds_seq_t *seq = php_ds_seq_fetch_object(obj)->seq;
    if (Z_TYPE_P(offset) != IS_LONG) {
        return 0;
    }

    ZVAL_DEREF(offset);

    return ds_seq_isset(seq, Z_LVAL_P(offset), check_empty);
}

static void php_ds_seq_unset_dimension
(zend_object *obj, zval *offset) {
    php_ds_seq_t *php_obj = php_ds_seq_fetch_object(obj);
    ds_seq_separate(&php_obj->seq);
    ds_seq_t *seq = php_obj->seq;
    zend_long index = 0;
    ZVAL_DEREF(offset);

    if (Z_TYPE_P(offset) == IS_LONG) {
        index = Z_LVAL_P(offset);

    } else {
        if (zend_parse_parameter(ZEND_PARSE_PARAMS_QUIET, 1, offset, "l", &index) == FAILURE) {
            return;
        }
    }

    if (ds_seq_index_exists(seq, index)) {
        ds_seq_remove(seq, index, NULL);
    }
}

static int php_ds_seq_count_elements
(zend_object *obj, zend_long *count) {
    *count = php_ds_seq_fetch_object(obj)->seq->size;
    return SUCCESS;
}

static void php_ds_seq_free_object(zend_object *object)
{
    php_ds_seq_t *obj = php_ds_seq_fetch_object(object);
    ds_seq_release(obj->seq);
    zend_object_std_dtor(&obj->std);
}

static HashTable *php_ds_seq_get_debug_info
(zend_object *obj, int *is_temp) {
    ds_seq_t *seq = php_ds_seq_fetch_object(obj)->seq;
    zval arr;
    *is_temp = 1;
    ds_seq_to_array(seq, &arr);
    return Z_ARRVAL(arr);
}

static zend_object *php_ds_seq_clone_obj
(zend_object *obj) {
    return php_ds_seq_create_clone(php_ds_seq_fetch_object(obj)->seq);
}

static HashTable *php_ds_seq_get_gc
(zend_object *obj, zval **gc_data, int *gc_count) {
    ds_seq_t *seq = php_ds_seq_fetch_object(obj)->seq;
    *gc_data  = seq->buffer;
    *gc_count = (int) (seq->head == 0 ? seq->size : seq->capacity);

    return NULL;
}

void php_ds_register_seq_handlers()
{
    memcpy(&php_seq_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    php_seq_handlers.offset = XtOffsetOf(php_ds_seq_t, std);

    php_seq_handlers.dtor_obj         = zend_objects_destroy_object;
    php_seq_handlers.free_obj         = php_ds_seq_free_object;
    php_seq_handlers.get_gc           = php_ds_seq_get_gc;
    php_seq_handlers.cast_object      = php_ds_default_cast_object;
    php_seq_handlers.clone_obj        = php_ds_seq_clone_obj;
    php_seq_handlers.get_debug_info   = php_ds_seq_get_debug_info;
    php_seq_handlers.count_elements   = php_ds_seq_count_elements;
    php_seq_handlers.read_dimension   = php_ds_seq_read_dimension;
    php_seq_handlers.write_dimension  = php_ds_seq_write_dimension;
    php_seq_handlers.has_dimension    = php_ds_seq_has_dimension;
    php_seq_handlers.unset_dimension  = php_ds_seq_unset_dimension;
}
