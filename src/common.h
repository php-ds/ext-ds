#ifndef DS_COMMON_H
#define DS_COMMON_H

#include "php.h"
#include "zend_exceptions.h"
#include "ext/standard/php_var.h"
#include "ext/spl/spl_exceptions.h"
#include "zend_smart_str.h"
#include "../php_ds.h"

/**
 * Used for consistent naming when working with callables.
 */
#define FCI_PARAMS zend_fcall_info fci, zend_fcall_info_cache fci_cache
#define FCI_ARGS   fci, fci_cache

/**
 * Used for consistent naming when working with variadics.
 */
#define VA_PARAMS zend_long argc, zval *argv
#define VA_ARGS   argc, argv

/**
 * Default namespace.
 */
#define PHP_DS_NS_NAME "Ds\\"
#define PHP_DS_NS(cls) PHP_DS_NS_NAME #cls

/**
 * This is useful for functions that require the string and its length.
 */
#define STR_AND_LEN(str) str, sizeof(str) - 1

/**
 * Slightly faster check when the given zval is probably a boolean.
 */
#define EXPECTED_BOOL_IS_TRUE(z) (Z_TYPE_P(z) != IS_FALSE && zend_is_true(z))

/**
 * Combined class, name, and arginfo method entry.
 */
#define PHP_DS_ME(cls, name) \
    PHP_ME(cls, name, arginfo_##cls##_##name, ZEND_ACC_PUBLIC)

/**
 *
 */
#define PHP_DS_NO_ITERATOR_OR_RETURN(obj) do { \
    if (UNEXPECTED(obj->iteratorCount)) { \
        MODIFIED_DURING_ITERATION(); \
    } \
} while (0)

/**
 * Increases the object's iterator count by 1.
 */
#define PHP_DS_INCR_ITERATOR(obj) (obj)->iteratorCount++

/**
 * Decreases the object's iterator count by 1.
 */
#define PHP_DS_DECR_ITERATOR(obj) (obj)->iteratorCount--

/**
 *
 */
#define DTOR_AND_UNDEF(z)           \
do {                                \
    zval *_z = z;                   \
    if (_z && ! Z_ISUNDEF_P(_z)) {  \
        zval_ptr_dtor(_z);          \
        ZVAL_UNDEF(_z);             \
    }                               \
} while (0)

/**
 *
 */
#define SET_AS_RETURN_AND_UNDEF(z)          \
do {                                        \
    zval *_z = z;                           \
    if (return_value) {                     \
        ZVAL_COPY_VALUE(return_value, _z);  \
        ZVAL_UNDEF(_z);                     \
    } else {                                \
        if ( ! Z_ISUNDEF_P(_z)) {           \
            zval_ptr_dtor(_z);              \
            ZVAL_UNDEF(_z);                 \
        }                                   \
    }                                       \
} while (0)

/**
 * Swaps two zval's.
 */
#define SWAP_ZVAL(a, b) \
do {                    \
    zval _t = a;        \
    a = b;              \
    b = _t;             \
} while (0)

/**
 * Adds the given zval "val" to "sum".
 */
#define DS_ADD_TO_SUM(val, sum)                                         \
do {                                                                    \
    if (Z_TYPE_P(val) == IS_LONG || Z_TYPE_P(val) == IS_DOUBLE) {       \
        fast_add_function(sum, sum, val);                               \
    } else {                                                            \
        zval _num;                                                      \
        ZVAL_COPY(&_num, val);                                          \
        convert_scalar_to_number(&_num);                                \
        fast_add_function(sum, sum, &_num);                             \
    }                                                                   \
} while (0)

/**
 * Used to replace a buffer with a new one.
 */
#define FREE_AND_REPLACE(ptr, repl) \
do {                                \
    void *_repl = repl;             \
    efree(ptr);                     \
    ptr = _repl;                    \
} while (0)

/**
 * Copies 'len' values from 'src' into 'dst'.
 */
#define COPY_ZVAL_BUFFER(dst, src, len)     \
do {                                        \
    zval *_src = src;                       \
    zval *_dst = dst;                       \
    zval *_end = src + len;                 \
    for (; _src != _end; ++_src, ++_dst) {  \
        ZVAL_COPY(_dst, _src);              \
    }                                       \
} while (0)

/**
 * Used to determine if a string zval is equal to a string literal.
 * Eg. ZVAL_EQUALS_STRING(value, "test")
 */
#define ZVAL_EQUALS_STRING(z, s) zend_string_equals_literal(Z_STR_P(z), s)

/**
 * Copies a zval into the return_value.
 */
#define RETURN_ZVAL_COPY(z)          \
do {                                 \
    zval *_z = z;                    \
    if (_z) {                        \
        ZVAL_COPY(return_value, _z); \
    }                                \
    return;                          \
} while (0)

#define SERIALIZE_SET_ZSTR(s)                                           \
    *buffer = (unsigned char *) estrndup(ZSTR_VAL((s)), ZSTR_LEN((s))); \
    *length = ZSTR_LEN((s));

#define PHP_DS_SERIALIZE_FUNCIONS(name) \
int name##_serialize(                   \
    zval                    *object,    \
    unsigned char          **buffer,    \
    size_t                  *length,    \
    zend_serialize_data     *data       \
);                                      \
int name##_unserialize(                 \
    zval                    *object,    \
    zend_class_entry        *ce,        \
    const unsigned char     *buffer,    \
    size_t                   length,    \
    zend_unserialize_data   *data       \
)

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

#define MODIFIED_DURING_ITERATION() ds_throw_exception( \
    spl_ce_LogicException, \
    "Modification during iteration is not supported")

/**
 *
 */
void ds_throw_exception(zend_class_entry *ce, const char *format, ...);

/*****************************************************************************/

/**
 * Returns the next power of 2 greater than or equal to n.
 */
uint32_t ds_next_power_of_2(uint32_t n, uint32_t min);

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
 * Allocates a zval buffer of a specified length.
 */
zval *ds_allocate_zval_buffer(zend_long length);

/**
 * Reallocates a zval buffer to a specified length.
 *
 * @param  buffer
 * @param  length The resulting length of the buffer.
 * @param  used   Number of slots currently in use in the given buffer.
 */
zval *ds_reallocate_zval_buffer(zval *buffer, zend_long length, zend_long current, zend_long used);

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
bool ds_is_array(zval *value);

/**
 * Determines if an array uses keys, similar to how json_encode does it.
 */
bool ds_php_array_uses_keys(HashTable *ht);

/**
 * Determines if a zval is an object and implements Traversable.
 */
bool ds_is_traversable(zval *value);

/**
 *
 */
void smart_str_appendz(smart_str *buffer, zval *value);

/**
 *
 */
bool ds_special_is_equal(zval *op1, zval *op2);

#endif
