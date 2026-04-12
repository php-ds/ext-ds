#include "../../common.h"
#include "../arginfo.h"
#include "php_key_ce.h"

zend_class_entry *key_ce;

ARGINFO_NONE(Key_hash)
ARGINFO_ZVAL_RETURN_BOOL(Key_equals, other)

static zend_function_entry key_methods[] = {
    PHP_ABSTRACT_ME(Key, hash, arginfo_Key_hash)
    PHP_ABSTRACT_ME(Key, equals, arginfo_Key_equals)
    PHP_FE_END
};

void php_ds_register_key()
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, PHP_DS_NS(Key), key_methods);
    key_ce = zend_register_internal_interface(&ce);
}
