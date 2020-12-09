#include "../../common.h"
#include "php_common_handlers.h"
#include "zend_smart_str.h"

int php_ds_default_cast_object
#if PHP_VERSION_ID >= 80000
(zend_object *obj, zval *return_value, int type) {
    zend_class_entry *ce = obj->ce;
#else
(zval *obj, zval *return_value, int type) {  
    zend_class_entry *ce = Z_OBJCE_P(obj);
#endif
    switch (type) {
        case IS_STRING: {
            smart_str buffer = {0};

            smart_str_appendl(&buffer, "object(", 7);
            smart_str_append (&buffer, ce->name);
            smart_str_appendc(&buffer, ')');

            smart_str_0(&buffer);
            ZVAL_STR(return_value, buffer.s);
            return SUCCESS;
        }
        case _IS_BOOL: {
            ZVAL_TRUE(return_value);
            return SUCCESS;
        }
    }

    return FAILURE;
}

zval *php_ds_read_dimension_by_key_not_supported
#if PHP_VERSION_ID >= 80000
(zend_object *obj, zval *offset, int type, zval *rv) {
#else
(zval *obj, zval *offset, int type, zval *rv) {
#endif
    ARRAY_ACCESS_BY_KEY_NOT_SUPPORTED();
    return NULL;
}

int php_ds_has_dimension_by_key_not_supported
#if PHP_VERSION_ID >= 80000
(zend_object *obj, zval *offset, int check_empty) {
#else
(zval *obj, zval *offset, int check_empty) {
#endif
    ARRAY_ACCESS_BY_KEY_NOT_SUPPORTED();
    return 0;
}

void php_ds_unset_dimension_by_key_not_supported
#if PHP_VERSION_ID >= 80000
(zend_object *obj, zval *offset) {
#else
(zval *obj, zval *offset) {
#endif
    ARRAY_ACCESS_BY_KEY_NOT_SUPPORTED();
}
