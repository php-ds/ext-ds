#ifndef DS_STACK_H
#define DS_STACK_H

#include "../common.h"
#include "ds_vector.h"

#define DS_STACK_SIZE(s)     ((s)->vector->size)
#define DS_STACK_CAPACITY(s) ((s)->vector->capacity)
#define DS_STACK_IS_EMPTY(s) (DS_STACK_SIZE(s) == 0)

#define DS_STACK_FOREACH(stack, value)                 \
do {                                                \
    zval _tmp;                                      \
                                                    \
    ds_vector_t *_v = stack->vector;                     \
    zval *_end = _v->buffer;                        \
    zval *_pos = _end + _v->size - 1;               \
                                                    \
    for (; _pos >= _end; --_pos, --_v->size) {      \
        ZVAL_COPY(&_tmp, _pos);                     \
        zval_ptr_dtor(_pos);                        \
        value = &_tmp;

#define DS_STACK_FOREACH_END()     \
    }                           \
    zval_ptr_dtor(&_tmp);       \
} while (0)                     \

typedef struct _ds_stack_t {
    ds_vector_t *vector;
} ds_stack_t;

ds_stack_t *ds_stack_ex(ds_vector_t *vector);
ds_stack_t *ds_stack();
ds_stack_t *ds_stack_clone(ds_stack_t *stack);

void  ds_stack_push(ds_stack_t *stack, VA_PARAMS);
void  ds_stack_allocate(ds_stack_t *stack, zend_long capacity);
void  ds_stack_push_one(ds_stack_t *stack, zval *value);
void  ds_stack_clear(ds_stack_t *stack);
void  ds_stack_pop(ds_stack_t *stack, zval *return_value);
zval *ds_stack_peek(ds_stack_t *stack);
void  ds_stack_push_all(ds_stack_t *stack, zval *value);
void  ds_stack_to_array(ds_stack_t *stack, zval *return_value);
void  ds_stack_destroy(ds_stack_t *stack);

#endif
