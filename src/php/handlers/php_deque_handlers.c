#include "php_common_handlers.h"
#include "php_deque_handlers.h"

#include "../objects/php_deque.h"
#include "../../ds/ds_deque.h"

zend_object_handlers php_deque_handlers;

static zval *php_ds_deque_read_dimension(zval *obj, zval *offset, int type, zval *return_value)
{
    ds_deque_t *deque = Z_DS_DEQUE_P(obj);
    zval *value;

    // Dereference the offset if it's a reference.
    ZVAL_DEREF(offset);

    // `??`
    if (type == BP_VAR_IS) {
        if (Z_TYPE_P(offset) != IS_LONG || ! ds_deque_isset(deque, Z_LVAL_P(offset), 0)) {
            return &EG(uninitialized_zval);
        }
    }

    // Enforce strict integer index.
    if (Z_TYPE_P(offset) != IS_LONG) {
        INTEGER_INDEX_REQUIRED(offset);
        return NULL;
    }

    // Access the value at the given index.
    value = ds_deque_get(deque, Z_LVAL_P(offset));

    // If we're accessing by reference we have to create a reference.
    // This is for access like $deque[$a][$b] = $c
    if (value && type != BP_VAR_R) {
        ZVAL_MAKE_REF(value);
    }

    return value;
}

static void php_ds_deque_write_dimension(zval *obj, zval *offset, zval *value)
{
    ds_deque_t *deque = Z_DS_DEQUE_P(obj);

    if (offset == NULL) { /* $v[] = ... */
        ds_deque_push(deque, value);

    } else {
        ZVAL_DEREF(offset);

        if (Z_TYPE_P(offset) != IS_LONG) {
            INTEGER_INDEX_REQUIRED(offset);
        } else {
            ds_deque_set(deque, Z_LVAL_P(offset), value);
        }
    }
}

static int php_ds_deque_has_dimension(zval *obj, zval *offset, int check_empty)
{
    if (Z_TYPE_P(offset) != IS_LONG) {
        return 0;
    }

    ZVAL_DEREF(offset);

    return ds_deque_isset(Z_DS_DEQUE_P(obj), Z_LVAL_P(offset), check_empty);
}

static void php_ds_deque_unset_dimension(zval *obj, zval *offset)
{
    ZVAL_DEREF(offset);

    if (Z_TYPE_P(offset) != IS_LONG) {
        return;

    } else {
        zend_long   index = Z_LVAL_P(offset);
        ds_deque_t *deque = Z_DS_DEQUE_P(obj);

        if (index >= 0 && index < deque->size) {
            ds_deque_remove(deque, index, NULL);
        }
    }
}

static int php_ds_deque_count_elements(zval *obj, zend_long *count)
{
    ds_deque_t *deque = Z_DS_DEQUE_P(obj);
    *count = deque->size;
    return SUCCESS;
}

static void php_ds_deque_free_object(zend_object *object)
{
    php_ds_deque_t *obj = (php_ds_deque_t*) object;
    zend_object_std_dtor(&obj->std);
    ds_deque_free(obj->deque);
}

static HashTable *php_ds_deque_get_debug_info(zval *obj, int *is_temp)
{
    zval return_value;
    ds_deque_t *deque = Z_DS_DEQUE_P(obj);

    *is_temp = 1;

    ds_deque_to_array(deque, &return_value);
    return Z_ARRVAL(return_value);
}

static zend_object *php_ds_deque_clone_obj(zval *obj)
{
    ds_deque_t *deque = Z_DS_DEQUE_P(obj);
    return php_ds_deque_create_clone(deque);
}

static HashTable *php_ds_deque_get_gc(zval *obj, zval **gc_data, int *gc_count)
{
    ds_deque_t *deque = Z_DS_DEQUE_P(obj);

    *gc_data  = deque->buffer;
    *gc_count = deque->head == 0 ? deque->size : deque->capacity;

    return NULL;
}

void php_ds_register_deque_handlers()
{
    memcpy(&php_deque_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    php_deque_handlers.offset = XtOffsetOf(php_ds_deque_t, std);

    php_deque_handlers.dtor_obj         = zend_objects_destroy_object;
    php_deque_handlers.free_obj         = php_ds_deque_free_object;
    php_deque_handlers.get_gc           = php_ds_deque_get_gc;
    php_deque_handlers.cast_object      = php_ds_default_cast_object;
    php_deque_handlers.clone_obj        = php_ds_deque_clone_obj;
    php_deque_handlers.get_debug_info   = php_ds_deque_get_debug_info;
    php_deque_handlers.count_elements   = php_ds_deque_count_elements;
    php_deque_handlers.read_dimension   = php_ds_deque_read_dimension;
    php_deque_handlers.write_dimension  = php_ds_deque_write_dimension;
    php_deque_handlers.has_dimension    = php_ds_deque_has_dimension;
    php_deque_handlers.unset_dimension  = php_ds_deque_unset_dimension;
}
