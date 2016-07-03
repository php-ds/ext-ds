#ifndef DS_STACK_HANDLERS_H
#define DS_STACK_HANDLERS_H

#include "php.h"

extern zend_object_handlers stack_handlers;

void php_ds_register_php_ds_stack_handlers();

#endif
