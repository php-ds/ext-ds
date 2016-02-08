
#include "php.h"
#include "ext/spl/spl_exceptions.h"
#include "../common.h"
#include "../internal/php_deque.h"

zend_object_handlers deque_handlers;

static zval *deque_read_dimension(zval *obj, zval *offset, int type, zval *return_value)
{
    if (Z_TYPE_P(offset) != IS_LONG) {
        INTEGER_INDEX_REQUIRED(offset);
        return NULL;

    } else {
        zval *value = deque_get(Z_DEQUE_P(obj), Z_LVAL_P(offset));

        // Create a reference to handle nested array access
        if (value && type != BP_VAR_R) {
            ZVAL_MAKE_REF(value);
        }
        return value;
    }
}

static void deque_write_dimension(zval *obj, zval *offset, zval *value)
{
    Deque *deque = Z_DEQUE_P(obj);

    if (offset == NULL) { /* $v[] = ... */
        deque_push(deque, value);

    } else if (Z_TYPE_P(offset) != IS_LONG) {
        INTEGER_INDEX_REQUIRED(offset);

    } else {
        deque_set(deque, Z_LVAL_P(offset), value);
    }
}

static int deque_has_dimension(zval *obj, zval *offset, int check_empty)
{
    if (Z_TYPE_P(offset) != IS_LONG) {
        return 0;
    }

    return deque_isset(Z_DEQUE_P(obj), Z_LVAL_P(offset), check_empty);
}

static void deque_unset_dimension(zval *obj, zval *offset)
{
    zend_long index;
    Deque *deque = Z_DEQUE_P(obj);

    if (Z_TYPE_P(offset) != IS_LONG) {
        return;
    }

    index = Z_LVAL_P(offset);

    if (index >= 0 && index < DEQUE_SIZE(deque)) {
        deque_remove(deque, index, NULL);
    }
}

static int deque_count_elements(zval *obj, zend_long *count)
{
    Deque *deque = Z_DEQUE_P(obj);
    *count = DEQUE_SIZE(deque);
    return SUCCESS;
}

static void deque_free_object(zend_object *object)
{
    DequeObj *intern = (DequeObj*) object;
    zend_object_std_dtor(&intern->std);
    deque_destroy(intern->deque);
    efree(intern->deque);
}

static HashTable *deque_get_debug_info(zval *obj, int *is_temp)
{
    zval return_value;
    Deque *deque = Z_DEQUE_P(obj);

    *is_temp = 1;

    deque_to_array(deque, &return_value);
    return Z_ARRVAL(return_value);
}

static zend_object *deque_create_copy_obj(zval *obj)
{
    Deque *deque = Z_DEQUE_P(obj);
    return deque_create_clone(deque);
}

void register_deque_handlers()
{
    memcpy(&deque_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    deque_handlers.offset = XtOffsetOf(DequeObj, std);

    deque_handlers.dtor_obj         = zend_objects_destroy_object;
    deque_handlers.free_obj         = deque_free_object;
    deque_handlers.cast_object      = ds_default_cast_object;
    deque_handlers.clone_obj        = deque_create_copy_obj;
    deque_handlers.get_debug_info   = deque_get_debug_info;
    deque_handlers.count_elements   = deque_count_elements;
    deque_handlers.read_dimension   = deque_read_dimension;
    deque_handlers.write_dimension  = deque_write_dimension;
    deque_handlers.has_dimension    = deque_has_dimension;
    deque_handlers.unset_dimension  = deque_unset_dimension;
}
