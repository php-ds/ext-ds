#include "php_ds_common_handlers.h"
#include "zend_smart_str.h"
#include "../../ds_common.h"

int php_ds_common_cast_object(zval *obj, zval *return_value, int type)
{
    switch (type) {
        case IS_STRING: {
            smart_str buffer = {0};

            smart_str_appendl(&buffer, "object(", 7);
            smart_str_append (&buffer, Z_OBJCE_P(obj)->name);
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

zval *php_ds_common_read_dimension(zval *obj, zval *offset, int type, zval *rv)
{
    ARRAY_ACCESS_NOT_SUPPORTED();
    return NULL;
}

void php_ds_common_write_dimension(zval *obj, zval *offset, zval *value)
{
    ARRAY_ACCESS_NOT_SUPPORTED();
}

int php_ds_common_has_dimension(zval *obj, zval *offset, int check_empty)
{
    ARRAY_ACCESS_NOT_SUPPORTED();
    return 0;
}

void php_ds_common_unset_dimension(zval *obj, zval *offset)
{
    ARRAY_ACCESS_NOT_SUPPORTED();
}
