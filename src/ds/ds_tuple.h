#ifndef DS_TUPLE_H
#define DS_TUPLE_H

#include "../ds_common.h"

#define DS_TUPLE_SIZE(s)     ((s)->size)
#define DS_TUPLE_CAPACITY(s) ((s)->capacity)
#define DS_TUPLE_IS_EMPTY(s) (DS_TUPLE_SIZE(s) == 0)

#define DS_TUPLE_FOREACH(tuple, value) \
do {                                   \

#define DS_TUPLE_FOREACH_END()  \
    }                           \
} while (0)                     \

typedef struct ds_tuple_t {
    zval       *buffer;
    zend_long   length;  // Buffer length
} ds_tuple_t;

ds_tuple_t *ds_tuple();

#endif
