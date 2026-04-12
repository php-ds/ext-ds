#include "../../common.h"
#include "../parameters.h"
#include "../arginfo.h"
#include "../objects/php_pair.h"
#include "../handlers/php_pair_handlers.h"
#include "php_pair_ce.h"

#define METHOD(name) PHP_METHOD(Pair, name)

zend_class_entry *php_ds_pair_ce;

METHOD(__construct)
{
    PARSE_ZVAL_ZVAL(key, value);
    {
        php_ds_pair_t *pair = THIS_DS_PAIR();
        php_ds_pair_set_key(pair, key);
        php_ds_pair_set_value(pair, value);
    }
}

METHOD(toArray)
{
    PARSE_NONE;
    php_ds_pair_to_array(THIS_DS_PAIR(), return_value);
}

METHOD(jsonSerialize)
{
    PARSE_NONE;
    php_ds_pair_to_array(THIS_DS_PAIR(), return_value);
}

METHOD(__serialize)
{
    PARSE_NONE;
    php_ds_pair_to_array(THIS_DS_PAIR(), return_value);
}

METHOD(__unserialize)
{
    PARSE_ZVAL(data);
    php_ds_pair_t *pair = THIS_DS_PAIR();

    zval *key = zend_hash_str_find(Z_ARRVAL_P(data), "key", sizeof("key") - 1);
    zval *value = zend_hash_str_find(Z_ARRVAL_P(data), "value", sizeof("value") - 1);

    if (key) {
        php_ds_pair_set_key(pair, key);
    }
    if (value) {
        php_ds_pair_set_value(pair, value);
    }
}

void php_ds_register_pair()
{
    zend_class_entry ce;

    zend_function_entry methods[] = {
        PHP_DS_ME(Pair, __construct)
        PHP_DS_ME(Pair, jsonSerialize)
        PHP_DS_ME(Pair, toArray)
        PHP_DS_ME(Pair, __serialize)
        PHP_DS_ME(Pair, __unserialize)
        PHP_FE_END
    };

    INIT_CLASS_ENTRY(ce, PHP_DS_NS(Pair), methods);
    php_ds_pair_ce = zend_register_internal_class(&ce);

    php_ds_pair_ce->ce_flags         |= ZEND_ACC_FINAL | ZEND_ACC_READONLY_CLASS;
    php_ds_pair_ce->create_object     = php_ds_pair_create_object;
    php_ds_pair_ce->serialize         = php_ds_pair_serialize;
    php_ds_pair_ce->unserialize       = php_ds_pair_unserialize;

    zval default_value;
    ZVAL_UNDEF(&default_value);

    zend_declare_typed_property(php_ds_pair_ce,
        zend_string_init("key", sizeof("key") - 1, 1),
        &default_value, ZEND_ACC_PUBLIC | ZEND_ACC_READONLY,
        NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_ANY));

    zend_declare_typed_property(php_ds_pair_ce,
        zend_string_init("value", sizeof("value") - 1, 1),
        &default_value, ZEND_ACC_PUBLIC | ZEND_ACC_READONLY,
        NULL, (zend_type) ZEND_TYPE_INIT_MASK(MAY_BE_ANY));

    zend_class_implements(php_ds_pair_ce, 1, php_json_serializable_ce);
    php_ds_register_pair_handlers();
}
