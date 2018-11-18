#ifndef PHP_DS_PAIR_H
#define PHP_DS_PAIR_H

#include "../../common.h"

#define Z_DS_PAIR(z)   ((php_ds_pair_t*)(Z_OBJ(z)))
#define Z_DS_PAIR_P(z) Z_DS_PAIR(*z)
#define THIS_DS_PAIR() Z_DS_PAIR_P(getThis())

#define ZVAL_DS_PAIR(z, p) ZVAL_OBJ(z, (zend_object*) p)

#define RETURN_DS_PAIR(p)                    \
do {                                         \
    php_ds_pair_t *_p = (php_ds_pair_t *) p; \
    if (_p) {                                \
        ZVAL_DS_PAIR(return_value, _p);      \
    } else {                                 \
        ZVAL_NULL(return_value);             \
    }                                        \
    return;                                  \
} while(0)

typedef struct _php_ds_pair_t {
    zend_object std;
} php_ds_pair_t;

php_ds_pair_t *php_ds_pair_ex(zval *key, zval *value);
php_ds_pair_t *php_ds_pair();

void php_ds_pair_set_key(php_ds_pair_t *obj, zval *key);
void php_ds_pair_set_value(php_ds_pair_t *obj, zval *value);

zval *php_ds_pair_get_key(php_ds_pair_t *obj);
zval *php_ds_pair_get_value(php_ds_pair_t *obj);

zend_object *php_ds_pair_create_object(zend_class_entry *ce);
zend_object *php_ds_pair_create_clone(php_ds_pair_t *obj);

void php_ds_pair_to_array(php_ds_pair_t *obj, zval *result);

PHP_DS_SERIALIZE_FUNCIONS(php_ds_pair);

#endif
