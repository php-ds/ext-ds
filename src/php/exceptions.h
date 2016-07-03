#ifndef DS_EXCEPTIONS_H
#define DS_EXCEPTIONS_H

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

#endif
