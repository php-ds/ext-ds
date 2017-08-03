#include "../../common.h"

#include "../parameters.h"
#include "../arginfo.h"

#include "../iterators/php_queue_iterator.h"
#include "../handlers/php_queue_handlers.h"

#include "php_collection_ce.h"
#include "php_queue_ce.h"

#define METHOD(name) PHP_METHOD(Queue, name)

zend_class_entry *php_ds_queue_ce;

METHOD(__construct)
{
    PARSE_OPTIONAL_ZVAL(values);

    if (values) {
        ds_queue_push_all(THIS_DS_QUEUE(), values);
    }
}

METHOD(allocate)
{
    PARSE_LONG(capacity);
    ds_queue_allocate(THIS_DS_QUEUE(), capacity);
}

METHOD(capacity)
{
    PARSE_NONE;
    RETURN_LONG(ds_queue_capacity(THIS_DS_QUEUE()));
}

METHOD(push)
{
    PARSE_VARIADIC_ZVAL();
    ds_queue_push(THIS_DS_QUEUE(), argc, argv);
}

METHOD(pop)
{
    PARSE_NONE;
    ds_queue_pop_throw(THIS_DS_QUEUE(), return_value);
}

METHOD(peek)
{
    PARSE_NONE;
    RETURN_ZVAL_COPY(ds_queue_peek_throw(THIS_DS_QUEUE()));
}

METHOD(copy)
{
    PARSE_NONE;
    RETURN_OBJ(php_ds_queue_create_clone(THIS_DS_QUEUE()));
}

METHOD(count)
{
    PARSE_NONE;
    RETURN_LONG(QUEUE_SIZE(THIS_DS_QUEUE()));
}

METHOD(clear)
{
    PARSE_NONE;
    ds_queue_clear(THIS_DS_QUEUE());
}

METHOD(toArray)
{
    PARSE_NONE;
    ds_queue_to_array(THIS_DS_QUEUE(), return_value);
}

METHOD(isEmpty)
{
    PARSE_NONE;
    RETURN_BOOL(QUEUE_SIZE(THIS_DS_QUEUE()) == 0);
}

METHOD(jsonSerialize)
{
    PARSE_NONE;
    ds_queue_to_array(THIS_DS_QUEUE(), return_value);
}

PHP_DS_ARRAY_ACCESS_FORWARDING_METHODS(php_ds_queue);

void php_ds_register_queue()
{
    zend_class_entry ce;

    zend_function_entry methods[] = {
        PHP_DS_ME(Queue, __construct)
        PHP_DS_ME(Queue, allocate)
        PHP_DS_ME(Queue, capacity)
        PHP_DS_ME(Queue, peek)
        PHP_DS_ME(Queue, pop)
        PHP_DS_ME(Queue, push)

        PHP_DS_COLLECTION_ME_LIST(Queue)
        PHP_FE_END
    };

    INIT_CLASS_ENTRY(ce, PHP_DS_NS(Queue), methods);

    php_ds_queue_ce = zend_register_internal_class(&ce);
    php_ds_queue_ce->ce_flags      |= ZEND_ACC_FINAL;
    php_ds_queue_ce->create_object  = php_ds_queue_create_object;
    php_ds_queue_ce->get_iterator   = php_ds_queue_get_iterator;
    php_ds_queue_ce->serialize      = php_ds_queue_serialize;
    php_ds_queue_ce->unserialize    = php_ds_queue_unserialize;

    zend_declare_class_constant_long(php_ds_queue_ce, STR_AND_LEN("MIN_CAPACITY"), DS_DEQUE_MIN_CAPACITY);
    zend_class_implements(php_ds_queue_ce, 1, php_ds_collection_ce);

    php_ds_register_queue_handlers();
}
