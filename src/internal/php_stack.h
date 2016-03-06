#ifndef PHP_DS_STACK_H
#define PHP_DS_STACK_H

#include "php.h"
#include "zend_exceptions.h"
#include "zend_interfaces.h"
#include "ext/spl/spl_iterators.h"

#include "php_vector.h"

typedef struct _Stack {
    zend_object     std;
    ds_vector_t         *vector;
} Stack;

#define STACK_SIZE(s) ((s)->vector->size)
#define STACK_IS_EMPTY(s) (STACK_SIZE(s) == 0)

#define Z_STACK(z)   ((Stack*)(Z_OBJ(z)))
#define Z_STACK_P(z) Z_STACK(*z)
#define THIS_STACK() Z_STACK_P(getThis())

#define ZVAL_STACK(z, stack)  ZVAL_OBJ(z, &stack->std)
#define ZVAL_NEW_STACK(z)     ZVAL_STACK(z, stack_init())

#define RETURN_STACK(stack) \
do { \
    ZVAL_STACK(return_value, stack); \
    return; \
} while(0)

#define STACK_FOREACH(stack, value)                 \
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

#define STACK_FOREACH_END()     \
    }                           \
    zval_ptr_dtor(&_tmp);       \
} while (0)                     \

Stack *stack_init();
zend_object *stack_create_object(zend_class_entry *ce);
zend_object *stack_create_clone(Stack *stack);
void stack_push(Stack *stack, VA_PARAMS);
void stack_user_allocate(Stack *stack, zend_long capacity);
zend_long stack_capacity(Stack *stack);
void stack_push_one(Stack *stack, zval *value);
void stack_clear(Stack *stack);
void stack_pop(Stack *stack, zval *return_value);
zval *stack_peek(Stack *stack);
void stack_push_all(Stack *stack, zval *value);
void stack_to_array(Stack *stack, zval *return_value);
void stack_destroy(Stack *stack);

int stack_serialize(zval *object, unsigned char **buffer, size_t *length, zend_serialize_data *data);
int stack_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t length, zend_unserialize_data *data);

#endif
