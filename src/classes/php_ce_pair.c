#include "../common.h"
#include "../internal/php_pair.h"
#include "../handlers/php_pair_handlers.h"
#include "php_ce_pair.h"

#define METHOD(name) PHP_METHOD(Pair, name)

zend_class_entry *pair_ce;

ARGINFO_ZVAL_ZVAL(__construct, key, value)
METHOD(__construct)
{
    Pair *pair;
    PARSE_ZVAL_ZVAL(key, value);

    pair = THIS_PAIR();

    if ( ! Z_ISUNDEF(pair->key)) {
        RECONSTRUCTION_NOT_ALLOWED();
        return;
    }

    ZVAL_COPY(&pair->key, key);
    ZVAL_COPY(&pair->value, value);
}

ARGINFO_NONE_RETURN_ARRAY(toArray)
METHOD(toArray)
{
    PARSE_NONE;
    pair_to_array(THIS_PAIR(), return_value);
}

ARGINFO_NONE(jsonSerialize)
METHOD(jsonSerialize)
{
    PARSE_NONE;
    pair_to_array(THIS_PAIR(), return_value);
}

void register_pair()
{
    zend_class_entry ce;

    zend_function_entry methods[] = {
        COLLECTION_ME(Pair, __construct)
        COLLECTION_ME(Pair, jsonSerialize)
        COLLECTION_ME(Pair, toArray)
        PHP_FE_END
    };

    INIT_CLASS_ENTRY(ce, COLLECTION_NS(Pair), methods);
    pair_ce = zend_register_internal_class(&ce);

    pair_ce->ce_flags         |= ZEND_ACC_FINAL;
    pair_ce->create_object     = pair_create_object;
    pair_ce->serialize         = pair_serialize;
    pair_ce->unserialize       = pair_unserialize;

    zend_class_implements(pair_ce, 1, php_json_serializable_ce);
    register_pair_handlers();
}
