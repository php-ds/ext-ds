#ifndef DS_HEAP_CE_H
#define DS_HEAP_CE_H

#include "php.h"
#include "../../common.h"
#include "../arginfo.h"

extern zend_class_entry *php_ds_heap_ce;

ARGINFO_OPTIONAL_ZVAL_OPTIONAL_CALLABLE(Heap___construct, values, comparator);
ARGINFO_LONG(                       Heap_allocate, capacity);
ARGINFO_NONE_RETURN_LONG(           Heap_capacity);
ARGINFO_VARIADIC_ZVAL(              Heap_push, values);
ARGINFO_NONE(                       Heap_pop);
ARGINFO_NONE(                       Heap_peek);
ARGINFO_NONE_RETURN_OBJ(            Heap_getIterator, Traversable);

void php_ds_register_heap();

#endif
