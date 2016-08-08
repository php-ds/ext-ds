#ifndef DS_PRIORITY_QUEUE_CE_H
#define DS_PRIORITY_QUEUE_CE_H

#include "php.h"
#include "../../common.h"
#include "../arginfo.h"

extern zend_class_entry *php_ds_priority_queue_ce;

ARGINFO_NONE(                   PriorityQueue___construct);
ARGINFO_LONG(                   PriorityQueue_allocate, capacity);
ARGINFO_NONE_RETURN_LONG(       PriorityQueue_capacity);
ARGINFO_NONE_RETURN_DS(         PriorityQueue_copy, PriorityQueue);
ARGINFO_ZVAL_LONG(              PriorityQueue_push, value, priority);
ARGINFO_NONE(                   PriorityQueue_pop);
ARGINFO_NONE(                   PriorityQueue_peek);

void php_ds_register_priority_queue();

#endif
