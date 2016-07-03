#include "../../common.h"
#include "../../ds/ds_pair.h"
#include "../handlers/php_pair_handlers.h"
#include "php_ce_pair.h"

#define METHOD(name) PHP_METHOD(Pair, name)

zend_class_entry *pair_ce;

ARGINFO_OPTIONAL_ZVAL_OPTIONAL_ZVAL(__construct, key, value)
METHOD(__construct)
{
    PARSE_OPTIONAL_ZVAL_OPTIONAL_ZVAL(key, value);
    {
        Pair *pair = THIS_PAIR();

        if (key) {
            ZVAL_COPY(&pair->key, key);
        } else {
            ZVAL_NULL(&pair->key);
        }

        if (value) {
            ZVAL_COPY(&pair->value, value);
        } else {
            ZVAL_NULL(&pair->value);
        }
    }
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

    INIT_CLASS_ENTRY(ce, DS_NS(Pair), methods);
    pair_ce = zend_register_internal_class(&ce);

    pair_ce->ce_flags         |= ZEND_ACC_FINAL;
    pair_ce->create_object     = pair_create_object;
    pair_ce->serialize         = pair_serialize;
    pair_ce->unserialize       = pair_unserialize;

    zend_class_implements(pair_ce, 1, php_json_serializable_ce);
    register_pair_handlers();
}
