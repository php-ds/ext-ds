#ifndef DS_PQ_CE_H
#define DS_PQ_CE_H

#include "php.h"
#include "../../common.h"
#include "../arginfo.h"
#include "../objects/php_pq.h"

#define THIS_DS_PQ() Z_DS_PQ_P(getThis())

#define RETURN_DS_PQ(queue)                 \
do {                                                    \
    ds_pq_t *_queue = queue;                \
    if (_queue) {                                       \
        ZVAL_DS_PQ(return_value, _queue);   \
    } else {                                            \
        ZVAL_NULL(return_value);                        \
    }                                                   \
    return;                                             \
} while(0)

extern zend_class_entry *php_ds_pq_ce;

ARGINFO_NONE(                   PriorityQueue___construct);
ARGINFO_LONG(                   PriorityQueue_allocate, capacity);
ARGINFO_NONE_RETURN_LONG(       PriorityQueue_capacity);
ARGINFO_NONE_RETURN_DS(         PriorityQueue_copy, PriorityQueue);
ARGINFO_ZVAL_LONG(              PriorityQueue_push, value, priority);
ARGINFO_NONE(                   PriorityQueue_pop);
ARGINFO_NONE(                   PriorityQueue_peek);

void php_ds_register_pq();

#endif
