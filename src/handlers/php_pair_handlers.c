#include "../common.h"
#include "../internal/php_pair.h"

zend_object_handlers pair_handlers;

static zval *get_value(Pair *pair, zval *offset)
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

static zval *pair_read_property(zval *object, zval *offset, int type, void **cache_slot, zval *rv)
{
    Pair *pair = Z_PAIR_P(object);

    if (offset && Z_TYPE_P(offset) == IS_STRING) {
        if (ZVAL_EQUALS_STRING(offset, "key")) {
            return &pair->key;
        }
        if (ZVAL_EQUALS_STRING(offset, "value")) {
            return &pair->value;
        }
    }

    return &EG(uninitialized_zval);
}

static void pair_write_property(zval *object, zval *offset, zval *value, void **cache_slot)
{
    Pair *pair = Z_PAIR_P(object);

    if (offset && Z_TYPE_P(offset) == IS_STRING) {
        if (ZVAL_EQUALS_STRING(offset, "key")) {
            ZVAL_COPY(&pair->key, value);
            return;
        }
        if (ZVAL_EQUALS_STRING(offset, "value")) {
            ZVAL_COPY(&pair->value, value);
            return;
        }
    }

    OFFSET_OUT_OF_BOUNDS();
}

static int pair_has_property(zval *object, zval *offset, int has_set_exists, void **cache_slot)
{
    Pair *pair = Z_PAIR_P(object);
    zval *value = get_value(pair, offset);

    if ( ! value) {
        return 0;
    }

    //
    if (has_set_exists == 2) {
        return 1;
    }

    return zval_isset(value, has_set_exists);
}

static void pair_unset_property(zval *object, zval *offset, void **cache_slot)
{
    Pair *pair = Z_PAIR_P(object);

    if (offset && Z_TYPE_P(offset) == IS_STRING) {
        if (ZVAL_EQUALS_STRING(offset, "key")) {
            ZVAL_NULL(&pair->key);
        }
        if (ZVAL_EQUALS_STRING(offset, "value")) {
            ZVAL_NULL(&pair->value);
        }
    }
}

static void pair_free_object(zend_object *object)
{
    Pair *pair = (Pair*) object;
    zend_object_std_dtor(&pair->std);
    pair_destroy(pair);
}

static int pair_count_elements(zval *object, zend_long *count)
{
    *count = 2;
    return SUCCESS;
}

static HashTable *pair_get_debug_info(zval *object, int *is_temp)
{
    zval arr;
    Pair *pair = Z_PAIR_P(object);

    *is_temp = 1;

    array_init_size(&arr, 2);

    add_assoc_zval_ex(&arr, "key", 3, &pair->key);
    add_assoc_zval_ex(&arr, "value", 5, &pair->value);

    Z_TRY_ADDREF_P(&pair->key);
    Z_TRY_ADDREF_P(&pair->value);

    return Z_ARRVAL(arr);
}

static zend_object *pair_clone_object(zval *object)
{
    Pair *pair = Z_PAIR_P(object);
    return pair_create_clone(pair);
}

void register_pair_handlers()
{
    memcpy(&pair_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    pair_handlers.offset = XtOffsetOf(Pair, std);

    pair_handlers.dtor_obj                = zend_objects_destroy_object;
    pair_handlers.free_obj                = pair_free_object;
    pair_handlers.clone_obj               = pair_clone_object;
    pair_handlers.cast_object             = ds_default_cast_object;
    pair_handlers.get_debug_info          = pair_get_debug_info;
    pair_handlers.count_elements          = pair_count_elements;

    pair_handlers.read_property           = pair_read_property;
    pair_handlers.write_property          = pair_write_property;
    pair_handlers.has_property            = pair_has_property;
    pair_handlers.unset_property          = pair_unset_property;
}
