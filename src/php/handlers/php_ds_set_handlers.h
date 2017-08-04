#ifndef PHP_DS_SET_HANDLERS_H
#define PHP_DS_SET_HANDLERS_H

#include "php.h"

extern zend_object_handlers php_ds_set_handlers;

void php_ds_register_set_handlers();

#endif
