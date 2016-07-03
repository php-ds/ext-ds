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

#include "../ds.h"


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
#define COLLECTION_NS(cls) "Ds\\" #cls

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

static void _throw_exception(zend_class_entry *ce, const char *format, ...)
{
    va_list ap;
    zend_string *str;
    va_start(ap, format); str = vstrpprintf(0, format, ap); va_end(ap);
    zend_throw_exception(ce, str->val, 0);
}

#define ARRAY_ACCESS_BY_KEY_NOT_SUPPORTED() _throw_exception( \
    spl_ce_OutOfBoundsException, \
    "Array access by key is not supported")

#define INDEX_OUT_OF_RANGE(index, max) _throw_exception( \
    spl_ce_OutOfRangeException, \
    max == 0 \
        ? "Index out of range: %d" \
        : "Index out of range: %d, expected 0 <= x <= %d", \
    index, \
    max - 1)

#define OFFSET_OUT_OF_BOUNDS() _throw_exception( \
    spl_ce_OutOfBoundsException, \
    "Offset out of bounds")

#define ARRAY_ACCESS_PUSH_NOT_SUPPORTED() _throw_exception( \
    spl_ce_OutOfBoundsException, \
    "Array access push syntax is not supported")

#define KEY_NOT_FOUND() _throw_exception( \
    spl_ce_OutOfBoundsException, \
    "Key not found")

#define OBJ_HASH_MUST_BE_SCALAR(z) _throw_exception( \
    spl_ce_UnexpectedValueException, \
    "Object hash must be scalar, %s given", zend_get_type_by_const(Z_TYPE_P(z)))

#define VALUE_MUST_BE_INTEGER(z) _throw_exception( \
    spl_ce_UnexpectedValueException, \
    "Value must be of type integer, %d given", zend_get_type_by_const(Z_TYPE_P(z)))

#define NOT_ALLOWED_WHEN_EMPTY() _throw_exception( \
    spl_ce_UnderflowException, \
    "Unexpected empty state")

#define ARRAY_OR_TRAVERSABLE_REQUIRED() _throw_exception( \
    spl_ce_InvalidArgumentException, \
    "Value must be an array or traversable object")

#define INTEGER_INDEX_REQUIRED(z) _throw_exception( \
    zend_ce_type_error, \
    "Index must be of type integer, %s given", zend_get_type_by_const(Z_TYPE_P(z)))

#define ITERATION_BY_REF_NOT_SUPPORTED() _throw_exception( \
    zend_ce_error, \
    "Iterating by reference is not supported")

#define ACCESS_BY_REF_NOT_ALLOWED() _throw_exception( \
    zend_ce_error, \
    "Access by reference is not allowed")

#define UNSERIALIZE_ERROR() _throw_exception( \
    zend_ce_error, \
    "Failed to unserialize data")

#define RECONSTRUCTION_NOT_ALLOWED() _throw_exception( \
    zend_ce_error, \
    "Immutable objects may not be reconstructed")

#define MUTABILITY_NOT_ALLOWED() _throw_exception( \
    zend_ce_error, \
    "Immutable objects may not be changed")

/*****************************************************************************/

/**
 * Similar to 'implode', joins a zval buffer using an optional 'glue'.
 * Use NULL and 0 for 'str' and 'len' to indicate an optional glue.
 */
zend_string *join_zval_buffer(
    zval        *buffer,
    zend_long    size,
    char        *str,
    size_t       len
);


/**
 * Normalizes input parameters for slicing so that the implementation can focus
 * on the actual slicing. Takes care of negative values, length > size etc.
 */
void normalize_slice_params(
    zend_long *offset,
    zend_long *length,
    zend_long size
);

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
 * Determines if a zval is set, ie. 'isset' and 'empty'.
 */
int zval_isset(zval *value, int check_empty);

/**
<<<<<<< HEAD
 * Determines if a zval is an array.
 */
bool is_array(zval *value);

/**
 * Normalizes input parameters for slicing so that the implementation can focus
 * on the actual slicing. Takes care of negative values, length > size etc.
=======
 * Determines if an array uses keys, similar to how json_encode does it.
>>>>>>> Significant include refactor, htable renaming
 */
bool array_uses_keys(HashTable *ht);

/**
 * Determines if a zval is an object and implements Traversable.
 */
bool is_traversable(zval *value);


#endif
