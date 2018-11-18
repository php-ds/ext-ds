#include "php_pair_handlers.h"
#include "php_common_handlers.h"
#include "../objects/php_pair.h"

zend_object_handlers php_pair_handlers;

static void php_ds_pair_unset_property(zval *object, zval *offset, void **cache_slot)
{
    if (EXPECTED(Z_TYPE_P(offset) == IS_STRING)) {
        if (ZVAL_EQUALS_STRING(offset, "key")) {
            zend_update_property_null(Z_OBJCE_P(object), object, Z_STRVAL_P(offset), Z_STRLEN_P(offset));
        } else if (ZVAL_EQUALS_STRING(offset, "value")) {
            zend_update_property_null(Z_OBJCE_P(object), object, Z_STRVAL_P(offset), Z_STRLEN_P(offset));
        }
    }
}

static int php_ds_pair_count_elements(zval *object, zend_long *count)
{
    *count = 2;
    return SUCCESS;
}

static zend_object *php_ds_pair_clone_object(zval *object)
{
    return php_ds_pair_create_clone(Z_DS_PAIR_P(object));
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
