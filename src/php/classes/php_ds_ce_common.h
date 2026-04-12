#ifndef PHP_DS_CE_COMMON_H
#define PHP_DS_CE_COMMON_H

#include "../../common.h"
#include "../arginfo.h"

/**
 * Common collection methods shared by all types (Seq, Map, Set, Heap).
 * These were previously part of the Collection interface.
 */
#define PHP_DS_COLLECTION_ME(cls, name) \
    PHP_ME(cls, name, arginfo_Collection_##name, ZEND_ACC_PUBLIC)

#define PHP_DS_COLLECTION_ME_LIST(cls) \
    PHP_DS_COLLECTION_ME(cls, clear) \
    PHP_DS_COLLECTION_ME(cls, copy) \
    PHP_DS_COLLECTION_ME(cls, count) \
    PHP_DS_COLLECTION_ME(cls, isEmpty) \
    PHP_DS_COLLECTION_ME(cls, jsonSerialize) \
    PHP_DS_COLLECTION_ME(cls, toArray) \
    PHP_DS_COLLECTION_ME(cls, __serialize) \
    PHP_DS_COLLECTION_ME(cls, __unserialize)

ARGINFO_NONE(               Collection_clear);
ARGINFO_NONE(               Collection_copy);
ARGINFO_NONE_RETURN_LONG(   Collection_count);
ARGINFO_NONE_RETURN_BOOL(   Collection_isEmpty);
ARGINFO_NONE_RETURN_TYPE(   Collection_jsonSerialize, IS_MIXED);
ARGINFO_NONE_RETURN_ARRAY(  Collection_toArray);
ARGINFO_NONE_RETURN_ARRAY(  Collection___serialize);
ARGINFO_ZVAL(               Collection___unserialize, data);

#endif
