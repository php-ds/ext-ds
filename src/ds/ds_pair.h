#ifndef DS_PAIR_H
#define DS_PAIR_H

#include "../common.h"

typedef struct _Pair {
    zend_object std;
    zval        key;
    zval        value;
} Pair;

#define Z_PAIR(z)   ((Pair*)(Z_OBJ(z)))
#define Z_PAIR_P(z) Z_PAIR(*z)
#define THIS_PAIR() Z_PAIR_P(getThis())

#define ZVAL_PAIR(z, pair) ZVAL_OBJ(z, &pair->std)

#define RETURN_PAIR(pair) \
do { \
    ZVAL_PAIR(return_value, pair); \
    return; \
} while(0)

void pair_to_array(Pair *pair, zval *return_value);
void pair_destroy(Pair *pair);
void pair_create_as_zval(zval *key, zval *value, zval *return_value);

zend_object *pair_create_clone(Pair *pair);
zend_object *pair_create_object(zend_class_entry *ce);

int pair_serialize(zval *object, unsigned char **buffer, size_t *length, zend_serialize_data *data);
int pair_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t length, zend_unserialize_data *data);
#endif
