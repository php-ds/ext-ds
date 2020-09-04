#ifndef DS_QUEUE_CE_H
#define DS_QUEUE_CE_H

#include "php.h"
#include "../../common.h"
#include "../arginfo.h"

extern zend_class_entry *php_ds_queue_ce;

ARGINFO_OPTIONAL_ZVAL(          Queue___construct, values);
ARGINFO_LONG(                   Queue_allocate, capacity);
ARGINFO_NONE_RETURN_LONG(       Queue_capacity);
ARGINFO_VARIADIC_ZVAL(          Queue_push, values);
ARGINFO_NONE(                   Queue_pop);
ARGINFO_NONE(                   Queue_peek);
ARGINFO_NONE_RETURN_OBJ(		Queue_getIterator, Traversable);

void php_ds_register_queue();

#endif
