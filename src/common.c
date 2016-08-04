#include "common.h"

static int ds_zval_user_compare_func(const void *a, const void *b)
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

    if (zend_call_function(
            &DSG(user_compare_fci),
            &DSG(user_compare_fci_cache)) == SUCCESS) {

        return (int) zval_get_long(&retval);
    }

    return 0;
}

static int ds_zval_compare_func(const void *a, const void *b)
{
    zval retval;

    zval *x = (zval*) a;
    zval *y = (zval*) b;

    if (compare_function(&retval, x, y) == SUCCESS) {
        return (int) zval_get_long(&retval);
    }

    return 0;
}

void ds_sort_zval_buffer(zval *buffer, zend_long size)
{
    qsort(buffer, size, sizeof(zval), ds_zval_compare_func);
}

void ds_user_sort_zval_buffer(zval *buffer, zend_long size)
{
    qsort(buffer, size, sizeof(zval), ds_zval_user_compare_func);
}

int ds_zval_isset(zval *value, int check_empty)
{
    if (value == NULL) {
        return 0;
    }

    if ( ! check_empty) {
        return Z_TYPE_P(value) != IS_NULL;
    }

    return zend_is_true(value);
}

void ds_normalize_slice_args(
    zend_long *offset,
    zend_long *length,
    zend_long size
) {
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

void smart_str_appendz(smart_str *buffer, zval *value)
{
    switch (Z_TYPE_P(value)) {
        case IS_STRING:
            smart_str_append(buffer, Z_STR_P(value));
            return;
        case IS_LONG:
            smart_str_append_long(buffer, Z_LVAL_P(value));
            return;
    }

    zend_string *str = zval_get_string(value);
    smart_str_append(buffer, str);
    zend_string_free(str);
}

zend_string *ds_join_zval_buffer(
    zval        *buffer,
    zend_long    size,
    char        *glue,
    size_t       len
) {
    smart_str str = {0};

    if (size <= 0) {
        return ZSTR_EMPTY_ALLOC();
    }

    if (size == 1) {
        return zval_get_string(buffer);
    }

    // Glue is optional, will use empty string by default if NULL
    if (glue && len) {
        zval *pos = buffer;
        zval *end = buffer + size - 1; // Exclude last value

        // Append each part and the glue right up to the last value.
        do {
            smart_str_appendz(&str, pos);
            smart_str_appendl(&str, glue, len);
        } while (++pos != end);

        // Append last value
        smart_str_appendz(&str, pos);

    } else {
        zval *pos = buffer;
        zval *end = buffer + size;

        // Append each part including the last, without glue.
        do {
            smart_str_appendz(&str, pos);
        } while (++pos != end);
    }

    smart_str_0(&str);
    return str.s;
}

bool ds_is_traversable(zval *value)
{
    return Z_TYPE_P(value) == IS_OBJECT &&
        instanceof_function(Z_OBJCE_P(value), zend_ce_traversable);
}

bool ds_is_array(zval *value)
{
    return Z_TYPE_P(value) == IS_ARRAY;
}

bool ds_php_array_uses_keys(HashTable *ht)
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

void ds_reverse_zval_range(zval *x, zval *y)
{
    for (; x < --y; ++x) SWAP_ZVAL(*x, *y);
}

void ds_throw_exception(zend_class_entry *ce, const char *format, ...)
{
    va_list ap;
    zend_string *str;

    va_start(ap, format);
    str = vstrpprintf(0, format, ap);
    va_end(ap);

    zend_throw_exception(ce, str->val, 0);
    zend_string_free(str);
}
