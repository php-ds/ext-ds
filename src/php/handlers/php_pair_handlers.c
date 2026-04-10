#include "php_pair_handlers.h"
#include "php_common_handlers.h"
#include "../objects/php_pair.h"

zend_object_handlers php_pair_handlers;

static void php_ds_pair_unset_property
(zend_object *obj, zend_string *offset, void **cache_slot) {
    if (zend_string_equals_literal(offset, "key") || zend_string_equals_literal(offset, "value")) {
        zend_update_property_null(obj->ce, obj, ZSTR_VAL(offset), ZSTR_LEN(offset));
    }
}

static int php_ds_pair_count_elements
(zend_object *obj, zend_long *count) {
    *count = 2;
    return SUCCESS;
}

static zend_object *php_ds_pair_clone_object
(zend_object *obj) {
    return php_ds_pair_create_clone((php_ds_pair_t*)obj);
}

void php_ds_register_pair_handlers()
{
    memcpy(&php_pair_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    php_pair_handlers.offset = XtOffsetOf(php_ds_pair_t, std);

    php_pair_handlers.clone_obj               = php_ds_pair_clone_object;
    php_pair_handlers.cast_object             = php_ds_default_cast_object;
    php_pair_handlers.count_elements          = php_ds_pair_count_elements;
    php_pair_handlers.unset_property          = php_ds_pair_unset_property;
}
