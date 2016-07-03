#include "../../common.h"

zend_class_entry *hashable_ce;

ARGINFO_NONE_RETURN_STRING(hash);
ARGINFO_ZVAL_RETURN_BOOL(equals, obj);

static zend_function_entry hashable_methods[] = {
    PHP_ABSTRACT_ME(Hashable, hash,   arginfo_hash)
    PHP_ABSTRACT_ME(Hashable, equals, arginfo_equals)
    PHP_FE_END
};

void register_hashable()
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, DS_NS(Hashable), hashable_methods);
    hashable_ce = zend_register_internal_interface(&ce);
}
