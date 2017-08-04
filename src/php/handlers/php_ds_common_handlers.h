#ifndef PHP_DS_COMMON_HANDLERS_H
#define PHP_DS_COMMON_HANDLERS_H

#include "php.h"

/**
 * Default object cast handler.
 */
int php_ds_common_cast_object(zval *obj, zval *return_value, int type);

zval *php_ds_common_read_dimension(zval *obj, zval *offset, int type, zval *rv);
void  php_ds_common_write_dimension(zval *obj, zval *offset, zval *value);
int   php_ds_common_has_dimension(zval *obj, zval *offset, int check_empty);
void  php_ds_common_unset_dimension(zval *obj, zval *offset);

#endif
