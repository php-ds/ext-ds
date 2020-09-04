#ifndef PHP_COMMON_HANDLERS_H
#define PHP_COMMON_HANDLERS_H

#include "php.h"

/**
 * Default object cast handler.
 */
#if PHP_VERSION_ID >= 80000
int php_ds_default_cast_object(zend_object *obj, zval *return_value, int type);
#else
int php_ds_default_cast_object(zval *obj, zval *return_value, int type);
#endif

#endif
