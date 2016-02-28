#include "../common.h"
#include "../iterators/php_set_iterator.h"
#include "../handlers/php_set_handlers.h"
#include "../classes/php_ce_set.h"
#include "php_set.h"

static Set *set_init_ex(HTable *table)
{
    Set *set = ecalloc(1, sizeof(Set));
    zend_object_std_init(&set->std, set_ce);
    set->std.handlers = &set_handlers;
    set->table = table;
    return set;
}

Set *set_init()
{
    return set_init_ex(htable_init());
}

zend_object *set_create_object_ex(HTable *table)
{
    return &set_init_ex(table)->std;
}

zend_object *set_create_object(zend_class_entry *ce)
{
    return set_create_object_ex(htable_init());
}

Set *set_clone(Set *src)
{
    return set_init_ex(htable_clone(src->table));
}

zend_object *set_create_clone(Set *src)
{
    return set_create_object_ex(htable_clone(src->table));
}

void set_init_zval_ex(zval *obj, Set *set)
{
    ZVAL_OBJ(obj, &set->std);
}

void set_user_allocate(Set *set, zend_long capacity)
{
    htable_ensure_capacity(set->table, capacity);
}

uint32_t set_capacity(Set *set)
{
    return set->table->capacity;
}

void set_init_zval(zval *obj)
{
    ZVAL_OBJ(obj, &set_init()->std);
}

void set_sort_callback(Set *set)
{
    htable_sort_callback_by_key(set->table);
}

void set_sort(Set *set)
{
    htable_sort_by_key(set->table);
}

void set_sorted_callback(Set *set, zval *obj)
{
    Set *sorted = set_clone(set);
    htable_sort_callback_by_key(sorted->table);
    set_init_zval_ex(obj, sorted);
}

void set_sorted(Set *set, zval *obj)
{
    Set *sorted = set_clone(set);
    htable_sort_by_key(sorted->table);
    set_init_zval_ex(obj, sorted);
}

static inline void _set_add(Set *set, zval *value)
{
    HBucket *b;
    htable_lookup_or_next(set->table, value, &b);
    ZVAL_NULL(&b->value);
}

void set_add(Set *set, zval *value)
{
    _set_add(set, value);
}

void set_add_va(Set *set, VA_PARAMS)
{
    for (; argc != 0; argc--) {
        _set_add(set, argv++);
    }
}

static int iterator_add(zend_object_iterator *iterator, void *puser)
{
    _set_add((Set *) puser, iterator->funcs->get_current_data(iterator));
    return SUCCESS;
}

static inline void add_traversable_to_set(Set *set, zval *obj)
{
    spl_iterator_apply(obj, iterator_add, set);
}

static inline void add_array_to_set(Set *set, HashTable *array)
{
    zval *value;
    ZEND_HASH_FOREACH_VAL(array, value) {
        _set_add(set, value);
    }
    ZEND_HASH_FOREACH_END();
}

void set_add_all(Set *set, zval *values)
{
    if (values == NULL) {
        return;
    }

    if (Z_TYPE_P(values) == IS_ARRAY) {
        add_array_to_set(set, Z_ARRVAL_P(values));
        return;
    }

    if (is_traversable(values)) {
        add_traversable_to_set(set, values);
        return;
    }

    ARRAY_OR_TRAVERSABLE_REQUIRED();
}

static inline bool _set_contains(Set *set, zval *value)
{
    return htable_has_key(set->table, value);
}

bool set_contains(Set *set, zval *value)
{
    return _set_contains(set, value);
}

bool set_contains_all(Set *set, VA_PARAMS)
{
    return htable_has_keys(set->table, argc, argv);
}

static inline void set_remove(Set *set, zval *value)
{
    htable_remove(set->table, value, NULL);
}

void set_remove_va(Set *set, VA_PARAMS)
{
    while (argc--) {
        set_remove(set, argv++);
    }
}

zval *set_get(Set *set, zend_long index)
{
    HBucket *bucket = htable_lookup_by_position(set->table, index);

    if ( ! bucket) {
        INDEX_OUT_OF_RANGE(index, set->table->size);
        return NULL;
    }

    return &bucket->key;
}

zval *set_get_first(Set *set)
{
    HBucket *bucket = htable_lookup_by_position(set->table, 0);

    if ( ! bucket) {
        NOT_ALLOWED_WHEN_EMPTY();
        return NULL;
    }

    return &bucket->key;
}

zval *set_get_last(Set *set)
{
    HBucket *bucket = htable_lookup_by_position(set->table, SET_SIZE(set) - 1);

    if ( ! bucket) {
        NOT_ALLOWED_WHEN_EMPTY();
        return NULL;
    }

    return &bucket->key;
}

void set_slice(Set *set, zend_long index, zend_long length, zval *obj)
{
    HTable *sliced = htable_slice(set->table, index, length);
    set_init_zval_ex(obj, set_init_ex(sliced));
}

void set_join(Set *set, const char *glue, const size_t len, zval *return_value)
{
    zend_string *str = htable_join_keys(set->table, glue, len);
    ZVAL_STR(return_value, str);
}

void set_diff(Set *set, zval *obj, zval *return_value)
{
    zval *value;

    Set *other  = Z_SET_P(obj);
    Set *result = set_init();

    SET_FOREACH(set, value) {
        if ( ! _set_contains(other, value)) {
            _set_add(result, value);
        }
    }
    SET_FOREACH_END();

    set_init_zval_ex(return_value, result);
}

void set_assign_diff(Set *set, zval *obj)
{
    zval *value;
    Set *other  = Z_SET_P(obj);

    SET_FOREACH(other, value) {
        set_remove(set, value);
    }
    SET_FOREACH_END();
}

void set_intersect(Set *set, zval *obj, zval *return_value)
{
    zval *value;

    Set *other  = Z_SET_P(obj);
    Set *result = set_init();

    SET_FOREACH(set, value) {
        if (_set_contains(other, value)) {
            _set_add(result, value);
        }
    }
    SET_FOREACH_END();

    set_init_zval_ex(return_value, result);
}

void set_assign_intersect(Set *set, zval *obj)
{
    zval *value;
    Set *other = Z_SET_P(obj);

    SET_FOREACH(set, value) {
        if ( ! _set_contains(other, value)) {
            set_remove(set, value);
        }
    }
    SET_FOREACH_END();
}

// Returns a new Set with buffer in either A or B but not both
void set_xor(Set *set, zval *obj, zval *return_value)
{
    zval *value;

    Set *other  = Z_SET_P(obj);
    Set *result = set_init();

    SET_FOREACH(set, value) {
        if ( ! _set_contains(other, value)) {
            _set_add(result, value);
        }
    }
    SET_FOREACH_END();

    SET_FOREACH(other, value) {
        if ( ! _set_contains(set, value)) {
            _set_add(result, value);
        }
    }
    SET_FOREACH_END();

    set_init_zval_ex(return_value, result);
}

// Elements in either A or B but not both
void set_assign_xor(Set *set, zval *obj)
{
    zval *value;

    Set *other  = Z_SET_P(obj);
    Set *result = set_init();

    SET_FOREACH(set, value) {
        if (_set_contains(other, value)) {
            set_remove(set, value);
        }
    }
    SET_FOREACH_END();

    SET_FOREACH(other, value) {
        set_remove(set, value);
    }
    SET_FOREACH_END();
}

// Creates a new Set that contains the values of the current Set as well as
// the values of the provided Set.
void set_union(Set *set, zval *obj, zval *return_value)
{
    zval *value;

    Set *other  = Z_SET_P(obj);
    Set *result = set_init();

    SET_FOREACH(set, value) {
        _set_add(result, value);
    }
    SET_FOREACH_END();

    SET_FOREACH(other, value) {
        _set_add(result, value);
    }
    SET_FOREACH_END();

    set_init_zval_ex(return_value, result);
}

void set_assign_union(Set *set, zval *obj)
{
    zval *value;
    Set *other = Z_SET_P(obj);

    SET_FOREACH(other, value) {
        _set_add(set, value);
    }
    SET_FOREACH_END();
}

void set_clear(Set *set)
{
    htable_clear(set->table);
}

void set_free(Set *set)
{
    htable_destroy(set->table);
}

void set_reduce(Set *set, FCI_PARAMS, zval *initial, zval *return_value)
{
    zval carry;
    zval *value;

    if (initial == NULL) {
        ZVAL_NULL(&carry);
    } else {
        ZVAL_COPY_VALUE(&carry, initial);
    }

    SET_FOREACH(set, value) {
        zval params[2];
        zval retval;

        ZVAL_COPY_VALUE(&params[0], &carry);
        ZVAL_COPY_VALUE(&params[1], value);

        fci.param_count = 2;
        fci.params      = params;
        fci.retval      = &retval;

        if (zend_call_function(&fci, &fci_cache) == FAILURE || Z_ISUNDEF(retval)) {
            ZVAL_NULL(return_value);
            return;
        } else {
            ZVAL_COPY_VALUE(&carry, &retval);
        }

    }
    SET_FOREACH_END();
    ZVAL_COPY(return_value, &carry);
}

void set_filter_callback(Set *set, FCI_PARAMS, zval *obj)
{
    if (SET_IS_EMPTY(set)) {
        set_init_zval(obj);
        return;

    } else {
        zval *value;
        zval param;
        zval retval;

        Set *filtered = set_init();

        SET_FOREACH(set, value) {
            ZVAL_COPY_VALUE(&param, value);

            fci.param_count = 1;
            fci.params      = &param;
            fci.retval      = &retval;

            if (zend_call_function(&fci, &fci_cache) == FAILURE || Z_ISUNDEF(retval)) {
                ZVAL_NULL(obj);
                return;
            } else if (zend_is_true(&retval)) {
                _set_add(filtered, value);
            }
        }
        SET_FOREACH_END();
        set_init_zval_ex(obj, filtered);
    }
}

void set_filter(Set *set, zval *obj)
{
    if (SET_IS_EMPTY(set)) {
        set_init_zval(obj);
        return;

    } else {
        zval *value;
        Set *filtered = set_init();

        SET_FOREACH(set, value) {
            if (zend_is_true(value)) {
                _set_add(filtered, value);
            }
        }
        SET_FOREACH_END();
        set_init_zval_ex(obj, filtered);
    }
}

void set_reverse(Set *set)
{
    htable_reverse(set->table);
}

void set_reversed(Set *set, zval *obj)
{
    set_init_zval_ex(obj, set_init_ex(htable_reversed(set->table)));
}

void set_to_array(Set *set, zval *arr)
{
    zval *value;

    array_init_size(arr, set->table->size);

    HTABLE_FOREACH_KEY(set->table, value) {
        add_next_index_zval(arr, value);
        Z_TRY_ADDREF_P(value);
    }
    HTABLE_FOREACH_END();
}

int set_serialize(zval *object, unsigned char **buffer, size_t *length, zend_serialize_data *data)
{
    Set *set = Z_SET_P(object);

    php_serialize_data_t serialize_data = (php_serialize_data_t) data;
    PHP_VAR_SERIALIZE_INIT(serialize_data);

    if (set->table->size == 0) {
        SERIALIZE_SET_ZSTR(ZSTR_EMPTY_ALLOC());

    } else {

        zval *key;
        smart_str buf = {0};

        HTABLE_FOREACH_KEY(set->table, key) {
            php_var_serialize(&buf, key, &serialize_data);
        }
        HTABLE_FOREACH_END();

        smart_str_0(&buf);
        SERIALIZE_SET_ZSTR(buf.s);
        zend_string_release(buf.s);
    }

    PHP_VAR_SERIALIZE_DESTROY(serialize_data);
    return SUCCESS;
}

int set_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t length, zend_unserialize_data *data)
{
    Set *set = set_init();

    php_unserialize_data_t unserialize_data = (php_unserialize_data_t) data;

    const unsigned char *pos = buffer;
    const unsigned char *max = buffer + length;

    PHP_VAR_UNSERIALIZE_INIT(unserialize_data);

    ZVAL_OBJ(object, &set->std);

    while (*pos != '}') {

        zval *value = var_tmp_var(&unserialize_data);

        if (php_var_unserialize(value, &pos, max, &unserialize_data)) {
            var_push_dtor(&unserialize_data, value);
        } else {
            goto error;
        }

        _set_add(set, value);
    }

    if (*(++pos) != '\0') {
        goto error;
    }

    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    return SUCCESS;

error:
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    UNSERIALIZE_ERROR();
    return FAILURE;
}
