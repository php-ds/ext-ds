
#include "php.h"
#include "ext/spl/spl_exceptions.h"
#include "../common.h"
#include "../internal/php_vector.h"

zend_object_handlers vector_object_handlers;

static zval *vector_read_dimension(zval *obj, zval *offset, int type, zval *return_value)
{
    if (Z_TYPE_P(offset) != IS_LONG) {
        INTEGER_INDEX_REQUIRED(offset);
        return NULL;

    } else {
        zval *value = vector_get(Z_VECTOR_P(obj), Z_LVAL_P(offset));

        // Create a reference to handle nested array access
        if (value && type != BP_VAR_R) {
            ZVAL_MAKE_REF(value);
        }

        return value;
    }
}

static void vector_write_dimension(zval *obj, zval *offset, zval *value)
{
    Vector *vector = Z_VECTOR_P(obj);

    /* $v[] = ... */
    if (offset == NULL) {
        vector_push(vector, value);

    } else if (Z_TYPE_P(offset) != IS_LONG) {
        INTEGER_INDEX_REQUIRED(offset);

    } else {
        vector_set(vector, Z_LVAL_P(offset), value);
    }
}

static int vector_has_dimension(zval *obj, zval *offset, int check_empty)
{
    if (Z_TYPE_P(offset) != IS_LONG) {
        return 0;
    }

    return vector_isset(Z_VECTOR_P(obj), Z_LVAL_P(offset), check_empty);
}

static void vector_unset_dimension(zval *obj, zval *offset)
{
    zend_long index;
    Vector *vector = Z_VECTOR_P(obj);

    if (Z_TYPE_P(offset) == IS_LONG) {
        index = Z_LVAL_P(offset);

    } else {
        if (zend_parse_parameter(ZEND_PARSE_PARAMS_QUIET, 1, offset, "l", &index) == FAILURE) {
            return;
        }
    }

    if (vector_index_exists(vector, index)) { // to avoid OOB
        vector_remove(vector, index, NULL);
    }
}

static int vector_count_elements(zval *obj, zend_long *count)
{
    *count = Z_VECTOR_P(obj)->size;
    return SUCCESS;
}

static void vector_free_object(zend_object *obj)
{
    VectorObj *intern = (VectorObj*) obj;
    zend_object_std_dtor(&intern->std);
    vector_destroy(intern->vector);
}

static HashTable *vector_get_debug_info(zval *obj, int *is_temp)
{
    zval arr;
    Vector *vector = Z_VECTOR_P(obj);

    *is_temp = 1;

    vector_to_array(vector, &arr);
    return Z_ARRVAL(arr);
}

static zend_object *vector_clone_obj(zval *obj)
{
    return vector_create_clone(Z_VECTOR_P(obj));
}

static HashTable *vector_get_gc(zval *obj, zval **gc_data, int *gc_count)
{
    Vector *vector = Z_VECTOR_P(obj);

    *gc_data  = vector->buffer;
    *gc_count = (int) vector->size;

    return NULL;
}

void register_vector_handlers()
{
    memcpy(&vector_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    vector_object_handlers.offset = XtOffsetOf(VectorObj, std);

    vector_object_handlers.dtor_obj = zend_objects_destroy_object;
    vector_object_handlers.free_obj = vector_free_object;
    vector_object_handlers.get_gc   = vector_get_gc;

    vector_object_handlers.clone_obj        = vector_clone_obj;
    vector_object_handlers.cast_object      = ds_default_cast_object;
    vector_object_handlers.get_debug_info   = vector_get_debug_info;

    vector_object_handlers.count_elements   = vector_count_elements;
    vector_object_handlers.read_dimension   = vector_read_dimension;
    vector_object_handlers.write_dimension  = vector_write_dimension;
    vector_object_handlers.has_dimension    = vector_has_dimension;
    vector_object_handlers.unset_dimension  = vector_unset_dimension;
}
