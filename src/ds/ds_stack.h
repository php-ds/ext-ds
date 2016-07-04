#ifndef PHP_DS_STACK_H
#define PHP_DS_STACK_H

#include "../common.h"
#include "ds_vector.h"

typedef struct php_ds_stack {
    zend_object     std;
    ds_vector_t    *vector;
} php_ds_stack_t;

#define DS_STACK_SIZE(s) ((s)->vector->size)
#define DS_STACK_IS_EMPTY(s) (DS_STACK_SIZE(s) == 0)

#define Z_DS_STACK(z)   ((php_ds_stack_t*)(Z_OBJ(z)))
#define Z_DS_STACK_P(z) Z_DS_STACK(*z)
#define THIS_DS_STACK() Z_DS_STACK_P(getThis())

#define ZVAL_DS_STACK(z, stack)  ZVAL_OBJ(z, &stack->std)
#define ZVAL_NEW_DS_STACK(z)     ZVAL_DS_STACK(z, php_ds_stack_init())

#define RETURN_DS_STACK(stack) \
do { \
    ZVAL_DS_STACK(return_value, stack); \
    return; \
} while(0)

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

php_ds_stack_t *php_ds_stack_init();
zend_object *php_ds_stack_create_object(zend_class_entry *ce);
zend_object *php_ds_stack_create_clone(php_ds_stack_t *stack);
void php_ds_stack_push(php_ds_stack_t *stack, VA_PARAMS);
void php_ds_stack_user_allocate(php_ds_stack_t *stack, zend_long capacity);
zend_long php_ds_stack_capacity(php_ds_stack_t *stack);
void php_ds_stack_push_one(php_ds_stack_t *stack, zval *value);
void php_ds_stack_clear(php_ds_stack_t *stack);
void php_ds_stack_pop(php_ds_stack_t *stack, zval *return_value);
zval *php_ds_stack_peek(php_ds_stack_t *stack);
void php_ds_stack_push_all(php_ds_stack_t *stack, zval *value);
void php_ds_stack_to_array(php_ds_stack_t *stack, zval *return_value);
void php_ds_stack_destroy(php_ds_stack_t *stack);

int php_ds_stack_serialize(zval *object, unsigned char **buffer, size_t *length, zend_serialize_data *data);
int php_ds_stack_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t length, zend_unserialize_data *data);

#endif
