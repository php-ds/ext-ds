#ifndef DS_HEAP_HANDLERS_H
#define DS_HEAP_HANDLERS_H

#include "php.h"

extern zend_object_handlers php_heap_handlers;

void php_ds_register_heap_handlers();

#endif
