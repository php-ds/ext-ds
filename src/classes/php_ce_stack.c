#include "../common.h"
#include "../internal/php_stack.h"
#include "../iterators/php_stack_iterator.h"
#include "../handlers/php_stack_handlers.h"
#include "php_ce_collection.h"
#include "php_ce_stack.h"

#define METHOD(name) PHP_METHOD(Stack, name)

zend_class_entry *stack_ce;

ARGINFO_OPTIONAL_ZVAL(__construct, values)
METHOD(__construct)
{
    PARSE_OPTIONAL_ZVAL(values);

    if (values) {
        if (Z_TYPE_P(values) == IS_LONG) {
            stack_user_allocate(THIS_STACK(), Z_LVAL_P(values));
        } else {
            stack_push_all(THIS_STACK(), values);
        }
    }
}

ARGINFO_LONG(allocate, capacity)
METHOD(allocate)
{
    PARSE_LONG(capacity);
    stack_user_allocate(THIS_STACK(), capacity);
}

ARGINFO_NONE_RETURN_LONG(capacity)
METHOD(capacity)
{
    PARSE_NONE;
    RETURN_LONG(stack_capacity(THIS_STACK()))
}

ARGINFO_VARIADIC_ZVAL(push, values)
METHOD(push)
{
    PARSE_VARIADIC_ZVAL();
    stack_push(THIS_STACK(), argc, argv);
}

ARGINFO_ZVAL(pushAll, values)
METHOD(pushAll)
{
    PARSE_ZVAL(values);
    stack_push_all(THIS_STACK(), values);
}

ARGINFO_NONE(pop)
METHOD(pop)
{
    PARSE_NONE;
    stack_pop(THIS_STACK(), return_value);
}

ARGINFO_NONE(peek)
METHOD(peek)
{
    PARSE_NONE;
    RETURN_ZVAL_COPY(stack_peek(THIS_STACK()));
}

ARGINFO_NONE_RETURN_LONG(count)
METHOD(count)
{
    PARSE_NONE;
    RETURN_LONG(STACK_SIZE(THIS_STACK()));
}

ARGINFO_NONE_RETURN_COLLECTION(copy, Stack)
METHOD(copy)
{
    PARSE_NONE;
    RETURN_OBJ(stack_create_clone(THIS_STACK()));
}

ARGINFO_NONE(clear)
METHOD(clear)
{
    PARSE_NONE;
    stack_clear(THIS_STACK());
}

ARGINFO_NONE_RETURN_ARRAY(toArray)
METHOD(toArray)
{
    PARSE_NONE;
    stack_to_array(THIS_STACK(), return_value);
}

ARGINFO_NONE_RETURN_BOOL(isEmpty)
METHOD(isEmpty)
{
    PARSE_NONE;
    RETURN_BOOL(STACK_IS_EMPTY(THIS_STACK()));
}

ARGINFO_NONE(jsonSerialize)
METHOD(jsonSerialize)
{
    PARSE_NONE;
    stack_to_array(THIS_STACK(), return_value);
}

void register_stack()
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

    INIT_CLASS_ENTRY(ce, COLLECTION_NS(Stack), methods);

    stack_ce = zend_register_internal_class(&ce);
    stack_ce->ce_flags      |= ZEND_ACC_FINAL;
    stack_ce->create_object  = stack_create_object;
    stack_ce->get_iterator   = stack_get_iterator;
    stack_ce->serialize      = stack_serialize;
    stack_ce->unserialize    = stack_unserialize;

    zend_class_implements(stack_ce, 1, collection_ce);
    register_stack_handlers();
}
