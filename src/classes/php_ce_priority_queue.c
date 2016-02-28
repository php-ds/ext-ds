#include "../common.h"
#include "../internal/php_priority_queue.h"
#include "../iterators/php_priority_queue_iterator.h"
#include "../handlers/php_priority_queue_handlers.h"
#include "php_ce_collection.h"
#include "php_ce_priority_queue.h"

#define METHOD(name) PHP_METHOD(PriorityQueue, name)

zend_class_entry *priority_queue_ce;

ARGINFO_NONE(__construct)
METHOD(__construct)
{
    /* Constructing using an iterator is difficult to do, and in most cases
       doesn't make any sense. Would you use the values? Default priority?
    */
    PARSE_NONE;
}

ARGINFO_LONG(allocate, capacity)
METHOD(allocate)
{
    PARSE_LONG(capacity);
    priority_queue_user_allocate(THIS_PRIORITY_QUEUE(), capacity);
}

ARGINFO_NONE_RETURN_LONG(capacity)
METHOD(capacity)
{
    PARSE_NONE;
    RETURN_LONG(priority_queue_capacity(THIS_PRIORITY_QUEUE()));
}

ARGINFO_NONE_RETURN_DS(copy, PriorityQueue)
METHOD(copy)
{
    PARSE_NONE;
    RETURN_OBJ(priority_queue_init_clone(THIS_PRIORITY_QUEUE()));
}

ARGINFO_ZVAL_LONG(push, value, priority)
METHOD(push)
{
    PARSE_ZVAL_LONG(value, priority);
    priority_queue_push(THIS_PRIORITY_QUEUE(), value, priority);
}

ARGINFO_NONE(pop)
METHOD(pop)
{
    PARSE_NONE;
    priority_queue_pop(THIS_PRIORITY_QUEUE(), return_value);
}

ARGINFO_NONE(peek)
METHOD(peek)
{
    PARSE_NONE;
    RETURN_ZVAL_COPY(priority_queue_peek(THIS_PRIORITY_QUEUE()));
}

ARGINFO_NONE_RETURN_BOOL(isEmpty)
METHOD(isEmpty)
{
    PARSE_NONE;
    RETURN_BOOL(PRIORITY_QUEUE_IS_EMPTY(THIS_PRIORITY_QUEUE()));
}

ARGINFO_NONE_RETURN_ARRAY(toArray)
METHOD(toArray)
{
    PARSE_NONE;
    priority_queue_to_array(THIS_PRIORITY_QUEUE(), return_value);
}

ARGINFO_NONE_RETURN_LONG(count)
METHOD(count)
{
    PARSE_NONE;
    RETURN_LONG(PRIORITY_QUEUE_SIZE(THIS_PRIORITY_QUEUE()));
}

ARGINFO_NONE(clear)
METHOD(clear)
{
    PARSE_NONE;
    priority_queue_clear(THIS_PRIORITY_QUEUE());
}

ARGINFO_NONE(jsonSerialize)
METHOD(jsonSerialize)
{
    PARSE_NONE;
    priority_queue_to_array(THIS_PRIORITY_QUEUE(), return_value);
}

void register_priority_queue()
{
    zend_class_entry ce;

    zend_function_entry methods[] = {
        COLLECTION_ME(PriorityQueue, __construct)
        COLLECTION_ME(PriorityQueue, allocate)
        COLLECTION_ME(PriorityQueue, capacity)
        COLLECTION_ME(PriorityQueue, peek)
        COLLECTION_ME(PriorityQueue, pop)
        COLLECTION_ME(PriorityQueue, push)

        COLLECTION_ME_LIST(PriorityQueue)
        PHP_FE_END
    };

    INIT_CLASS_ENTRY(ce, COLLECTION_NS(PriorityQueue), methods);

    priority_queue_ce = zend_register_internal_class(&ce);
    priority_queue_ce->ce_flags      |= ZEND_ACC_FINAL;
    priority_queue_ce->create_object  = priority_queue_init_object;
    priority_queue_ce->get_iterator   = priority_queue_get_iterator;
    priority_queue_ce->serialize      = priority_queue_serialize;
    priority_queue_ce->unserialize    = priority_queue_unserialize;

    zend_class_implements(priority_queue_ce, 1, collection_ce);
    register_priority_queue_handlers();
}
