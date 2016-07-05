#include "../common.h"

#include "../php/handlers/php_pair_handlers.h"
#include "../php/classes/php_ce_pair.h"

#include "ds_pair.h"

void ds_pair_to_array(ds_pair_t *pair, zval *array)
{
    array_init_size(array, 2);

    add_assoc_zval(array, "key", &pair->key);
    add_assoc_zval(array, "value", &pair->value);

    Z_TRY_ADDREF_P(&pair->key);
    Z_TRY_ADDREF_P(&pair->value);
}

void ds_pair_destroy(ds_pair_t *pair)
{
    DTOR_AND_UNDEF(&pair->key);
    DTOR_AND_UNDEF(&pair->value);
    efree(pair);
}

ds_pair_t *ds_pair()
{
    ds_pair_t *pair = ecalloc(1, sizeof(ds_pair_t));

    ZVAL_UNDEF(&pair->key);
    ZVAL_UNDEF(&pair->value);

    return pair;
}

ds_pair_t *ds_pair_ex(zval *key, zval *value)
{
    ds_pair_t *pair = ds_pair();

    ZVAL_COPY(&pair->key,   key);
    ZVAL_COPY(&pair->value, value);

    return pair;
}

ds_pair_t *ds_pair_clone(ds_pair_t *pair)
{
    ds_pair_t *clone = ds_pair();

    ZVAL_COPY(&clone->key,   &pair->key);
    ZVAL_COPY(&clone->value, &pair->value);

    return clone;
}
