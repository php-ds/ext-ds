#ifndef PHP_COMMON_HANDLERS_H
#define PHP_COMMON_HANDLERS_H

#include "php.h"

/**
 * Default object cast handler.
 */
int php_ds_default_cast_object(zval *obj, zval *return_value, int type);

#endif
