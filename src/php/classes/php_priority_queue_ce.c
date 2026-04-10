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

METHOD(__construct)
{
    PARSE_NONE;
}

METHOD(allocate)
{
    PARSE_LONG(capacity);
    if (capacity < 0) {
        CAPACITY_INVALID(capacity);
        return;
    }
    ds_priority_queue_allocate(THIS_DS_PRIORITY_QUEUE(), capacity);
}

METHOD(capacity)
{
    PARSE_NONE;
    RETURN_LONG(ds_priority_queue_capacity(THIS_DS_PRIORITY_QUEUE()));
}

METHOD(copy)
{
    PARSE_NONE;
    RETURN_OBJ(php_ds_priority_queue_create_clone(THIS_DS_PRIORITY_QUEUE()));
}

METHOD(push)
{
    PARSE_ZVAL_ZVAL(value, priority);
    ds_priority_queue_push(THIS_DS_PRIORITY_QUEUE(), value, priority);
}

METHOD(pop)
{
    PARSE_NONE;
    ds_priority_queue_pop(THIS_DS_PRIORITY_QUEUE(), return_value);
}

METHOD(peek)
{
    PARSE_NONE;
    RETURN_ZVAL_COPY(ds_priority_queue_peek(THIS_DS_PRIORITY_QUEUE()));
}

METHOD(isEmpty)
{
    PARSE_NONE;
    RETURN_BOOL(DS_PRIORITY_QUEUE_IS_EMPTY(THIS_DS_PRIORITY_QUEUE()));
}

METHOD(toArray)
{
    PARSE_NONE;
    ds_priority_queue_to_array(THIS_DS_PRIORITY_QUEUE(), return_value);
}

METHOD(count)
{
    PARSE_NONE;
    RETURN_LONG(DS_PRIORITY_QUEUE_SIZE(THIS_DS_PRIORITY_QUEUE()));
}

METHOD(clear)
{
    PARSE_NONE;
    ds_priority_queue_clear(THIS_DS_PRIORITY_QUEUE());
}

METHOD(jsonSerialize)
{
    PARSE_NONE;
    ds_priority_queue_to_array(THIS_DS_PRIORITY_QUEUE(), return_value);
}

METHOD(__serialize)
{
    PARSE_NONE;
    ds_priority_queue_t *queue = THIS_DS_PRIORITY_QUEUE();

    array_init_size(return_value, queue->size);

    if (queue->size > 0) {
        ds_priority_queue_node_t *nodes = ds_priority_queue_create_sorted_buffer(queue);
        ds_priority_queue_node_t *pos = nodes;
        ds_priority_queue_node_t *end = nodes + queue->size;

        for (; pos < end; ++pos) {
            zval pair;
            array_init_size(&pair, 2);
            Z_TRY_ADDREF(pos->value);
            Z_TRY_ADDREF(pos->priority);
            add_next_index_zval(&pair, &pos->value);
            add_next_index_zval(&pair, &pos->priority);
            add_next_index_zval(return_value, &pair);
        }

        efree(nodes);
    }
}

METHOD(__unserialize)
{
    PARSE_ZVAL(data);
    ds_priority_queue_t *queue = THIS_DS_PRIORITY_QUEUE();

    zval *entry;
    ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(data), entry) {
        zval *value = zend_hash_index_find(Z_ARRVAL_P(entry), 0);
        zval *priority = zend_hash_index_find(Z_ARRVAL_P(entry), 1);
        if (value && priority) {
            ds_priority_queue_push(queue, value, priority);
        }
    } ZEND_HASH_FOREACH_END();
}

METHOD(getIterator) {
    PARSE_NONE;
    ZVAL_COPY(return_value, getThis());
}

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
        PHP_DS_ME(PriorityQueue, getIterator)

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

    zend_class_implements(php_ds_priority_queue_ce, 1, collection_ce);
    php_ds_register_priority_queue_handlers();
}
