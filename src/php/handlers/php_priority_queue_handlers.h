#ifndef DS_PRIORITY_QUEUE_HANDLERS_H
#define DS_PRIORITY_QUEUE_HANDLERS_H

#include "php.h"

extern zend_object_handlers php_priority_queue_handlers;

void register_priority_queue_handlers();

#endif
