#ifndef PHP_DS_QUEUE_HANDLERS_H
#define PHP_DS_QUEUE_HANDLERS_H

#include "php.h"

extern zend_object_handlers php_ds_queue_handlers;

void php_ds_register_queue_handlers();

#endif