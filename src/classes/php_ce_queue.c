#include "../common.h"
#include "../internal/php_queue.h"
#include "../iterators/php_queue_iterator.h"
#include "../handlers/php_queue_handlers.h"
#include "php_ce_collection.h"
#include "php_ce_queue.h"

#define METHOD(name) PHP_METHOD(Queue, name)

zend_class_entry *queue_ce;

ARGINFO_OPTIONAL_ZVAL(__construct, values)
METHOD(__construct)
{
    PARSE_OPTIONAL_ZVAL(values);

    if (values) {
        if (Z_TYPE_P(values) == IS_LONG) {
            queue_user_allocate(THIS_QUEUE(), Z_LVAL_P(values));
        } else {
            queue_push_all(THIS_QUEUE(), values);
        }
    }
}

ARGINFO_LONG(allocate, capacity)
METHOD(allocate)
{
    PARSE_LONG(capacity);
    queue_user_allocate(THIS_QUEUE(), capacity);
}

ARGINFO_NONE_RETURN_LONG(capacity)
METHOD(capacity)
{
    PARSE_NONE;
    RETURN_LONG(queue_capacity(THIS_QUEUE()));
}

ARGINFO_VARIADIC_ZVAL(push, values)
METHOD(push)
{
    PARSE_VARIADIC_ZVAL();
    queue_push(THIS_QUEUE(), argc, argv);
}

ARGINFO_ZVAL(pushAll, values)
METHOD(pushAll)
{
    PARSE_ZVAL(values);
    queue_push_all(THIS_QUEUE(), values);
}

ARGINFO_NONE(pop)
METHOD(pop)
{
    PARSE_NONE;
    queue_pop(THIS_QUEUE(), return_value);
}

ARGINFO_NONE(peek)
METHOD(peek)
{
    PARSE_NONE;
    RETURN_ZVAL_COPY(queue_peek(THIS_QUEUE()));
}

ARGINFO_NONE_RETURN_DS(copy, Queue)
METHOD(copy)
{
    PARSE_NONE;
    RETURN_OBJ(queue_create_clone(THIS_QUEUE()));
}

ARGINFO_NONE_RETURN_LONG(count)
METHOD(count)
{
    PARSE_NONE;
    RETURN_LONG(QUEUE_SIZE(THIS_QUEUE()));
}

ARGINFO_NONE(clear)
METHOD(clear)
{
    PARSE_NONE;
    queue_clear(THIS_QUEUE());
}

ARGINFO_NONE_RETURN_ARRAY(toArray)
METHOD(toArray)
{
    PARSE_NONE;
    queue_to_array(THIS_QUEUE(), return_value);
}

ARGINFO_NONE_RETURN_BOOL(isEmpty)
METHOD(isEmpty)
{
    PARSE_NONE;
    RETURN_BOOL(QUEUE_SIZE(THIS_QUEUE()) == 0);
}

ARGINFO_NONE(jsonSerialize)
METHOD(jsonSerialize)
{
    PARSE_NONE;
    queue_to_array(THIS_QUEUE(), return_value);
}

void register_queue()
{
    zend_class_entry ce;

    zend_function_entry methods[] = {
        COLLECTION_ME(Queue, __construct)
        COLLECTION_ME(Queue, allocate)
        COLLECTION_ME(Queue, capacity)
        COLLECTION_ME(Queue, peek)
        COLLECTION_ME(Queue, pop)
        COLLECTION_ME(Queue, push)
        COLLECTION_ME(Queue, pushAll)

        COLLECTION_ME_LIST(Queue)
        PHP_FE_END
    };

    INIT_CLASS_ENTRY(ce, COLLECTION_NS(Queue), methods);

    queue_ce = zend_register_internal_class(&ce);
    queue_ce->ce_flags      |= ZEND_ACC_FINAL;
    queue_ce->create_object  = queue_create_object;
    queue_ce->get_iterator   = queue_get_iterator;
    queue_ce->serialize      = queue_serialize;
    queue_ce->unserialize    = queue_unserialize;

    zend_class_implements(queue_ce, 1, collection_ce);
    register_queue_handlers();
}
