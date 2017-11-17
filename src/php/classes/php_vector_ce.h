#ifndef DS_VECTOR_CE_H
#define DS_VECTOR_CE_H

#include "php.h"
#include "../../common.h"
#include "../arginfo.h"

extern zend_class_entry *php_ds_vector_ce;

ARGINFO_OPTIONAL_ZVAL(Vector___construct, values);

void php_ds_register_vector();

#endif
