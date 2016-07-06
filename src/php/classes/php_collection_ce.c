#include "../../common.h"
#include "php_collection_ce.h"

zend_class_entry *collection_ce;

#define COLLECTION_ABSTRACT_ME(name) \
    PHP_ABSTRACT_ME(Collection, name, arginfo_Collection_##name)

void register_collection()
{
    zend_class_entry ce;

    zend_function_entry methods[] = {
        COLLECTION_ABSTRACT_ME(clear)
        COLLECTION_ABSTRACT_ME(copy)
        COLLECTION_ABSTRACT_ME(isEmpty)
        COLLECTION_ABSTRACT_ME(toArray)
        PHP_FE_END
    };

    INIT_CLASS_ENTRY(ce, DS_NS(Collection), methods);
    collection_ce = zend_register_internal_interface(&ce);
    zend_class_implements(collection_ce, 3,
        zend_ce_traversable,        // Traversable
        spl_ce_Countable,           // Countable
        php_json_serializable_ce    // Serializable
    );
}
