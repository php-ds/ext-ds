#include "php_map_handlers.h"
#include "php_common_handlers.h"
#include "../../ds/ds_map.h"

zend_object_handlers map_handlers;

static zval *map_read_dimension(zval *obj, zval *offset, int type, zval *return_value)
{
    Map *map = Z_MAP_P(obj);

    if (offset == NULL) {
        ARRAY_ACCESS_PUSH_NOT_SUPPORTED();
        return NULL;

    } else {

        zval *value = map_get(map, offset, NULL);

        if (value && type != BP_VAR_R) {
            ZVAL_MAKE_REF(value);
        }

        return value;
    }
}

static void map_write_dimension(zval *obj, zval *offset, zval *value)
{
    Map *map = Z_MAP_P(obj);

    if (offset == NULL) {
        ARRAY_ACCESS_PUSH_NOT_SUPPORTED();
        return;
    }

    ds_htable_put(map->table, offset, value);
}

static int map_has_dimension(zval *obj, zval *offset, int check_empty)
{
    Map *map = Z_MAP_P(obj);

    return ds_htable_isset(map->table, offset, check_empty);
}

static void map_unset_dimension(zval *obj, zval *offset)
{
    Map *map = Z_MAP_P(obj);
    ds_htable_remove(map->table, offset, NULL);
}

static int map_count_elements(zval *obj, zend_long *count)
{
    *count = MAP_SIZE(Z_MAP_P(obj));
    return SUCCESS;
}

static void map_free_object(zend_object *object)
{
    Map *intern = (Map*) object;

    zend_object_std_dtor(&intern->std);

    ds_htable_destroy(intern->table);
}

static HashTable *map_get_debug_info(zval *obj, int *is_temp)
{
    Map *map = Z_MAP_P(obj);
    *is_temp = 1;

    return ds_htable_pairs_to_php_ht(map->table);
}

static zend_object *map_clone_obj(zval *obj)
{
    Map *map = Z_MAP_P(obj);
    return map_create_clone(map);
}

void register_map_handlers()
{
    memcpy(&map_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    map_handlers.offset = XtOffsetOf(Map, std);

    map_handlers.dtor_obj            = zend_objects_destroy_object;
    map_handlers.free_obj            = map_free_object;
    map_handlers.clone_obj           = map_clone_obj;
    map_handlers.get_debug_info      = map_get_debug_info;
    map_handlers.count_elements      = map_count_elements;
    map_handlers.read_dimension      = map_read_dimension;
    map_handlers.write_dimension     = map_write_dimension;
    map_handlers.has_dimension       = map_has_dimension;
    map_handlers.unset_dimension     = map_unset_dimension;
    map_handlers.cast_object         = ds_default_cast_object;
    // map_handlers.get_properties      = map_get_properties;
}
