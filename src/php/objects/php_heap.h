#ifndef PHP_DS_HEAP_H
#define PHP_DS_HEAP_H

#include "../../ds/ds_heap.h"

typedef struct _php_ds_heap_t {
    ds_heap_t              *heap;
    zend_fcall_info         fci;
    zend_fcall_info_cache   fci_cache;
    bool                    has_comparator;
    zend_object             std;
} php_ds_heap_t;

static inline php_ds_heap_t *php_ds_heap_fetch_object(zend_object *obj) {
    return (php_ds_heap_t *)((char*)(obj) - XtOffsetOf(php_ds_heap_t, std));
}

#define Z_DS_HEAP(z)    (php_ds_heap_fetch_object(Z_OBJ(z)))
#define Z_DS_HEAP_P(z)  Z_DS_HEAP(*z)
#define THIS_DS_HEAP()  Z_DS_HEAP_P(getThis())

#define RETURN_DS_HEAP(h)                                           \
do {                                                                \
    ds_heap_t *_h = h;                                              \
    if (_h) {                                                       \
        ZVAL_OBJ(return_value, php_ds_heap_create_object_ex(_h));   \
    } else {                                                        \
        ZVAL_NULL(return_value);                                    \
    }                                                               \
    return;                                                         \
} while(0)

/**
 * Returns the compare function for this heap.
 * If the heap has a custom comparator, sets up the module globals
 * and returns the user compare function. Otherwise returns the default.
 */
ds_heap_compare_func_t php_ds_heap_get_compare(php_ds_heap_t *obj);

/**
 * User comparator function that reads from module globals.
 */
int ds_heap_user_compare(zval *a, zval *b);

zend_object *php_ds_heap_create_object_ex(ds_heap_t *heap);
zend_object *php_ds_heap_create_object(zend_class_entry *ce);
zend_object *php_ds_heap_create_clone(php_ds_heap_t *obj);

PHP_DS_SERIALIZE_FUNCIONS(php_ds_heap);

#endif
