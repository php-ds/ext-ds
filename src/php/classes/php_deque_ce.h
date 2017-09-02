#ifndef DS_DEQUE_CE_H
#define DS_DEQUE_CE_H

#include "php.h"

extern zend_class_entry *php_ds_deque_ce;

ARGINFO_OPTIONAL_ZVAL(          Deque___construct, values);

void php_ds_register_deque();

#endif
