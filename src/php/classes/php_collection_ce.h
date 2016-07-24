#ifndef DS_COLLECTION_CE_H
#define DS_COLLECTION_CE_H

#include "../../common.h"
#include "../arginfo.h"

extern zend_class_entry *collection_ce;

#define PHP_DS_COLLECTION_INTERFACE_ME(cls, name) \
    PHP_ME(cls, name, arginfo_Collection_##name, ZEND_ACC_PUBLIC)

#define PHP_DS_COLLECTION_ME_LIST(cls) \
PHP_DS_COLLECTION_INTERFACE_ME(cls, clear) \
PHP_DS_COLLECTION_INTERFACE_ME(cls, count) \
PHP_DS_COLLECTION_INTERFACE_ME(cls, isEmpty) \
PHP_DS_COLLECTION_INTERFACE_ME(cls, jsonSerialize) \
PHP_DS_COLLECTION_INTERFACE_ME(cls, toArray)

ARGINFO_NONE(               Collection_clear);
ARGINFO_NONE_RETURN_LONG(   Collection_count);
ARGINFO_NONE_RETURN_BOOL(   Collection_isEmpty);
ARGINFO_NONE(               Collection_jsonSerialize);
ARGINFO_NONE_RETURN_ARRAY(  Collection_toArray);

void php_ds_register_collection();

#endif
