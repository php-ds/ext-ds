#include "../../common.h"

#include "../parameters.h"
#include "../arginfo.h"

#include "../iterators/php_priority_queue_iterator.h"
#include "../handlers/php_priority_queue_handlers.h"
#include "../objects/php_priority_queue.h"

#include "php_collection_ce.h"
#include "php_priority_queue_ce.h"

#define METHOD(name) PHP_METHOD(PriorityQueue, name)

zend_class_entry *php_ds_priority_queue_ce;

ARGINFO_NONE(__construct)
METHOD(__construct)
{
    PARSE_NONE;
}

ARGINFO_LONG(allocate, capacity)
METHOD(allocate)
{
    PARSE_LONG(capacity);
    ds_priority_queue_allocate(THIS_DS_PRIORITY_QUEUE(), capacity);
}

ARGINFO_NONE_RETURN_LONG(capacity)
METHOD(capacity)
{
    PARSE_NONE;
    RETURN_LONG(ds_priority_queue_capacity(THIS_DS_PRIORITY_QUEUE()));
}

ARGINFO_NONE_RETURN_DS(copy, PriorityQueue)
METHOD(copy)
{
    PARSE_NONE;
    RETURN_OBJ(php_ds_priority_queue_create_clone(THIS_DS_PRIORITY_QUEUE()));
}

ARGINFO_ZVAL_LONG(push, value, priority)
METHOD(push)
{
    PARSE_ZVAL_LONG(value, priority);
    ds_priority_queue_push(THIS_DS_PRIORITY_QUEUE(), value, priority);
}

ARGINFO_NONE(pop)
METHOD(pop)
{
    PARSE_NONE;
    ds_priority_queue_pop(THIS_DS_PRIORITY_QUEUE(), return_value);
}

ARGINFO_NONE(peek)
METHOD(peek)
{
    PARSE_NONE;
    RETURN_ZVAL_COPY(ds_priority_queue_peek(THIS_DS_PRIORITY_QUEUE()));
}

ARGINFO_NONE_RETURN_BOOL(isEmpty)
METHOD(isEmpty)
{
    PARSE_NONE;
    RETURN_BOOL(DS_PRIORITY_QUEUE_IS_EMPTY(THIS_DS_PRIORITY_QUEUE()));
}

ARGINFO_NONE_RETURN_ARRAY(toArray)
METHOD(toArray)
{
    PARSE_NONE;
    ds_priority_queue_to_array(THIS_DS_PRIORITY_QUEUE(), return_value);
}

ARGINFO_NONE_RETURN_LONG(count)
METHOD(count)
{
    PARSE_NONE;
    RETURN_LONG(DS_PRIORITY_QUEUE_SIZE(THIS_DS_PRIORITY_QUEUE()));
}

ARGINFO_NONE(clear)
METHOD(clear)
{
    PARSE_NONE;
    ds_priority_queue_clear(THIS_DS_PRIORITY_QUEUE());
}

ARGINFO_NONE(jsonSerialize)
METHOD(jsonSerialize)
{
    PARSE_NONE;
    ds_priority_queue_to_array(THIS_DS_PRIORITY_QUEUE(), return_value);
}

void php_ds_register_priority_queue()
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

    INIT_CLASS_ENTRY(ce, DS_NS(PriorityQueue), methods);

    php_ds_priority_queue_ce = zend_register_internal_class(&ce);
    php_ds_priority_queue_ce->ce_flags      |= ZEND_ACC_FINAL;
    php_ds_priority_queue_ce->create_object  = php_ds_priority_queue_create_object;
    php_ds_priority_queue_ce->get_iterator   = php_ds_priority_queue_get_iterator;
    php_ds_priority_queue_ce->serialize      = php_ds_priority_queue_serialize;
    php_ds_priority_queue_ce->unserialize    = php_ds_priority_queue_unserialize;

    zend_declare_class_constant_long(
        php_ds_priority_queue_ce,
        STR_AND_LEN("MIN_CAPACITY"),
        DS_PRIORITY_QUEUE_MIN_CAPACITY
    );

    zend_class_implements(php_ds_priority_queue_ce, 1, collection_ce);
    php_ds_register_priority_queue_handlers();
}
