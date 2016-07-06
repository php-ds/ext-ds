#ifndef DS_COLLECTION_CE_H
#define DS_COLLECTION_CE_H

#include "../../common.h"
#include "../arginfo.h"

extern zend_class_entry *collection_ce;

#define COLLECTION_INTERFACE_ME(cls, name) \
    PHP_ME(cls, name, arginfo_Collection_##name, ZEND_ACC_PUBLIC)

#define COLLECTION_ME_LIST(cls) \
COLLECTION_INTERFACE_ME(cls, clear) \
COLLECTION_INTERFACE_ME(cls, copy) \
COLLECTION_INTERFACE_ME(cls, count) \
COLLECTION_INTERFACE_ME(cls, isEmpty) \
COLLECTION_INTERFACE_ME(cls, jsonSerialize) \
COLLECTION_INTERFACE_ME(cls, toArray)

ARGINFO_NONE(
    Collection_clear
);

ARGINFO_NONE_RETURN_DS(
    Collection_copy, Collection
);

ARGINFO_NONE_RETURN_LONG(
    Collection_count
);

ARGINFO_NONE_RETURN_BOOL(
    Collection_isEmpty
);

ARGINFO_NONE(
    Collection_jsonSerialize
);

ARGINFO_NONE_RETURN_ARRAY(
    Collection_toArray
);

void register_collection();

#endif
