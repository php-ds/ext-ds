#ifndef PHP_DS_STACK_H
#define PHP_DS_STACK_H

#include "../../ds/ds_stack.h"

typedef struct _php_ds_stack_t {
    ds_stack_t      *stack;
    zend_object      std;
} php_ds_stack_t;

static inline php_ds_stack_t *php_ds_stack_fetch_object(zend_object *obj) {
	return (php_ds_stack_t *)((char*)(obj) - XtOffsetOf(php_ds_stack_t, std));
}

#define Z_DS_STACK(z)   php_ds_stack_fetch_object(Z_OBJ(z))->stack
#define Z_DS_STACK_P(z) Z_DS_STACK(*z)
#define THIS_DS_STACK() Z_DS_STACK_P(getThis())

#define ZVAL_DS_STACK(z, s)  ZVAL_OBJ(z, php_ds_stack_create_object_ex(s))

#define RETURN_DS_STACK(s)                  \
do {                                        \
    ds_stack_t *_s = s;                     \
    if (_s) {                               \
        ZVAL_DS_STACK(return_value, _s);    \
    } else {                                \
        ZVAL_NULL(return_value);            \
    }                                       \
    return;                                 \
} while(0)

zend_object *php_ds_stack_create_object_ex(ds_stack_t *stack);
zend_object *php_ds_stack_create_object(zend_class_entry *ce);
zend_object *php_ds_stack_create_clone(ds_stack_t *stack);

PHP_DS_SERIALIZE_FUNCIONS(php_ds_stack);

#endif
