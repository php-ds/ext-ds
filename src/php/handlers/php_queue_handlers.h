#ifndef DS_QUEUE_HANDLERS_H
#define DS_QUEUE_HANDLERS_H

#include "php.h"

extern zend_object_handlers php_queue_handlers;

void register_queue_handlers();

#endif
