#ifndef DS_VECTOR_CE_H
#define DS_VECTOR_CE_H

#include "php.h"
#include "../../common.h"
#include "../arginfo.h"

extern zend_class_entry *php_ds_vector_ce;

ARGINFO_OPTIONAL_ZVAL(Vector___construct, values);
ARGINFO_NONE_RETURN_OBJ(Vector_getIterator, Traversable);

void php_ds_register_vector();

#endif
