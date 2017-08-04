#include "../../ds_common.h"

#include "../php_ds_parameters.h"
#include "../php_ds_arginfo.h"

#include "../iterators/php_ds_priority_queue_iterator.h"
#include "../handlers/php_ds_priority_queue_handlers.h"

#include "php_ds_collection_ce.h"
#include "php_ds_priority_queue_ce.h"

#define METHOD(name) PHP_METHOD(PriorityQueue, name)

zend_class_entry *php_ds_priority_queue_ce;

METHOD(__construct)
{
    PHP_DS_PARAM_PARSE_NONE;
}

METHOD(allocate)
{
    PHP_DS_PARAM_PARSE_LONG(capacity);
    ds_priority_queue_allocate(THIS_DS_PRIORITY_QUEUE(), capacity);
}

METHOD(capacity)
{
    PHP_DS_PARAM_PARSE_NONE;
    RETURN_LONG(ds_priority_queue_capacity(THIS_DS_PRIORITY_QUEUE()));
}

METHOD(copy)
{
    PHP_DS_PARAM_PARSE_NONE;
    RETURN_OBJ(php_ds_priority_queue_create_clone(THIS_DS_PRIORITY_QUEUE()));
}

METHOD(push)
{
    PHP_DS_PARAM_PARSE_ZVAL_LONG(value, priority);
    ds_priority_queue_push(THIS_DS_PRIORITY_QUEUE(), value, priority);
}

METHOD(pop)
{
    PHP_DS_PARAM_PARSE_NONE;
    ds_priority_queue_pop(THIS_DS_PRIORITY_QUEUE(), return_value);
}

METHOD(peek)
{
    PHP_DS_PARAM_PARSE_NONE;
    RETURN_ZVAL_COPY(ds_priority_queue_peek(THIS_DS_PRIORITY_QUEUE()));
}

METHOD(isEmpty)
{
    PHP_DS_PARAM_PARSE_NONE;
    RETURN_BOOL(DS_PRIORITY_QUEUE_IS_EMPTY(THIS_DS_PRIORITY_QUEUE()));
}

METHOD(toArray)
{
    PHP_DS_PARAM_PARSE_NONE;
    ds_priority_queue_to_array(THIS_DS_PRIORITY_QUEUE(), return_value);
}

METHOD(count)
{
    PHP_DS_PARAM_PARSE_NONE;
    RETURN_LONG(DS_PRIORITY_QUEUE_SIZE(THIS_DS_PRIORITY_QUEUE()));
}

METHOD(clear)
{
    PHP_DS_PARAM_PARSE_NONE;
    ds_priority_queue_clear(THIS_DS_PRIORITY_QUEUE());
}

METHOD(jsonSerialize)
{
    PHP_DS_PARAM_PARSE_NONE;
    ds_priority_queue_to_array(THIS_DS_PRIORITY_QUEUE(), return_value);
}

PHP_DS_ARRAY_ACCESS_FORWARDING_METHODS(php_ds_priority_queue);

void php_ds_register_priority_queue()
{
    zend_class_entry ce;

    zend_function_entry methods[] = {
        PHP_DS_ME(PriorityQueue, __construct)
        PHP_DS_ME(PriorityQueue, allocate)
        PHP_DS_ME(PriorityQueue, capacity)
        PHP_DS_ME(PriorityQueue, peek)
        PHP_DS_ME(PriorityQueue, pop)
        PHP_DS_ME(PriorityQueue, push)

        PHP_DS_COLLECTION_ME_LIST(PriorityQueue)
        PHP_FE_END
    };

    INIT_CLASS_ENTRY(ce, PHP_DS_NS(PriorityQueue), methods);

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

    zend_class_implements(php_ds_priority_queue_ce, 1, php_ds_collection_ce);
    php_ds_register_priority_queue_handlers();
}
