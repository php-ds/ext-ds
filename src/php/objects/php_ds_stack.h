#ifndef PHP_DS_STACK_H
#define PHP_DS_STACK_H

#include "../../ds/ds_stack.h"

#define Z_DS_STACK(z)   (((php_ds_stack_t*)(Z_OBJ(z)))->stack)
#define Z_DS_STACK_P(z) Z_DS_STACK(*z)

#define ZVAL_DS_STACK(z, s)  ZVAL_OBJ(z, php_ds_stack_create_object_ex(s))

typedef struct _php_ds_stack_t {
    zend_object      std;
    ds_stack_t      *stack;
} php_ds_stack_t;

zend_object *php_ds_stack_create_object_ex(ds_stack_t *stack);
zend_object *php_ds_stack_create_object(zend_class_entry *ce);
zend_object *php_ds_stack_create_clone(ds_stack_t *stack);

PHP_DS_SERIALIZE_FUNCIONS(php_ds_stack);

#endif
