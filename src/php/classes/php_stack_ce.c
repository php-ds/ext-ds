#include "../../common.h"

#include "../parameters.h"
#include "../arginfo.h"

#include "../objects/php_stack.h"

#include "../iterators/php_stack_iterator.h"
#include "../handlers/php_stack_handlers.h"

#include "php_collection_ce.h"
#include "php_stack_ce.h"

#define METHOD(name) PHP_METHOD(Stack, name)

zend_class_entry *php_ds_stack_ce;

ARGINFO_OPTIONAL_ZVAL(__construct, values)
METHOD(__construct)
{
    PARSE_OPTIONAL_ZVAL(values);

    if (values) {
        if (Z_TYPE_P(values) == IS_LONG) {
            ds_stack_allocate(THIS_DS_STACK(), Z_LVAL_P(values));
        } else {
            ds_stack_push_all(THIS_DS_STACK(), values);
        }
    }
}

ARGINFO_LONG(allocate, capacity)
METHOD(allocate)
{
    PARSE_LONG(capacity);
    ds_stack_allocate(THIS_DS_STACK(), capacity);
}

ARGINFO_NONE_RETURN_LONG(capacity)
METHOD(capacity)
{
    PARSE_NONE;
    RETURN_LONG(DS_STACK_CAPACITY(THIS_DS_STACK()));
}

ARGINFO_VARIADIC_ZVAL(push, values)
METHOD(push)
{
    PARSE_VARIADIC_ZVAL();
    ds_stack_push(THIS_DS_STACK(), argc, argv);
}

ARGINFO_ZVAL(pushAll, values)
METHOD(pushAll)
{
    PARSE_ZVAL(values);
    ds_stack_push_all(THIS_DS_STACK(), values);
}

ARGINFO_NONE(pop)
METHOD(pop)
{
    PARSE_NONE;
    ds_stack_pop(THIS_DS_STACK(), return_value);
}

ARGINFO_NONE(peek)
METHOD(peek)
{
    PARSE_NONE;
    RETURN_ZVAL_COPY(ds_stack_peek(THIS_DS_STACK()));
}

ARGINFO_NONE_RETURN_LONG(count)
METHOD(count)
{
    PARSE_NONE;
    RETURN_LONG(DS_STACK_SIZE(THIS_DS_STACK()));
}

ARGINFO_NONE_RETURN_DS(copy, ds_stack_t)
METHOD(copy)
{
    PARSE_NONE;
    RETURN_OBJ(php_ds_stack_create_clone(THIS_DS_STACK()));
}

ARGINFO_NONE(clear)
METHOD(clear)
{
    PARSE_NONE;
    ds_stack_clear(THIS_DS_STACK());
}

ARGINFO_NONE_RETURN_ARRAY(toArray)
METHOD(toArray)
{
    PARSE_NONE;
    ds_stack_to_array(THIS_DS_STACK(), return_value);
}

ARGINFO_NONE_RETURN_BOOL(isEmpty)
METHOD(isEmpty)
{
    PARSE_NONE;
    RETURN_BOOL(DS_STACK_IS_EMPTY(THIS_DS_STACK()));
}

ARGINFO_NONE(jsonSerialize)
METHOD(jsonSerialize)
{
    PARSE_NONE;
    ds_stack_to_array(THIS_DS_STACK(), return_value);
}

void php_ds_register_stack()
{
    zend_class_entry ce;

    zend_function_entry methods[] = {
        COLLECTION_ME(Stack, __construct)
        COLLECTION_ME(Stack, allocate)
        COLLECTION_ME(Stack, capacity)
        COLLECTION_ME(Stack, peek)
        COLLECTION_ME(Stack, pop)
        COLLECTION_ME(Stack, push)
        COLLECTION_ME(Stack, pushAll)

        COLLECTION_ME_LIST(Stack)
        PHP_FE_END
    };

    INIT_CLASS_ENTRY(ce, DS_NS(Stack), methods);

    php_ds_stack_ce = zend_register_internal_class(&ce);
    php_ds_stack_ce->ce_flags      |= ZEND_ACC_FINAL;
    php_ds_stack_ce->create_object  = php_ds_stack_create_object;
    php_ds_stack_ce->get_iterator   = php_ds_stack_get_iterator;
    php_ds_stack_ce->serialize      = php_ds_stack_serialize;
    php_ds_stack_ce->unserialize    = php_ds_stack_unserialize;

    zend_class_implements(php_ds_stack_ce, 1, collection_ce);
    php_register_ds_stack_handlers();
}
