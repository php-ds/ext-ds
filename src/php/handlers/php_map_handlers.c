#include "php_map_handlers.h"
#include "php_common_handlers.h"
#include "../../ds/ds_map.h"
#include "../objects/php_map.h"

zend_object_handlers php_map_handlers;

static zval *php_ds_map_read_dimension(zval *obj, zval *offset, int type, zval *rv)
{
    if (offset == NULL) {
        ARRAY_ACCESS_PUSH_NOT_SUPPORTED();
        return NULL;

    } else {
        ds_map_t *map = Z_DS_MAP_P(obj);
        zval *value;

        // Dereference the offset if it's a reference.
        ZVAL_DEREF(offset);

        // `??`
        if (type == BP_VAR_IS) {
            if ( ! ds_htable_isset(map->table, offset, 0)) {
                return &EG(uninitialized_zval);;
            }
        }

        // Get the value from the map.
        value = ds_map_get(map, offset, NULL);

        // If we're accessing by reference we have to create a reference.
        // This is for access like $map[$a][$b] = $c
        if (value && type != BP_VAR_R) {
            ZVAL_MAKE_REF(value);
        }

        return value;
    }
}

static void php_ds_map_write_dimension(zval *obj, zval *offset, zval *value)
{
    ds_map_t *map = Z_DS_MAP_P(obj);

    if (offset == NULL) {
        ARRAY_ACCESS_PUSH_NOT_SUPPORTED();
        return;
    }

    ZVAL_DEREF(offset);

    ds_htable_put(map->table, offset, value);
}

static int php_ds_map_has_dimension(zval *obj, zval *offset, int check_empty)
{
    ds_map_t *map = Z_DS_MAP_P(obj);

    ZVAL_DEREF(offset);

    return ds_htable_isset(map->table, offset, check_empty);
}

static void php_ds_map_unset_dimension(zval *obj, zval *offset)
{
    ds_map_t *map = Z_DS_MAP_P(obj);

    ZVAL_DEREF(offset);

    ds_htable_remove(map->table, offset, NULL);
}

static int php_ds_map_count_elements(zval *obj, zend_long *count)
{
    *count = DS_MAP_SIZE(Z_DS_MAP_P(obj));
    return SUCCESS;
}

static void php_ds_map_free_object(zend_object *object)
{
    php_ds_map_t *intern = (php_ds_map_t*) object;
    zend_object_std_dtor(&intern->std);
    ds_map_free(intern->map);
}

static HashTable *php_ds_map_get_debug_info(zval *obj, int *is_temp)
{
    *is_temp = 1;
    return ds_map_pairs_to_php_hashtable(Z_DS_MAP_P(obj));
}

static zend_object *php_ds_map_clone_obj(zval *obj)
{
    return php_ds_map_create_clone(Z_DS_MAP_P(obj));
}

static HashTable *php_ds_map_get_gc(zval *obj, zval **gc_data, int *gc_size)
{
    ds_map_t *map = Z_DS_MAP_P(obj);

    if (DS_MAP_IS_EMPTY(map)) {
        *gc_data  = NULL;
        *gc_size = 0;

    } else {
        *gc_data = (zval*) map->table->buckets;
        *gc_size = (int)   map->table->next * 2;
    }

    return NULL;
}

void php_ds_register_map_handlers()
{
    memcpy(&php_map_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    php_map_handlers.offset             = 0;
    php_map_handlers.dtor_obj           = zend_objects_destroy_object;
    php_map_handlers.get_gc             = php_ds_map_get_gc;
    php_map_handlers.free_obj           = php_ds_map_free_object;
    php_map_handlers.clone_obj          = php_ds_map_clone_obj;
    php_map_handlers.get_debug_info     = php_ds_map_get_debug_info;
    php_map_handlers.count_elements     = php_ds_map_count_elements;
    php_map_handlers.read_dimension     = php_ds_map_read_dimension;
    php_map_handlers.write_dimension    = php_ds_map_write_dimension;
    php_map_handlers.has_dimension      = php_ds_map_has_dimension;
    php_map_handlers.unset_dimension    = php_ds_map_unset_dimension;
    php_map_handlers.cast_object        = php_ds_default_cast_object;
    // php_map_handlers.get_properties      = ds_map_get_properties;
}
