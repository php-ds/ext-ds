#include "../../common.h"

#include "../parameters.h"
#include "../arginfo.h"

#include "../iterators/php_queue_iterator.h"
#include "../handlers/php_queue_handlers.h"
#include "../objects/php_queue.h"

#include "php_collection_ce.h"
#include "php_queue_ce.h"

#define METHOD(name) PHP_METHOD(Queue, name)

zend_class_entry *php_ds_queue_ce;

ARGINFO_OPTIONAL_ZVAL(__construct, values)
METHOD(__construct)
{
    PARSE_OPTIONAL_ZVAL(values);

    if (values) {
        if (Z_TYPE_P(values) == IS_LONG) {
            ds_queue_allocate(THIS_DS_QUEUE(), Z_LVAL_P(values));
        } else {
            ds_queue_push_all(THIS_DS_QUEUE(), values);
        }
    }
}

ARGINFO_LONG(allocate, capacity)
METHOD(allocate)
{
    PARSE_LONG(capacity);
    ds_queue_allocate(THIS_DS_QUEUE(), capacity);
}

ARGINFO_NONE_RETURN_LONG(capacity)
METHOD(capacity)
{
    PARSE_NONE;
    RETURN_LONG(ds_queue_capacity(THIS_DS_QUEUE()));
}

ARGINFO_VARIADIC_ZVAL(push, values)
METHOD(push)
{
    PARSE_VARIADIC_ZVAL();
    ds_queue_push(THIS_DS_QUEUE(), argc, argv);
}

ARGINFO_ZVAL(pushAll, values)
METHOD(pushAll)
{
    PARSE_ZVAL(values);
    ds_queue_push_all(THIS_DS_QUEUE(), values);
}

ARGINFO_NONE(pop)
METHOD(pop)
{
    PARSE_NONE;
    ds_queue_pop(THIS_DS_QUEUE(), return_value);
}

ARGINFO_NONE(peek)
METHOD(peek)
{
    PARSE_NONE;
    RETURN_ZVAL_COPY(ds_queue_peek(THIS_DS_QUEUE()));
}

ARGINFO_NONE_RETURN_DS(copy, Queue)
METHOD(copy)
{
    PARSE_NONE;
    RETURN_OBJ(php_ds_queue_create_clone(THIS_DS_QUEUE()));
}

ARGINFO_NONE_RETURN_LONG(count)
METHOD(count)
{
    PARSE_NONE;
    RETURN_LONG(QUEUE_SIZE(THIS_DS_QUEUE()));
}

ARGINFO_NONE(clear)
METHOD(clear)
{
    PARSE_NONE;
    ds_queue_clear(THIS_DS_QUEUE());
}

ARGINFO_NONE_RETURN_ARRAY(toArray)
METHOD(toArray)
{
    PARSE_NONE;
    ds_queue_to_array(THIS_DS_QUEUE(), return_value);
}

ARGINFO_NONE_RETURN_BOOL(isEmpty)
METHOD(isEmpty)
{
    PARSE_NONE;
    RETURN_BOOL(QUEUE_SIZE(THIS_DS_QUEUE()) == 0);
}

ARGINFO_NONE(jsonSerialize)
METHOD(jsonSerialize)
{
    PARSE_NONE;
    ds_queue_to_array(THIS_DS_QUEUE(), return_value);
}

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
        PHP_DS_ME(Queue, pushAll)

        PHP_DS_ME_LIST(Queue)
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
    zend_class_implements(php_ds_queue_ce, 1, collection_ce);

    php_ds_register_queue_handlers();
}
