#ifndef DS_PAIR_H
#define DS_PAIR_H

#include "../common.h"

typedef struct _ds_pair_t {
    zval key;
    zval value;
} ds_pair_t;

ds_pair_t *ds_pair();
ds_pair_t *ds_pair_ex(zval *key, zval *value);
ds_pair_t *ds_pair_clone(ds_pair_t *pair);

void ds_pair_to_array(ds_pair_t *pair, zval *return_value);
void ds_pair_destroy(ds_pair_t *pair);

#endif
