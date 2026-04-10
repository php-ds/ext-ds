#ifndef PHP_COMMON_HANDLERS_H
#define PHP_COMMON_HANDLERS_H

#include "php.h"

/**
 * Default object cast handler.
 */
int php_ds_default_cast_object
(zend_object *obj, zval *return_value, int type);

zval *php_ds_read_dimension_by_key_not_supported
(zend_object *obj, zval *offset, int type, zval *rv);

int php_ds_has_dimension_by_key_not_supported
(zend_object *obj, zval *offset, int check_empty);

void php_ds_unset_dimension_by_key_not_supported
(zend_object *obj, zval *offset);

#endif