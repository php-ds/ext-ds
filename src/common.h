#ifndef DS_COMMON_H
#define DS_COMMON_H

#include "php.h"
// #include "main/php.h"
#include "zend_exceptions.h"
// #include "zend_interfaces.h"
// #include "zend_operators.h"
// #include "ext/standard/info.h"
#include "ext/standard/php_var.h"
// #include "ext/spl/spl_iterators.h"
#include "ext/spl/spl_exceptions.h"
#include "zend_smart_str.h"
// #include "json/php_json.h"

#include "../php_ds.h"


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
 * Default namespace.
 */
#define DS_NS(cls) "Ds\\" #cls

/**
 *
 */
#define STR_AND_LEN(str) str, sizeof(str) - 1

/**
 * Combined class, name, and arginfo method entry.
 */
#define COLLECTION_ME(cls, name) \
    PHP_ME(cls, name, arginfo_##name, ZEND_ACC_PUBLIC)

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


#define SERIALIZE_SET_ZSTR(s) \
*buffer = (unsigned char *) estrndup(ZSTR_VAL((s)), ZSTR_LEN((s))); \
*length = ZSTR_LEN((s));

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


/** EXCEPTIONS **************************************************************/



#define ARRAY_ACCESS_BY_KEY_NOT_SUPPORTED() ds_throw_exception( \
    spl_ce_OutOfBoundsException, \
    "Array access by key is not supported")

#define INDEX_OUT_OF_RANGE(index, max) ds_throw_exception( \
    spl_ce_OutOfRangeException, \
    max == 0 \
        ? "Index out of range: %d" \
        : "Index out of range: %d, expected 0 <= x <= %d", \
    index, \
    max - 1)

#define OFFSET_OUT_OF_BOUNDS() ds_throw_exception( \
    spl_ce_OutOfBoundsException, \
    "Offset out of bounds")

#define ARRAY_ACCESS_PUSH_NOT_SUPPORTED() ds_throw_exception( \
    spl_ce_OutOfBoundsException, \
    "Array access push syntax is not supported")

#define KEY_NOT_FOUND() ds_throw_exception( \
    spl_ce_OutOfBoundsException, \
    "Key not found")

#define OBJ_HASH_MUST_BE_SCALAR(z) ds_throw_exception( \
    spl_ce_UnexpectedValueException, \
    "Object hash must be scalar, %s given", zend_get_type_by_const(Z_TYPE_P(z)))

#define VALUE_MUST_BE_INTEGER(z) ds_throw_exception( \
    spl_ce_UnexpectedValueException, \
    "Value must be of type integer, %d given", zend_get_type_by_const(Z_TYPE_P(z)))

#define NOT_ALLOWED_WHEN_EMPTY() ds_throw_exception( \
    spl_ce_UnderflowException, \
    "Unexpected empty state")

#define ARRAY_OR_TRAVERSABLE_REQUIRED() ds_throw_exception( \
    spl_ce_InvalidArgumentException, \
    "Value must be an array or traversable object")

#define INTEGER_INDEX_REQUIRED(z) ds_throw_exception( \
    zend_ce_type_error, \
    "Index must be of type integer, %s given", zend_get_type_by_const(Z_TYPE_P(z)))

#define ITERATION_BY_REF_NOT_SUPPORTED() ds_throw_exception( \
    zend_ce_error, \
    "Iterating by reference is not supported")

#define ACCESS_BY_REF_NOT_ALLOWED() ds_throw_exception( \
    zend_ce_error, \
    "Access by reference is not allowed")

#define UNSERIALIZE_ERROR() ds_throw_exception( \
    zend_ce_error, \
    "Failed to unserialize data")

#define RECONSTRUCTION_NOT_ALLOWED() ds_throw_exception( \
    zend_ce_error, \
    "Immutable objects may not be reconstructed")

#define MUTABILITY_NOT_ALLOWED() ds_throw_exception( \
    zend_ce_error, \
    "Immutable objects may not be changed")

/**
 *
 */
void ds_throw_exception(zend_class_entry *ce, const char *format, ...);

/*****************************************************************************/

/**
 * Similar to 'implode', joins a zval buffer using an optional 'glue'.
 * Use NULL and 0 for 'str' and 'len' to indicate an optional glue.
 */
zend_string *ds_join_zval_buffer(
    zval        *buffer,
    zend_long    size,
    char        *str,
    size_t       len
);


/**
 * Normalizes input parameters for slicing so that the implementation can focus
 * on the actual slicing. Takes care of negative values, length > size etc.
 */
void ds_normalize_slice_args(
    zend_long *offset,
    zend_long *length,
    zend_long size
);

/**
 * Sorts a zval buffer in place using the default internal compare_func.
 */
void ds_sort_zval_buffer(zval *buffer, zend_long size);

/**
 * Sorts a zval buffer in place using a user-provided, global compare function.
 */
void ds_user_sort_zval_buffer(zval *buffer, zend_long size);

/**
 * Reverses zvals between two ranges, usually a range within a buffer.
 */
void ds_reverse_zval_range(zval *x, zval *y);

/**
 * Determines if a zval is set, ie. 'isset' and 'empty'.
 */
int ds_zval_isset(zval *value, int check_empty);

/**
 * Determines if a zval is an array.
 */
bool ds_zval_is_array(zval *value);

/**
 * Determines if an array uses keys, similar to how json_encode does it.
 */
bool ds_php_array_uses_keys(HashTable *ht);

/**
 * Determines if a zval is an object and implements Traversable.
 */
bool ds_zval_is_traversable(zval *value);

#endif
