#ifndef PHP_DS_SET_H
#define PHP_DS_SET_H

#include "../../ds/ds_set.h"

typedef struct _php_ds_set_t {
    ds_set_t      *set;
    zend_object    std;
} php_ds_set_t;

static inline php_ds_set_t *php_ds_set_fetch_object(zend_object *obj) {
	return (php_ds_set_t *)((char*)(obj) - XtOffsetOf(php_ds_set_t, std));
}

#define Z_DS_SET(z)   (php_ds_set_fetch_object(Z_OBJ(z))->set)
#define Z_DS_SET_P(z) Z_DS_SET(*z)
#define THIS_DS_SET() Z_DS_SET_P(getThis())

#define ZVAL_DS_SET(z, set) ZVAL_OBJ(z, php_ds_set_create_object_ex(set))

#define RETURN_DS_SET(s)                    \
do {                                        \
    ds_set_t *_s = s;                       \
    if (_s) {                               \
        ZVAL_DS_SET(return_value, _s);      \
    } else {                                \
        ZVAL_NULL(return_value);            \
    }                                       \
    return;                                 \
} while(0)

zend_object *php_ds_set_create_object_ex(ds_set_t *set);
zend_object *php_ds_set_create_object(zend_class_entry *ce);
zend_object *php_ds_set_create_clone(ds_set_t *set);

PHP_DS_SERIALIZE_FUNCIONS(php_ds_set);

#endif
