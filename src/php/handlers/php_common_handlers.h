#ifndef PHP_COMMON_HANDLERS_H
#define PHP_COMMON_HANDLERS_H

#include "php.h"

/**
 * Default object cast handler.
 */
int php_ds_default_cast_object
#if PHP_VERSION_ID >= 80000
(zend_object *obj, zval *return_value, int type);
#else
(zval *obj, zval *return_value, int type);
#endif

zval *php_ds_read_dimension_by_key_not_supported
#if PHP_VERSION_ID >= 80000
(zend_object *obj, zval *offset, int type, zval *rv);
#else
(zval *obj, zval *offset, int type, zval *rv);
#endif

int php_ds_has_dimension_by_key_not_supported
#if PHP_VERSION_ID >= 80000
(zend_object *obj, zval *offset, int check_empty);
#else
(zval *obj, zval *offset, int check_empty);
#endif

void php_ds_unset_dimension_by_key_not_supported
#if PHP_VERSION_ID >= 80000
(zend_object *obj, zval *offset);
#else
(zval *obj, zval *offset);
#endif

#endif