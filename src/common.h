#ifndef DS_COMMON_H
#define DS_COMMON_H

#include <stdbool.h>
#include <assert.h>

#include "php.h"
#include "main/php.h"
#include "zend_exceptions.h"
#include "zend_interfaces.h"
#include "zend_operators.h"
#include "ext/standard/info.h"
#include "ext/standard/php_var.h"
#include "ext/spl/spl_iterators.h"
#include "ext/spl/spl_exceptions.h"
#include "zend_smart_str.h"
#include "json/php_json.h"

#include "ds.h"
#include "parameters.h"
#include "exceptions.h"
#include "arginfo.h"

/**
 * Default namespace.
 */
#define DS_NS(cls) "Ds\\" #cls

#define STR_AND_LEN(str) str, sizeof(str) - 1

/**
 * Combined class, name, and arginfo method entry.
 */
#define COLLECTION_ME(cls, name) \
    PHP_ME(cls, name, arginfo_##name, ZEND_ACC_PUBLIC)

#define SERIALIZE_SET_ZSTR(s) \
*buffer = (unsigned char *) estrndup(ZSTR_VAL((s)), ZSTR_LEN((s))); \
*length = ZSTR_LEN((s));

/**
 * Used for consistent naming when working with callables.
 */
#define FCI_PARAMS zend_fcall_info fci, zend_fcall_info_cache fci_cache
#define FCI_ARGS   fci, fci_cache

/**
 * Used for consistent naming when working with variadics.
 */
#define VA_PARAMS  zend_long argc, zval *argv

/**
 * Allocates a zval buffer of a specified length.
 */
#define ALLOC_ZVAL_BUFFER(n) ecalloc(n, sizeof(zval))

/**
 * Reallocates a zval buffer to a specified length.
 */
#define REALLOC_ZVAL_BUFFER(ptr, n) \
do { \
    ptr = erealloc(ptr, n * sizeof(zval)); \
} while (0)

/**
 * Copies 'len' values from 'src' into 'dst'.
 */
#define COPY_ZVAL_BUFFER(dst, src, len) \
do { \
    zval *_src = src; \
    zval *_dst = dst; \
    zval *_end = src + len; \
    for (; _src != _end; ++_src, ++_dst) { \
        ZVAL_COPY(_dst, _src); \
    } \
} while (0)

/**
 * Used to determine if a string zval is equal to a string literal.
 * Eg. ZVAL_EQUALS_STRING(value, "test")
 */
#define ZVAL_EQUALS_STRING(z, s) (strcmp(Z_STR_P((z))->val, (s)) == 0)

/**
 * Copies a zval into the return_value.
 */
#define RETURN_ZVAL_COPY(z) \
do { \
    zval *_z = z; \
    if (_z) { \
        RETURN_ZVAL(_z, 1, 0); \
    } else { \
        return; \
    } \
} while (0)

#define DTOR_AND_UNDEF(z) \
do { \
    zval *_z = z; \
    if (_z && ! Z_ISUNDEF_P(_z)) { \
        zval_ptr_dtor(_z); \
        ZVAL_UNDEF(_z); \
    } \
} while (0)

/**
 * Copies 'src' into 'dst', then destructs 'src'
 */
#define ZVAL_COPY_DTOR(dst, src) \
do { \
    zval *_dst = dst; \
    zval *_src = src; \
    if (_dst) { \
        ZVAL_COPY(_dst, _src); \
    } \
    zval_ptr_dtor(_src); \
} while (0)

/**
 * Destructs 'dst', then copies 'src' to it.
 * Used to replace an existing value.
 */
#define ZVAL_DTOR_COPY(dst, src) \
do { \
    zval *_dst = dst; \
    zval *_src = src; \
    if (_dst) { \
        zval_ptr_dtor(_dst); \
    } \
    ZVAL_COPY(_dst, _src); \
} while (0)

#define SWAP_ZVAL(a, b) \
do { \
    zval _t = a; \
    a = b; \
    b = _t; \
} while (0)

/**
 * Used to replace a buffer with a new one.
 */
#define FREE_AND_REPLACE(ptr, repl) \
do { \
    efree(ptr); \
    ptr = repl; \
} while (0)

/**
 * Default object cast handler.
 */
int ds_default_cast_object(zval *obj, zval *return_value, int type);

/**
 * Determines if a zval is set, ie. 'isset' and 'empty'.
 */
int zval_isset(zval *value, int check_empty);

/**
 * Determines if an array uses keys, similar to how json_encode does it.
 */
bool array_uses_keys(HashTable *ht);

/**
 * Similar to 'implode', joins a zval buffer using an optional 'glue'.
 * Use NULL and 0 for 'str' and 'len' to indicate an optional glue.
 */
zend_string *join_zval_buffer(
    zval *buffer,
    const zend_long size,
    const char *str,
    const size_t len
);

/**
 * Creates an empty, default instance of a Sequence.
 */
void create_empty_sequence(zval *obj);

/**
 * Creates a default instance of a Sequence using a given buffer of zvals.
 */
void create_sequence(zval *obj, zval *buffer, zend_long size);

/**
 * Sorts a zval buffer in place using the default internal compare_func.
 */
void sort_zval_buffer(zval *buffer, zend_long size);

/**
 * Sorts a zval buffer in place using a user-provided, global compare function.
 */
void user_sort_zval_buffer(zval *buffer, zend_long size);

/**
 * Reverses zvals between two ranges, usually a range within a buffer.
 */
void reverse_zval_range(zval *x, zval *y);

/**
 * Determines if a zval is an object and implements Traversable.
 */
bool is_traversable(zval *value);

/**
 * Determines if a zval is an array.
 */
bool is_array(zval *value);

/**
 * Normalizes input parameters for slicing so that the implementation can focus
 * on the actual slicing. Takes care of negative values, length > size etc.
 */
void normalize_slice_params(
    zend_long *offset,
    zend_long *length,
    zend_long size
);

#endif
