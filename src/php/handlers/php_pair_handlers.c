#include "php_pair_handlers.h"
#include "php_common_handlers.h"
#include "../../ds/ds_pair.h"
#include "../objects/php_pair.h"

zend_object_handlers php_pair_handlers;

static zval *get_property(ds_pair_t *pair, zval *offset)
{
    if (offset && Z_TYPE_P(offset) == IS_STRING) {
        if (ZVAL_EQUALS_STRING(offset, "key")) {
            return &pair->key;
        }
        if (ZVAL_EQUALS_STRING(offset, "value")) {
            return &pair->value;
        }
    }

    return NULL;
}

static zval *php_ds_pair_get_property_ptr_ptr(zval *object, zval *offset, int type, void **cache_slot)
{
    zval *property = get_property(Z_DS_PAIR_P(object), offset);

    if ( ! property) {
        return &EG(uninitialized_zval);
    }

    return property;
}

static zval *php_ds_pair_read_property(zval *object, zval *offset, int type, void **cache_slot, zval *rv)
{
    zval *property = get_property(Z_DS_PAIR_P(object), offset);

    if ( ! property) {
        OFFSET_OUT_OF_BOUNDS();
        return &EG(uninitialized_zval);
    }

    return property;
}

static void php_ds_pair_write_property(zval *object, zval *offset, zval *value, void **cache_slot)
{
    zval *property = get_property(Z_DS_PAIR_P(object), offset);

    if (property) {
        zval_ptr_dtor(property);
        ZVAL_COPY(property, value);
        return;
    }

    OFFSET_OUT_OF_BOUNDS();
}

static int php_ds_pair_has_property(zval *object, zval *offset, int has_set_exists, void **cache_slot)
{
    zval *value = get_property(Z_DS_PAIR_P(object), offset);

    if ( ! value) {
        return 0;
    }

    //
    if (has_set_exists == 2) {
        return 1;
    }

    return ds_zval_isset(value, has_set_exists);
}

static void php_ds_pair_unset_property(zval *object, zval *offset, void **cache_slot)
{
    zval *property = get_property(Z_DS_PAIR_P(object), offset);

    if (property) {
        zval_ptr_dtor(property);
        ZVAL_NULL(property);
    }
}

static void php_ds_pair_free_object(zend_object *object)
{
    php_ds_pair_t *obj = (php_ds_pair_t*) object;
    zend_object_std_dtor(&obj->std);
    ds_pair_free(obj->pair);
}

static int php_ds_pair_count_elements(zval *object, zend_long *count)
{
    *count = 2;
    return SUCCESS;
}

static HashTable *php_ds_pair_get_debug_info(zval *object, int *is_temp)
{
    zval arr;
    ds_pair_t *pair = Z_DS_PAIR_P(object);

    *is_temp = 1;

    array_init_size(&arr, 2);

    add_assoc_zval_ex(&arr, "key", 3, &pair->key);
    add_assoc_zval_ex(&arr, "value", 5, &pair->value);

    Z_TRY_ADDREF_P(&pair->key);
    Z_TRY_ADDREF_P(&pair->value);

    return Z_ARRVAL(arr);
}

static zend_object *php_ds_pair_clone_object(zval *object)
{
    return php_ds_pair_create_clone(Z_DS_PAIR_P(object));
}

static HashTable *php_ds_pair_get_gc(zval *obj, zval **gc_data, int *gc_count)
{
    ds_pair_t *pair = Z_DS_PAIR_P(obj);

    *gc_data  = (zval*) pair;
    *gc_count = (int) 2;

    return NULL;
}

void php_ds_register_pair_handlers()
{
    memcpy(&php_pair_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    php_pair_handlers.offset = XtOffsetOf(php_ds_pair_t, std);

    php_pair_handlers.dtor_obj                = zend_objects_destroy_object;
    php_pair_handlers.get_gc                  = php_ds_pair_get_gc;
    php_pair_handlers.free_obj                = php_ds_pair_free_object;
    php_pair_handlers.clone_obj               = php_ds_pair_clone_object;
    php_pair_handlers.cast_object             = php_ds_default_cast_object;
    php_pair_handlers.get_debug_info          = php_ds_pair_get_debug_info;
    php_pair_handlers.count_elements          = php_ds_pair_count_elements;

    php_pair_handlers.get_property_ptr_ptr    = php_ds_pair_get_property_ptr_ptr;
    php_pair_handlers.read_property           = php_ds_pair_read_property;
    php_pair_handlers.write_property          = php_ds_pair_write_property;
    php_pair_handlers.has_property            = php_ds_pair_has_property;
    php_pair_handlers.unset_property          = php_ds_pair_unset_property;
}
