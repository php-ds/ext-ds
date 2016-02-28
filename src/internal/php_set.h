#ifndef PHP_DS_SET_H
#define PHP_DS_SET_H

#include "php.h"
#include "zend_exceptions.h"
#include "zend_interfaces.h"
#include "ext/spl/spl_iterators.h"

#include "php_htable.h"

typedef struct _Set {
    zend_object  std;
    HTable      *table;
} Set;

#define SET_SIZE(s) ((s)->table->size)
#define SET_IS_EMPTY(s) (SET_SIZE(s) == 0)

#define Z_SET(z)   ((Set*)(Z_OBJ(z)))
#define Z_SET_P(z) Z_SET(*z)
#define THIS_SET() Z_SET_P(getThis())

#define ZVAL_SET(z, set)  ZVAL_OBJ(z, &set->std)
#define ZVAL_NEW_SET(z)   ZVAL_SET(z, set_init())

#define RETURN_SET(set) \
do { \
    ZVAL_SET(return_value, set); \
    return; \
} while(0)

#define SET_FOREACH(set, value) HTABLE_FOREACH_KEY(set->table, value)
#define SET_FOREACH_END() HTABLE_FOREACH_END()

#define SET_SIZE(s) ((s)->table->size)
#define SET_IS_EMPTY(s) (SET_SIZE(s) == 0)

Set *set_init();

zend_object *set_create_object_ex(HTable *table);
zend_object *set_create_object(zend_class_entry *ce);
zend_object *set_create_clone(Set *set);
void set_init_zval_ex(zval *obj, Set *set);
void set_user_allocate(Set *set, zend_long capacity);
uint32_t set_capacity(Set *set);

Set *set_clone(Set *set);
void set_free(Set *set);
void set_add(Set *set, zval *value);
void set_add_va(Set *set, VA_PARAMS);
bool set_contains_all(Set *set, VA_PARAMS);
bool set_contains(Set *set, zval *value);
void set_remove_va(Set *set, VA_PARAMS);
void set_clear(Set *set);
void set_to_array(Set *set, zval *arr);
void set_add_all(Set *set, zval *value);
void set_init_zval_ex(zval *obj, Set *set);
void set_init_zval(zval *obj);

zval *set_get(Set *set, zend_long index);
zval *set_get_first(Set *set);
zval *set_get_last(Set *set);

void set_slice(Set *set, zend_long index, zend_long length, zval *obj);
void set_join(Set *set, const char *glue, const size_t len, zval *return_value);
void set_sort_callback(Set *set);
void set_sort(Set *set);
void set_sorted_callback(Set *set, zval *obj);
void set_sorted(Set *set, zval *obj);

void set_reduce(Set *set, FCI_PARAMS, zval *initial, zval *return_value);
void set_filter_callback(Set *set, FCI_PARAMS, zval *return_value);
void set_filter(Set *set, zval *return_value);

void set_reverse(Set *set);
void set_reversed(Set *set, zval *return_value);

int set_serialize(zval *object, unsigned char **buffer, size_t *length, zend_serialize_data *data);
int set_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t length, zend_unserialize_data *data);

void set_diff(Set *set, zval *obj, zval *return_value);
void set_intersect(Set *set, zval *obj, zval *return_value);
void set_xor(Set *set, zval *obj, zval *return_value);
void set_union(Set *set, zval *obj, zval *return_value);

void set_assign_diff(Set *set, zval *obj);
void set_assign_intersect(Set *set, zval *obj);
void set_assign_xor(Set *set, zval *obj);
void set_assign_union(Set *set, zval *obj);

#endif
