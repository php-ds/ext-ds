#ifndef DS_PAIR_CE_H
#define DS_PAIR_CE_H

#include "php.h"
#include "../../common.h"
#include "../arginfo.h"

extern zend_class_entry *php_ds_pair_ce;

ARGINFO_ZVAL_ZVAL(                      Pair___construct, key, value);
ARGINFO_NONE_RETURN_ARRAY(              Pair_toArray);
ARGINFO_NONE_RETURN_TYPE(               Pair_jsonSerialize, IS_MIXED);
ARGINFO_NONE_RETURN_ARRAY(              Pair___serialize);
ARGINFO_ZVAL(                           Pair___unserialize, data);

void php_ds_register_pair();

#endif
