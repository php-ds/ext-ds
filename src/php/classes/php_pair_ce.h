#ifndef DS_PAIR_CE_H
#define DS_PAIR_CE_H

#include "php.h"
#include "ext/json/php_json.h"
#include "../../common.h"
#include "../arginfo.h"

extern zend_class_entry *php_ds_pair_ce;

ARGINFO_OPTIONAL_ZVAL_OPTIONAL_ZVAL(    Pair___construct, key, value);
ARGINFO_NONE_RETURN_DS(                 Pair_copy, Pair);
ARGINFO_NONE_RETURN_ARRAY(              Pair_toArray);
ARGINFO_NONE(                           Pair_jsonSerialize);

void php_ds_register_pair();

#endif
