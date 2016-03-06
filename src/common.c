#include "common.h"
#include "internal/php_vector.h"

void normalize_slice_params(zend_long *offset, zend_long *length, const zend_long size)
{
    zend_long idx = *offset;
    zend_long len = *length;

    // If the offset is beyond is the end or the length is zero, zero both.
    if (size == 0 || idx >= size) {
        idx = 0;
        len = 0;

    } else {

        // If index is negative, start that far from the end.
        if (idx < 0) {
            idx = MAX(0, size + idx);
        }

        // If length is given and negative, stop that far from the end.
        if (len < 0) {
            len = MAX(0, (size + len) - idx);
        }

        // If the length extends beyond the end, only go up to the end.
        if ((idx + len) > size) {
            len = MAX(0, size - idx);
        }
    }

    *offset = idx;
    *length = len;
}

zend_string *join_zval_buffer(
    zval *buffer,
    const zend_long size,
    const char *glue,
    const size_t len
) {
    smart_str str = {0};

    if (size == 0) {
        return ZSTR_EMPTY_ALLOC();
    }

    if (size == 1) {
        return zval_get_string(buffer);
    }

    if (glue && len) {
        zval *pos = buffer;
        zval *end = buffer + size - 1; // Exclude last value
        do {
            smart_str_append(&str, zval_get_string(pos));
            smart_str_appendl(&str, glue, len);
        } while (++pos != end);

        // Append last value
        smart_str_append(&str, zval_get_string(pos));

    } else {
        zval *pos = buffer;
        zval *end = buffer + size;

        do {
            smart_str_append(&str, zval_get_string(pos));
        } while (++pos != end);
    }

    smart_str_0(&str);
    return str.s;
}

void create_empty_sequence(zval *obj)
{
    ZVAL_NEW_VECTOR(obj);
}

void create_sequence(zval *obj, zval *buffer, zend_long size)
{
    ZVAL_DS_VECTOR(obj, ds_vector_from_buffer(buffer, size));
}

static int zval_user_compare(const void *a, const void *b)
{
    zval params[2];
    zval retval;

    zval *x = (zval*) a;
    zval *y = (zval*) b;

    ZVAL_COPY_VALUE(&params[0], x);
    ZVAL_COPY_VALUE(&params[1], y);

    DSG(user_compare_fci).param_count = 2;
    DSG(user_compare_fci).params      = params;
    DSG(user_compare_fci).retval      = &retval;

    if (zend_call_function(&DSG(user_compare_fci), &DSG(user_compare_fci_cache)) == SUCCESS) {
        return (int) zval_get_long(&retval);
    }

    return 0;
}

static int zval_compare(const void *a, const void *b)
{
    zval retval;

    zval *x = (zval*) a;
    zval *y = (zval*) b;

    if (compare_function(&retval, x, y) == SUCCESS) {
        return (int) zval_get_long(&retval);
    }

    return 0;
}

void sort_zval_buffer(zval *buffer, zend_long size)
{
    qsort(buffer, size, sizeof(zval), zval_compare);
}

void user_sort_zval_buffer(zval *buffer, zend_long size)
{
    qsort(buffer, size, sizeof(zval), zval_user_compare);
}

int zval_isset(zval *value, int check_empty)
{
    if (value == NULL) {
        return 0;
    }

    if ( ! check_empty) {
        return Z_TYPE_P(value) != IS_NULL;
    }

    return zend_is_true(value);
}

bool is_array(zval *value)
{
    return Z_TYPE_P(value) == IS_ARRAY;
}

bool is_traversable(zval *value)
{
    return Z_TYPE_P(value) == IS_OBJECT &&
        instanceof_function(Z_OBJCE_P(value), zend_ce_traversable);
}

bool array_uses_keys(HashTable *ht)
{
    zend_string     *key;
    zend_long       index;
    zend_long       expected = 0;

    ZEND_HASH_FOREACH_KEY(ht, index, key) {
        if (key || index != expected++) {
            return true;
        }
    }
    ZEND_HASH_FOREACH_END();
    return false;
}

void reverse_zval_range(zval *x, zval *y)
{
    for (; x < --y; ++x) SWAP_ZVAL(*x, *y);
}

int ds_default_cast_object(zval *obj, zval *return_value, int type)
{
    switch (type) {
        case IS_STRING: {
            smart_str buffer = {0};

            smart_str_appendl(&buffer, "object(", 7);
            smart_str_append (&buffer, Z_OBJCE_P(obj)->name);
            smart_str_appendc(&buffer, ')');

            smart_str_0(&buffer);
            ZVAL_STR(return_value, buffer.s);
            return SUCCESS;
        }
    }
    return FAILURE;
}
