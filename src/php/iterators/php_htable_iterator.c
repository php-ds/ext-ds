#include "../../common.h"

#include "../../ds/ds_htable.h"
#include "php_htable_iterator.h"

static ds_htable_bucket_t *find_starting_bucket(ds_htable_t *table)
{
    ds_htable_bucket_t *bucket = table->buckets;

    if (table->size != 0) {
        ds_htable_bucket_t *last = table->buckets + table->capacity;

        while (bucket != last && DS_HTABLE_BUCKET_DELETED(bucket)) {
            ++bucket;
        }
    }

    return bucket;
}

static void php_ds_htable_iterator_dtor(zend_object_iterator *iter)
{
    ds_htable_iterator_t *iterator = (ds_htable_iterator_t *) iter;

    OBJ_RELEASE(iterator->obj);
    DTOR_AND_UNDEF(&iterator->intern.data);
}

static int php_ds_htable_iterator_valid(zend_object_iterator *iter)
{
    ds_htable_iterator_t *iterator = (ds_htable_iterator_t *) iter;
    uint32_t size                  = iterator->table->size;
    uint32_t position              = iterator->position;

    return position < size ? SUCCESS : FAILURE;
}

static zval *php_ds_htable_iterator_get_current_value(zend_object_iterator *iter)
{
    ds_htable_iterator_t *iterator = (ds_htable_iterator_t *) iter;
    ds_htable_bucket_t   *bucket   = iterator->bucket;

    if ( ! DS_HTABLE_BUCKET_DELETED(bucket)) {
        return &bucket->value;
    }

    return NULL;
}

static zval *php_ds_htable_iterator_get_current_keyval(zend_object_iterator *iter)
{
    ds_htable_iterator_t *iterator = (ds_htable_iterator_t *) iter;
    ds_htable_bucket_t   *bucket   = iterator->bucket;

    if ( ! DS_HTABLE_BUCKET_DELETED(bucket)) {
        return &bucket->key;
    }

    return NULL;
}

static void php_ds_htable_iterator_get_current_key(zend_object_iterator *iter, zval *key)
{
    ds_htable_iterator_t *iterator = (ds_htable_iterator_t *) iter;
    ds_htable_bucket_t   *bucket   = iterator->bucket;

    if ( ! DS_HTABLE_BUCKET_DELETED(bucket)) {
        ZVAL_COPY(key, &bucket->key);
    }
}

static zval *php_ds_htable_iterator_get_current_pair(zend_object_iterator *iter)
{
    ds_htable_iterator_t *iterator = (ds_htable_iterator_t *) iter;
    ds_htable_bucket_t   *bucket   = iterator->bucket;

    if ( ! DS_HTABLE_BUCKET_DELETED(bucket)) {

        zval *key = &bucket->key;
        zval *val = &bucket->value;

        zval *arr = &iterator->intern.data;

        Z_TRY_ADDREF_P(key);
        Z_TRY_ADDREF_P(val);

        array_init_size(arr, 2);

        add_next_index_zval(arr, key);
        add_next_index_zval(arr, val);

        return arr;
    }

    return NULL;
}

static void php_ds_htable_iterator_get_current_pos(zend_object_iterator *iter, zval *key)
{
    ZVAL_LONG(key, ((ds_htable_iterator_t *) iter)->position);
}

static void php_ds_htable_iterator_move_forward(zend_object_iterator *iter)
{
    ds_htable_iterator_t *iterator = (ds_htable_iterator_t *) iter;

    if (++iterator->position < iterator->table->size) {
        do {
            ++iterator->bucket;
        } while (DS_HTABLE_BUCKET_DELETED(iterator->bucket));
    }
}

static void php_ds_htable_iterator_rewind(zend_object_iterator *iter)
{
    ds_htable_iterator_t *iterator = (ds_htable_iterator_t *) iter;

    iterator->position = 0;
    iterator->bucket   = find_starting_bucket(iterator->table);
}

static zend_object_iterator_funcs php_ds_htable_get_value_iterator_funcs = {
    php_ds_htable_iterator_dtor,
    php_ds_htable_iterator_valid,
    php_ds_htable_iterator_get_current_value, // value
    php_ds_htable_iterator_get_current_pos,   // key
    php_ds_htable_iterator_move_forward,
    php_ds_htable_iterator_rewind
};

static zend_object_iterator_funcs php_ds_htable_get_key_iterator_funcs = {
    php_ds_htable_iterator_dtor,
    php_ds_htable_iterator_valid,
    php_ds_htable_iterator_get_current_keyval, // value
    php_ds_htable_iterator_get_current_pos,    // key
    php_ds_htable_iterator_move_forward,
    php_ds_htable_iterator_rewind
};

static zend_object_iterator_funcs php_ds_htable_get_pair_iterator_funcs = {
    php_ds_htable_iterator_dtor,
    php_ds_htable_iterator_valid,
    php_ds_htable_iterator_get_current_pair, // value
    php_ds_htable_iterator_get_current_pos,  // key
    php_ds_htable_iterator_move_forward,
    php_ds_htable_iterator_rewind
};

static zend_object_iterator_funcs php_ds_htable_get_assoc_iterator_funcs = {
    php_ds_htable_iterator_dtor,
    php_ds_htable_iterator_valid,
    php_ds_htable_iterator_get_current_value, // value
    php_ds_htable_iterator_get_current_key,   // key
    php_ds_htable_iterator_move_forward,
    php_ds_htable_iterator_rewind
};


static zend_object_iterator *php_ds_htable_create_htable_iterator(
    zval *obj,
    ds_htable_t *table,
    zend_object_iterator_funcs *funcs,
    int by_ref
) {
    ds_htable_iterator_t *iterator;

    if (by_ref) {
        ITERATION_BY_REF_NOT_SUPPORTED();
        return NULL;
    }

    iterator = ecalloc(1, sizeof(ds_htable_iterator_t));

    zend_iterator_init((zend_object_iterator*) iterator);

    ZVAL_UNDEF(&iterator->intern.data);

    iterator->intern.funcs  = funcs;
    iterator->table         = table;
    iterator->obj           = Z_OBJ_P(obj);

    // Add a reference to the object so that it doesn't get collected when
    // the iterated object is implict, eg. foreach ($obj->getInstance() as $value){ ... }
    ++GC_REFCOUNT(iterator->obj);

    return (zend_object_iterator *) iterator;
}

zend_object_iterator *php_ds_htable_get_value_iterator_ex(
    zend_class_entry *ce,
    zval *obj,
    int by_ref,
    ds_htable_t *table
){
    return php_ds_htable_create_htable_iterator(
        obj, table, &php_ds_htable_get_value_iterator_funcs, by_ref);
}

zend_object_iterator *php_ds_htable_get_key_iterator_ex(
    zend_class_entry *ce,
    zval *obj,
    int by_ref,
    ds_htable_t *table
){
    return php_ds_htable_create_htable_iterator(
        obj, table, &php_ds_htable_get_key_iterator_funcs, by_ref);
}

zend_object_iterator *php_ds_htable_get_pair_iterator_ex(
    zend_class_entry *ce,
    zval *obj,
    int by_ref,
    ds_htable_t *table
){
    return php_ds_htable_create_htable_iterator(
        obj, table, &php_ds_htable_get_pair_iterator_funcs, by_ref);
}

zend_object_iterator *php_ds_htable_get_assoc_iterator_ex(
    zend_class_entry *ce,
    zval *obj,
    int by_ref,
    ds_htable_t *table
){
    return php_ds_htable_create_htable_iterator(
        obj, table, &php_ds_htable_get_assoc_iterator_funcs, by_ref);
}
