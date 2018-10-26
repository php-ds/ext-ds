/**
 * The MIT License (MIT)
 * Copyright (c) 2018 Rudi Theunissen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including  without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH
 * THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php_decimal.h"

/**
 * Class entry.
 */
zend_class_entry *php_decimal_ce;

/**
 * Object handlers.
 */
zend_object_handlers php_decimal_handlers;


/******************************************************************************/
/*                          ERRORS AND DEBUGGING                              */
/******************************************************************************/

/**
 * Prints an mpd to stdout.
 */
static void php_decimal_print_mpd(mpd_t *mpd)
{
    char *str;
    mpd_to_sci_size(&str, mpd, 1);
    php_printf(str);
    mpd_free(str);
}

/**
 * Prints a decimal object to stdout.
 */
static void php_decimal_print(php_decimal_t *obj)
{
    char *str;
    mpd_to_sci_size(&str, PHP_DECIMAL_MPD(obj), 1);
    php_printf("---\nrepr: %s\nprec: %d\n---\n", str, obj->prec);
    mpd_free(str);
}

/**
 * Throws an exception when an invalid data type was given to create a decimal.
 */
static void php_decimal_wrong_type(zval *arg, int index, const char *expected)
{
    const char *space;
    const char *cname = get_active_class_name(&space);

    zend_throw_exception_ex(zend_ce_type_error, 0, "%s%s%s() expected parameter %d to be %s, %s given",
        cname,
        space,
        get_active_function_name(),
        index,
        expected,
        zend_zval_type_name(arg)
    );
}

/**
 * Called when a decimal is constructed using an unsupported data type.
 */
static void php_decimal_unsupported_type(zval *arg)
{
    php_decimal_wrong_type(arg, 1, "a string, integer, or decimal");
}

/**
 * Called when an operation on many failed, because the values were not given as
 * an array or traversable object.
 */
static void php_decimal_expected_iterable(zval *arg)
{
    php_decimal_wrong_type(arg, 1, "an array or traversable object");
}

/**
 * Throws an exception when an invalid precision was given, eg. -1
 */
static void php_decimal_precision_out_of_range(php_decimal_prec_t prec)
{
    zend_throw_exception_ex(spl_ce_OutOfRangeException, 0,
        "Decimal precision out of range: %d <= P <= %ld, %ld given",
        PHP_DECIMAL_MIN_PREC,
        PHP_DECIMAL_MAX_PREC,
        prec
    );
}

/**
 * Called when a memory allocation error has been detected.
 */
static void php_decimal_memory_error()
{
    zend_error(E_ERROR, "Failed to allocate memory for decimal", 0);
}

/**
 * Called when an unexpected error has occurred, to prevent undefined behaviour.
 */
static void php_decimal_unknown_error()
{
    zend_error(E_ERROR, "Failed to allocate memory for decimal", 0);
}

/**
 * Called when a rounding operation is attempted with an invalid mode.
 */
static void php_decimal_unsupported_rounding_mode(php_decimal_rounding_t mode)
{
    zend_throw_exception(spl_ce_InvalidArgumentException, "Unsupported rounding mode", 0);
}

/**
 * Called when a string conversion failed.
 */
static void php_decimal_failed_to_parse_string(zend_string * str)
{
    zend_throw_exception_ex(spl_ce_DomainException, 0, "Failed to parse string as decimal: \"%s\"", ZSTR_VAL(str));
}

/**
 * Called when a string conversion did not use all digits of the input.
 */
static void php_decimal_loss_of_data_on_string_conversion()
{
    zend_error(E_WARNING, "Loss of data on string conversion");
}

/**
 * Called when a PHP integer conversion did not use all digits of the input.
 */
static void php_decimal_loss_of_data_on_long_conversion()
{
    zend_error(E_WARNING, "Loss of data on integer conversion");
}

/**
 * Called when a decimal could not be rounded, potentially beyond max prec.
 */
static void php_decimal_failed_to_round()
{
    zend_throw_exception(spl_ce_RuntimeException, "Failed to round decimal", 0);
}

/**
 * Called when an attempt was made to divide a decimal by 0.
 */
static void php_decimal_division_by_zero_error()
{
    zend_throw_exception(zend_ce_division_by_zero_error, "Division by zero", 0);
}

/**
 * Called when a value could not be unserialized. This should never happen unless
 * someone is trying to do something they're not supposed to.
 */
static void php_decimal_unserialize_error()
{
    zend_throw_exception(spl_ce_InvalidArgumentException, "Could not unserialize decimal", 0);
}

/**
 * Called when an attempt is made to read or write object properties.
 */
static void php_decimal_object_properties_not_supported()
{
    zend_error(E_NOTICE, "Object properties are not supported");
}

/**
 * Called when a decimal is too large to be converted to double.
 */
static void php_decimal_floating_point_overflow()
{
    zend_throw_exception(spl_ce_OverflowException, "Floating point overflow", 0);
}

/**
 * Called when a decimal is too small to be converted to double, eg. 1E-1000
 */
static void php_decimal_floating_point_underflow()
{
    zend_throw_exception(spl_ce_UnderflowException, "Floating point underflow", 0);
}

/**
 * Called when a decimal is too large to be converted to int, eg. 1E+1000
 */
static void php_decimal_integer_overflow()
{
    zend_throw_exception(spl_ce_OverflowException, "Integer overflow", 0);
}

/**
 * Called when __construct is called directly on a decimal object.
 */
static void php_decimal_constructor_already_called()
{
    zend_throw_exception(spl_ce_BadMethodCallException, "Decimal objects are immutable", 0);
}

/**
 * Called when a trap is triggered in mpdecimal when calling signalling
 * functions (non-quiet). These methods usually don't have the "q" prefix in
 * their names and don't require a status argument. The non-signalling functions
 * should be used whenever we do not expect anything to go wrong or when we want
 * to handle the status directly. This is basically a catch-all error handler.
 *
 * See PHP_DECIMAL_TRAPS
 */
static void php_decimal_mpd_traphandler(mpd_context_t *ctx)
{
    uint32_t status = mpd_getstatus(ctx);

    /* Potentially out of memory, which should be caught by the PHP allocator. */
    if (status & MPD_Malloc_error) {
        php_decimal_memory_error();
        return;
    }

    /* ¯\_(ツ)_/¯ */
    php_decimal_unknown_error();
}


/******************************************************************************/
/*                             CONTEXT & PRECISION                            */
/******************************************************************************/

/**
 * The global, shared mpd context.
 */
#define php_decimal_context() (&DECIMAL_G(ctx))

/**
 * Sets the significand precision of a given decimal object.
 */
static zend_always_inline void php_decimal_set_precision(php_decimal_t *obj, php_decimal_prec_t prec)
{
    obj->prec = prec;
}

/**
 * Returns the significand precision of a given decimal object.
 */
static zend_always_inline php_decimal_prec_t php_decimal_get_precision(php_decimal_t *obj)
{
    return obj->prec;
}

/**
 * Returns true if the given precision is valid, false otherwise.
 */
static zend_always_inline zend_bool php_decimal_precision_is_valid(php_decimal_prec_t prec)
{
    return prec >= PHP_DECIMAL_MIN_PREC && prec <= PHP_DECIMAL_MAX_PREC;
}

/**
 * Checks that a given precision is valid, and returns immediately if it is not.
 */
#define PHP_DECIMAL_VALID_PRECISION_OR_RETURN(prec) do { \
    php_decimal_prec_t p = prec; \
    if (php_decimal_precision_is_valid(p) == false) { \
        php_decimal_precision_out_of_range(p); \
        return; \
    } \
} while (0)

/******************************************************************************/
/*                         ALLOC, INIT, FREE, CLONE                           */
/******************************************************************************/

/**
 * Resets all flags and allocates the data for a given MPD. We are using this
 * manual approach to avoid allocating an mpd_t when we allocate a decimal.
 *
 * We instead embed the struct itself, rather than a pointer to a new one.
 * This requires that we borrow some internals from mpdecimal to initialize an
 * already allocated mpd_t. This would usually be done by mpd_new.
 */
static zend_always_inline void php_decimal_init_mpd(mpd_t *mpd)
{
    mpd->flags  = 0;
    mpd->exp    = 0;
    mpd->digits = 0;
    mpd->len    = 0;
    mpd->alloc  = MPD_MINALLOC;
    mpd->data   = mpd_alloc(MPD_MINALLOC, sizeof(mpd_uint_t));

    if (mpd->data == NULL) {
        php_decimal_memory_error();
    }
}

/**
 * Initializes an allocated decimal object.
 */
static zend_always_inline void php_decimal_init(php_decimal_t *obj)
{
    php_decimal_init_mpd(PHP_DECIMAL_MPD(obj));
}

/**
 * Allocates a new php_decimal_t that has not been initialized. We don't want to
 * initialize a decimal until its constructor is called, so that we can check in
 * the constructor whether the object has already been initialized.
 */
static zend_always_inline php_decimal_t *php_decimal_alloc()
{
    php_decimal_t *obj = ecalloc(1, sizeof(php_decimal_t));

    /* TODO not sure if we need to check for this here, does ecalloc exit? */
    if (obj) {
        obj->std.handlers = &php_decimal_handlers;
        zend_object_std_init((zend_object *) obj, php_decimal_ce);
    } else {
        php_decimal_memory_error();
    }

    return obj;
}

/**
 * Creates a new decimal object, initialized to a given precision.
 */
static zend_always_inline php_decimal_t *php_decimal_with_prec(php_decimal_prec_t prec)
{
    php_decimal_t *obj = php_decimal_alloc();

    php_decimal_init(obj);
    php_decimal_set_precision(obj, prec);

    return obj;
}

/**
 * Creates a new decimal object, initialized to the default precision.
 */
static zend_always_inline php_decimal_t *php_decimal()
{
    return php_decimal_with_prec(PHP_DECIMAL_DEFAULT_PRECISION);
}

/**
 * Creates a custom zend_object that is also an uninitialized decimal object.
 */
static zend_object *php_decimal_create_object(zend_class_entry *ce)
{
    return (zend_object *) php_decimal_alloc();
}

/**
 * Creates a copy of the given decimal object.
 */
static inline php_decimal_t *php_decimal_create_copy(php_decimal_t *src)
{
    php_decimal_t *dst = php_decimal_with_prec(php_decimal_get_precision(src));

    mpd_copy(PHP_DECIMAL_MPD(dst), PHP_DECIMAL_MPD(src), php_decimal_context());

    return dst;
}

/**
 * Clones the given zval, which must be a decimal object.
 */
static zend_object *php_decimal_clone_obj(zval *obj)
{
    return (zend_object *) php_decimal_create_copy(Z_DECIMAL_P(obj));
}

/**
 * Frees all internal memory used by a given decimal, but not object itself.
 */
static inline void php_decimal_release(php_decimal_t *obj)
{
    /* The mpd_t is embedded so will be freed along with the object. */
    if (PHP_DECIMAL_IS_INITIALIZED(obj)) {
        mpd_free(PHP_DECIMAL_MPD(obj)->data);
    }

    zend_object_std_dtor(&obj->std);
}

/**
 * Frees a decimal object. This should only be used for objects that were
 * allocated directly and never used to create a PHP object.
 */
static inline void php_decimal_free(php_decimal_t *obj)
{
    php_decimal_release(obj);
    efree(obj);
}

/**
 * Free handler, should only free internal memory, not the object itself.
 */
static void php_decimal_free_object(zend_object *obj)
{
    php_decimal_release((php_decimal_t*) obj);
}

/******************************************************************************/
/*                                 ROUNDING                                   */
/******************************************************************************/

/**
 * Returns the parity of the integer value of a given mpd, or 1 if undefined.
 * 0 for even, 1 for odd.
 */
static int php_decimal_truncated_parity(mpd_t *mpd)
{
    if (!mpd_isinteger(mpd)) {
        if (mpd_isspecial(mpd)) {
            return 1;
        }

        PHP_DECIMAL_TEMP_MPD(tmp);
        mpd_trunc(&tmp, mpd, php_decimal_context());
        mpd = &tmp;
    }

    return mpd_isodd(mpd);
}

/**
 * This library supports both its own rounding constants and the PHP rounding
 * mode constants. The user should not have to wonder which constant to use.
 *
 * In order to support both, as well as some modes not supported by mpdecimal,
 * we need to convert the requested mode to an mpdecimal rounding mode.
 */
static php_decimal_rounding_t php_decimal_convert_to_mpd_rounding_mode(mpd_t *mpd, zend_long scale, php_decimal_rounding_t mode)
{
    switch (mode) {

        case PHP_DECIMAL_ROUND_UP:          return MPD_ROUND_UP;
        case PHP_DECIMAL_ROUND_DOWN:        return MPD_ROUND_DOWN;

        case PHP_DECIMAL_ROUND_CEILING:     return MPD_ROUND_CEILING;
        case PHP_DECIMAL_ROUND_FLOOR:       return MPD_ROUND_FLOOR;

        case PHP_DECIMAL_ROUND_HALF_UP:     return MPD_ROUND_HALF_UP;
        case PHP_DECIMAL_ROUND_HALF_DOWN:   return MPD_ROUND_HALF_DOWN;
        case PHP_DECIMAL_ROUND_HALF_EVEN:   return MPD_ROUND_HALF_EVEN;

        case PHP_DECIMAL_ROUND_TRUNCATE:    return MPD_ROUND_TRUNC;

        /* PHP constants */
        case PHP_ROUND_HALF_UP:   return MPD_ROUND_HALF_UP;
        case PHP_ROUND_HALF_DOWN: return MPD_ROUND_HALF_DOWN;
        case PHP_ROUND_HALF_EVEN: return MPD_ROUND_HALF_EVEN;

        /**
         * Special case for half-odd.
         *
         * This depends on the value's parity because half-odd is not
         * implemented by mpdecimal. We want to support the PHP constant, so we
         * need to adjust to a mode that is equivalent to an MPD rounding mode.
         */
        case PHP_ROUND_HALF_ODD:
        case PHP_DECIMAL_ROUND_HALF_ODD: {

            /* INF and NAN won't be rounded. */
            if (mpd_isspecial(mpd)) {
                return MPD_ROUND_TRUNC;

            /**
             * Determine the integer parity at the point of rounding to determine
             * which way we should round to get to the nearest odd number.
             *
             * For example, 0.12345, rounded to 4 decimal places is on the 4.
             */
            } else {
                PHP_DECIMAL_TEMP_MPD(tmp);
                mpd_shiftl(&tmp, mpd, scale, php_decimal_context());

                /* An odd digit should round down towards itself. */
                if (php_decimal_truncated_parity(&tmp)) {
                    return MPD_ROUND_HALF_DOWN;
                } else {
                    return MPD_ROUND_HALF_UP;
                }
            }
        }

        /* Couldn't determine a match, rounding mode is not supported. */
        default:
            php_decimal_unsupported_rounding_mode(mode);
            return 0;
    }
}

/**
 * Rounds a given mpd to a number of decimal places (scale), using a given
 * php decimal rounding mode. If the scale is beyond the number of decimal
 * places, trailing zeroes should be appended, increasing significance.
 */
static zend_always_inline void php_decimal_round_mpd(mpd_t *res, mpd_t *mpd, zend_long scale, php_decimal_rounding_t mode)
{
    uint32_t status = 0;

    /* No need to round if the scale is beyond the number of decimal digits */
    if (-scale <= mpd->exp) {
        mpd_qcopy(res, mpd, &status);
        return;
    }

    mpd_qsetround(php_decimal_context(), php_decimal_convert_to_mpd_rounding_mode(mpd, scale, mode));
    mpd_qrescale(res, mpd, -scale, php_decimal_context(), &status);
    mpd_qsetround(php_decimal_context(), MPD_ROUND_HALF_EVEN);

    if (status & MPD_Invalid_operation) {
        php_decimal_failed_to_round();
    }
}


/******************************************************************************/
/*                                CONVERSIONS                                 */
/******************************************************************************/

/**
 * Sets the value of an mpd to infinity.
 */
static zend_always_inline void php_decimal_mpd_set_inf(mpd_t *mpd, uint8_t sign)
{
    mpd_set_infinity(mpd);
    mpd_set_sign(mpd, sign);
}

/**
 * Sets the value of a decimal object to infinity.
 */
static zend_always_inline void php_decimal_set_inf(php_decimal_t *obj, uint8_t sign)
{
    php_decimal_mpd_set_inf(PHP_DECIMAL_MPD(obj), sign);
}

/**
 * Sets the value of an mpd to NAN.
 */
static zend_always_inline void php_decimal_mpd_set_nan(mpd_t *mpd)
{
    mpd_set_qnan(mpd);
}

/**
 * Sets the value of a decimal object to NAN.
 */
static zend_always_inline void php_decimal_set_nan(php_decimal_t *obj)
{
    php_decimal_mpd_set_nan(PHP_DECIMAL_MPD(obj));
}

/**
 * Parses a string to a given precision. Trailing zeroes are not preserved.
 */
static php_success_t php_decimal_mpd_set_string(mpd_t *mpd, zend_string *str, php_decimal_prec_t prec, zend_bool quiet)
{
    uint32_t status = 0;

    PHP_DECIMAL_WITH_PRECISION(prec, {
        mpd_qset_string(mpd, ZSTR_VAL(str), php_decimal_context(), &status);
    });

    /* Check that the conversion was successful. */
    if (status & MPD_Conversion_syntax) {
        if (!quiet) {
            php_decimal_failed_to_parse_string(str);
        }
        return FAILURE;
    }

    /* Check that we haven't lost data, ie. string wasn't parsed completely. */
    /* Can check MPD_Rounded too if we care about losing trailing zeroes. */
    if (status & MPD_Inexact) {
        php_decimal_loss_of_data_on_string_conversion();
    }

    return SUCCESS;
}

/**
 * Sets an mpd to a given double value. Will only be successful if the double is
 * a special value, ie INF, -INF and NAN.
 */
static php_success_t php_decimal_mpd_set_special_double(mpd_t *res, double dval)
{
    if (zend_isinf(dval)) {
        mpd_set_infinity(res);
        mpd_set_sign(res, signbit(dval));
        return SUCCESS;
    }

    if (zend_isnan(dval)) {
        mpd_set_qnan(res);
        return SUCCESS;
    }

    return FAILURE;
}

/**
 * Sets an mpd to a given double value. If the double is not a special value, it
 * will be cast to string first. This is useful because we don't want to parse
 * floats when constructing, but we do when comparing.
 */
static void php_decimal_mpd_set_double(mpd_t *res, double dval)
{
    zend_string *str;

    zval tmp;
    ZVAL_DOUBLE(&tmp, dval);

    str = zval_get_string(&tmp);
    php_decimal_mpd_set_string(res, str, PHP_DECIMAL_MAX_PREC, false);
    zend_string_free(str);
}

/**
 * Sets an mpd to a given long value. The precision is to determine whether data
 * was lost, eg. 123 with a precision of 2 would be 120.
 */
static void php_decimal_mpd_set_long(mpd_t *mpd, zend_long lval, php_decimal_prec_t prec)
{
    uint32_t status = 0;

    PHP_DECIMAL_WITH_PRECISION(prec, {
        mpd_qset_ssize(mpd, lval, php_decimal_context(), &status);
    });

    if (status & MPD_Rounded) {
        php_decimal_loss_of_data_on_long_conversion();
    }
}

/**
 * Sets the value of a given decimal object to zero.
 */
static zend_always_inline void php_decimal_set_zero(php_decimal_t *obj)
{
    mpd_zerocoeff(PHP_DECIMAL_MPD(obj));
}

/**
 * Attempts to parse a non-object value as a decimal, using a given precision.
 */
static zend_always_inline php_success_t php_decimal_parse_scalar_ex(mpd_t *res, zval *value, php_decimal_prec_t prec, zend_bool quiet)
{
    switch (Z_TYPE_P(value)) {
        case IS_STRING:
            return php_decimal_mpd_set_string(res, Z_STR_P(value), prec, quiet);

        case IS_LONG:
            php_decimal_mpd_set_long(res, Z_LVAL_P(value), prec);
            return SUCCESS;

        /* Should only consider special values here - float is not supported. */
        case IS_DOUBLE: {
            if (php_decimal_mpd_set_special_double(res, Z_DVAL_P(value)) == SUCCESS) {
                return SUCCESS;
            }
        }

        default: {
            if (!quiet) {
                php_decimal_unsupported_type(value);
            }
            php_decimal_mpd_set_nan(res);
            return FAILURE;
        }
    }
}

/**
 * Attempts to parse a zval with a non-decimal value.
 */
static zend_always_inline php_success_t php_decimal_parse_scalar(mpd_t *mpd, zval *value, php_decimal_prec_t prec)
{
    return php_decimal_parse_scalar_ex(mpd, value, prec, false);
}

/**
 * Attempts to parse a zval with a non-decimal value, ignoring exceptions.
 */
static zend_always_inline php_success_t php_decimal_parse_scalar_quiet(mpd_t *mpd, zval *value, php_decimal_prec_t prec)
{
    return php_decimal_parse_scalar_ex(mpd, value, prec, true);
}

/**
 * Attempts to parse a mixed value into a decimal object, using the decimal
 * object's internal mpd and precision. The precision of the result should be
 * the maximum of its already-set precision and the precision of the parsed
 * value if that value is also decimal object. Scalar values should assume that
 * the target precision is already set on the result.
 */
static zend_always_inline php_success_t php_decimal_parse_into(php_decimal_t *obj, zval *value)
{
    /* Check if is non-decimal, attempt to parse as scalar */
    if (!Z_IS_DECIMAL_P(value)) {
        return php_decimal_parse_scalar(PHP_DECIMAL_MPD(obj), value, php_decimal_get_precision(obj));

    /* Decimal object, set max precision, copy internal value. */
    } else {
        php_decimal_prec_t prec1 = php_decimal_get_precision(obj);
        php_decimal_prec_t prec2 = php_decimal_get_precision(Z_DECIMAL_P(value));

        php_decimal_set_precision(obj, MAX(prec1, prec2));
        mpd_copy(PHP_DECIMAL_MPD(obj), Z_DECIMAL_MPD_P(value), php_decimal_context());
        return SUCCESS;
    }
}

/**
 * Converts a decimal object to double. Exceptions are thrown if the value of
 * the decimal is outside of the range of a double. There is no warning or error
 * if the value of the decimal can not be represented accurately.
 */
static double php_decimal_to_double(php_decimal_t *obj)
{
    mpd_t *mpd = PHP_DECIMAL_MPD(obj);

    if (mpd_iszero(mpd)) {
        return 0;

    } else if (mpd_isspecial(mpd)) {
        return mpd_isqnan(mpd)     ? php_get_nan()
             : mpd_ispositive(mpd) ? php_get_inf() : -php_get_inf();

    } else {
        char *str;

        /* Convert the decimal to a string first. */
        size_t len = mpd_to_sci_size(&str, mpd, 1);

        /* Attempt to parse the string to double. */
        double dval = zend_strtod(str, NULL);

        /* Check if limits were reached. */
        if (zend_isinf(dval)) {
            php_decimal_floating_point_overflow();

        } else if (dval == 0 && !mpd_iszero(mpd)) {
            php_decimal_floating_point_underflow();
        }

        mpd_free(str);
        return dval;
    }
}

/**
 * Converts a decimal object to long, aka PHP int. An exception will be thrown
 * if the decimal is outside of the range of a PHP integer.
 *
 * TODO: PHP magically turn ints into floats when they get too big. Is that
 *       something we should consider here also?
 */
static zend_long php_decimal_to_long(php_decimal_t *obj)
{
    uint32_t  status = 0;
    zend_long result = 0;

    mpd_t *mpd = PHP_DECIMAL_MPD(obj);

    /* This matches PHP's behaviour. */
    if (UNEXPECTED(mpd_isspecial(mpd))) {
        return result;
    }

    /* Truncate to an integer value first, otherwise mpd_get_ssize fails. */
    if (!mpd_isinteger(mpd)) {
        PHP_DECIMAL_TEMP_MPD(tmp);
        mpd_trunc(&tmp, mpd, php_decimal_context());
        mpd = &tmp;
    }

    result = mpd_qget_ssize(mpd, &status);

    /* Check for overflow. */
    if (status & MPD_Invalid_operation) {
        php_decimal_integer_overflow();
        return 0;
    }

    return result;
}

/**
 * Converts an mpd to string, following PHP convensions.
 */
static zend_always_inline zend_string *php_decimal_mpd_to_string(mpd_t *mpd)
{
    char          *str;
    zend_string   *res;
    mpd_ssize_t    len;

    PHP_DECIMAL_CHECK_SPECIAL_STRING_RETURN(mpd);

    len = mpd_to_sci_size(&str, mpd, 1);
    res = zend_string_init(str, len, 0);
    mpd_free(str);

    return res;
}

/**
 * Converts a given php_decimal_t to a zend_string.
 */
static zend_always_inline zend_string *php_decimal_to_string(php_decimal_t *obj)
{
    return php_decimal_mpd_to_string(PHP_DECIMAL_MPD(obj));
}

/**
 * Formats an mpd, used by toFixed.
 */
static zend_string *php_decimal_format_mpd(mpd_t *mpd, zend_long places, zend_bool commas, php_decimal_rounding_t mode)
{
    char        *str;
    zend_string *res;
    smart_str    fmt = {0};

    PHP_DECIMAL_TEMP_MPD(tmp);
    PHP_DECIMAL_CHECK_SPECIAL_STRING_RETURN(mpd);

    /* Round first */
    php_decimal_round_mpd(&tmp, mpd, places, mode);

    /* Always show negative sign, but never positive. */
    smart_str_appendc(&fmt, '-');

    /* Specify whether we want to separate thousands with a comma. */
    if (commas) {
        smart_str_appendc(&fmt, ',');
    }

    /* Specify how many decimal places we want. */
    smart_str_appendc(&fmt, '.');
    smart_str_append_long(&fmt, places);

    /* Fixed point representation. */
    smart_str_appendc(&fmt, 'F');
    smart_str_0(&fmt);

    str = mpd_format(&tmp, ZSTR_VAL(fmt.s), php_decimal_context());
    res = zend_string_init(str, strlen(str), 0);

    smart_str_free(&fmt);
    mpd_free(str);

    return res;
}

/**
 * Formats a decimal object, used by toFixed.
 */
static zend_always_inline zend_string *php_decimal_format(php_decimal_t *obj, zend_long places, zend_bool commas, php_decimal_rounding_t mode)
{
    return php_decimal_format_mpd(PHP_DECIMAL_MPD(obj), places, commas, mode);
}


/******************************************************************************/
/*                                OPERATIONS                                  */
/******************************************************************************/

/**
 * Sets the result of res to op1 + op2, using the precision of res.
 *
 * We are not concerned with invalid operations such as -INF + INF because PHP
 * quietly uses NAN. The intention is to be as consistent with PHP as possible.
 */
static inline void php_decimal_add(php_decimal_t *res, mpd_t *op1, mpd_t *op2)
{
    uint32_t status = 0;

    PHP_DECIMAL_WITH_PRECISION(php_decimal_get_precision(res), {
        mpd_qadd(PHP_DECIMAL_MPD(res), op1, op2, php_decimal_context(), &status);
    });
}

/**
 * Sets the result of res to op1 - op2, using the precision of res.
 *
 * We are not concerned with invalid operations such as -INF - INF because PHP
 * quietly uses NAN. The intention is to be as consistent with PHP as possible.
 */
static inline void php_decimal_sub(php_decimal_t *res, mpd_t *op1, mpd_t *op2)
{
    uint32_t status = 0;

    PHP_DECIMAL_WITH_PRECISION(php_decimal_get_precision(res), {
        mpd_qsub(PHP_DECIMAL_MPD(res), op1, op2, php_decimal_context(), &status);
    });
}

/**
 * Sets the result of res to op1 * op2, using the precision of res.
 *
 * We are not concerned with invalid operations such as INF * NAN because PHP
 * quietly uses NAN. The intention is to be as consistent with PHP as possible.
 */
static inline void php_decimal_mul(php_decimal_t *res, mpd_t *op1, mpd_t *op2)
{
    uint32_t status = 0;

    PHP_DECIMAL_WITH_PRECISION(php_decimal_get_precision(res), {
        mpd_qmul(PHP_DECIMAL_MPD(res), op1, op2, php_decimal_context(), &status);
    });
}

/**
 * Sets the result of res to op1 / op2, using the precision of res.
 *
 * Division by zero will throw an exception, but undefined division such as
 * INF / NAN will quietly return NAN.
 */
static inline void php_decimal_div(php_decimal_t *res, mpd_t *op1, mpd_t *op2)
{
    uint32_t status = 0;

    if (mpd_iszero(op2)) {
        php_decimal_division_by_zero_error();
        php_decimal_set_inf(res, mpd_arith_sign(op1));
        return;
    }

    PHP_DECIMAL_WITH_PRECISION(php_decimal_get_precision(res), {
        mpd_qdiv(PHP_DECIMAL_MPD(res), op1, op2, php_decimal_context(), &status);
    });
}

/**
 * Sets res to the decimal remainder after dividing op1 by op2. This should not
 * be used as the equivalent of % because modulo is an integer operation.
 *
 * In order to stay consistent with div, invalid operations should quietly
 * return NAN, even though PHP throws for all invalid modulo operations.
 */
static inline void php_decimal_rem(php_decimal_t *res, mpd_t *op1, mpd_t *op2)
{
    uint32_t status = 0;

    if (mpd_iszero(op2)) {
        php_decimal_division_by_zero_error();
        php_decimal_set_inf(res, mpd_arith_sign(op1));
        return;
    }

    PHP_DECIMAL_WITH_PRECISION(php_decimal_get_precision(res), {
        mpd_qrem(PHP_DECIMAL_MPD(res), op1, op2, php_decimal_context(), &status);
    });
}

/**
 * Sets res to the integer remainder after dividing op1 by op2. This is the
 * equivalent of %, ie. integer mod.
 *
 * In order to stay consistent with div, invalid operations should quietly
 * return NAN, even though PHP throws for all invalid modulo operations.
 */
static inline void php_decimal_mod(php_decimal_t *res, mpd_t *op1, mpd_t *op2)
{
    /* Truncate op1 if not an integer, use res as temp */
    if (!mpd_isinteger(op2) && !mpd_isspecial(op2)) {
        mpd_trunc(PHP_DECIMAL_MPD(res), op2, php_decimal_context());
        op2 = PHP_DECIMAL_MPD(res);
    }

    /* Truncate op2 if not an integer. */
    if (!mpd_isinteger(op1) && !mpd_isspecial(op1)) {
        PHP_DECIMAL_TEMP_MPD(tmp);
        mpd_trunc(&tmp, op1, php_decimal_context());
        op1 = &tmp;
    }

    php_decimal_rem(res, op1, op2);
}

/**
 * Sets rem to the result of raising base to the power of exp. Anything to the
 * power of zero should equal 1.
 */
static inline void php_decimal_pow(php_decimal_t *res, mpd_t *base, mpd_t *exp)
{
    uint32_t status = 0;

    if (mpd_iszero(exp)) {
        php_decimal_mpd_set_long(PHP_DECIMAL_MPD(res), 1, php_decimal_get_precision(res));
        return;
    }

    PHP_DECIMAL_WITH_PRECISION(php_decimal_get_precision(res), {
        mpd_qpow(PHP_DECIMAL_MPD(res), base, exp, php_decimal_context(), &status);
    });
}

/**
 * Sets res to the natural log of op1.
 */
static inline void php_decimal_ln(php_decimal_t *res, mpd_t *op1)
{
    uint32_t status = 0;

    PHP_DECIMAL_WITH_PRECISION(php_decimal_get_precision(res), {
        mpd_qln(PHP_DECIMAL_MPD(res), op1, php_decimal_context(), &status);
    });
}

/**
 * Sets res to e to the power of exp.
 */
static inline void php_decimal_exp(php_decimal_t *res, mpd_t *exp)
{
    uint32_t status = 0;

    PHP_DECIMAL_WITH_PRECISION(php_decimal_get_precision(res), {
        mpd_qexp(PHP_DECIMAL_MPD(res), exp, php_decimal_context(), &status);
    });
}

/**
 * Sets res to the base-10 log of op1.
 */
static inline void php_decimal_log10(php_decimal_t *res, mpd_t *op1)
{
    uint32_t status = 0;

    PHP_DECIMAL_WITH_PRECISION(php_decimal_get_precision(res), {
        mpd_qlog10(PHP_DECIMAL_MPD(res), op1, php_decimal_context(), &status);
    });
}

/**
 * Sets res to the square root of op1. Negative values should quietly return
 * NAN, and special numbers should be copied as is.
 */
static inline void php_decimal_sqrt(php_decimal_t *res, mpd_t *op1)
{
    uint32_t status = 0;

    if (mpd_isspecial(op1)) {
        mpd_qcopy(PHP_DECIMAL_MPD(res), op1, &status);
        return;
    }

    if (mpd_isnegative(op1)) {
        php_decimal_set_nan(res);
        return;
    }

    PHP_DECIMAL_WITH_PRECISION(php_decimal_get_precision(res), {
        mpd_qsqrt(PHP_DECIMAL_MPD(res), op1, php_decimal_context(), &status);
    });
}

/**
 * Sets res to the floor of op1, ie. rounded down towards negative infinity.
 */
static inline void php_decimal_floor(php_decimal_t *res, mpd_t *op1)
{
    uint32_t status = 0;
    mpd_qfloor(PHP_DECIMAL_MPD(res), op1, php_decimal_context(), &status);
}

/**
 * Sets res to the ceiling of op1, ie. rounded up towards positive infinity.
 */
static inline void php_decimal_ceil(php_decimal_t *res, mpd_t *op1)
{
    uint32_t status = 0;
    mpd_qceil(PHP_DECIMAL_MPD(res), op1, php_decimal_context(), &status);
}

/**
 * Sets res to the integer value of op1, ie. discard all digits behind the
 * decimal point. The result is guaranteed to be an integer, unless op1 is a
 * special number in which case it should be copied as is.
 */
static inline void php_decimal_truncate(php_decimal_t *res, mpd_t *op1)
{
    uint32_t status = 0;
    mpd_qtrunc(PHP_DECIMAL_MPD(res), op1, php_decimal_context(), &status);
}

/**
 * Sets res to the value of op1 after shifting its decimal point.
 */
static inline void php_decimal_shift(php_decimal_t *res, mpd_t *op1, zend_long places)
{
    uint32_t status = 0;

    PHP_DECIMAL_TEMP_MPD(exp);
    php_decimal_mpd_set_long(&exp, places, php_decimal_get_precision(res));

    PHP_DECIMAL_WITH_PRECISION(php_decimal_get_precision(res), {
        mpd_qscaleb(PHP_DECIMAL_MPD(res), op1, &exp, php_decimal_context(), &status);
    });
}

/**
 * Sets res to the absolute value of op1.
 */
static inline void php_decimal_abs(php_decimal_t *res, mpd_t *op1)
{
    uint32_t status = 0;
    mpd_qabs(PHP_DECIMAL_MPD(res), op1, php_decimal_context(), &status);
}

/**
 * Sets res to the value of op1 with its sign inverted.
 */
static inline void php_decimal_negate(php_decimal_t *res, mpd_t *op1)
{
    uint32_t status = 0;
    mpd_qcopy_negate(PHP_DECIMAL_MPD(res), op1, &status);
}

/**
 * Sets res to the sum of all values in the given array, returning the number of
 * values that were counted or -1 if the operation failed.
 */
static zend_long php_decimal_sum_array(php_decimal_t *res, HashTable *arr)
{
    zval *value;
    php_decimal_set_zero(res);

    php_printf("start of sum array, res should be zero\n");
    php_decimal_print(res);

    ZEND_HASH_FOREACH_VAL(arr, value) {
        mpd_t *op1;
        mpd_t *op2;
        php_decimal_prec_t prec;

        php_printf("array value: ");
        php_debug_zval_dump(value, 1);

        if (Z_IS_DECIMAL_P(value)) {
            php_printf("value is a decimal\n");
            op1  = PHP_DECIMAL_MPD(res);
            op2  = Z_DECIMAL_MPD_P(value);
            prec = MAX(php_decimal_get_precision(Z_DECIMAL_P(value)), php_decimal_get_precision(res));

            php_printf("op1 = ");
            php_decimal_print_mpd(op1);
            php_printf("op2 = ");
            php_decimal_print_mpd(op2);

        } else {
            php_printf("value is NOT a decimal\n");
            PHP_DECIMAL_TEMP_MPD(tmp);
            op1  = PHP_DECIMAL_MPD(res);
            op2  = &tmp;
            prec = php_decimal_get_precision(res);

            /* Attempt to parse the value, otherwise bail out. */
            if (php_decimal_parse_scalar(&tmp, value, prec) == FAILURE) {
                return -1;
            }

            php_printf("op1 = ");
            php_decimal_print_mpd(op1);
            php_printf("\n");
            php_printf("op2 = ");
            php_decimal_print_mpd(op2);
            php_printf("\n");
        }

        php_printf("before add\n");
        php_decimal_print(res);

        /* Set precision, do add. */
        php_decimal_set_precision(res, prec);
        php_decimal_add(res, op1, op2);

        php_printf("after add\n");
        php_decimal_print(res);
    }
    ZEND_HASH_FOREACH_END();

    php_printf("after sum\n");
    php_decimal_print(res);

    return zend_hash_num_elements(arr);
}

/**
 * Sets res to the sum of all values in the given traversable, returning the
 * number of values that were counted or -1 if the operation failed.
 */
static zend_long php_decimal_sum_traversable(php_decimal_t *res, zval *values)
{
    zend_long count = 0;
    php_decimal_set_zero(res);

    /* Get the iterator from the object. */
    zend_object_iterator *iterator = Z_OBJCE_P(values)->get_iterator(Z_OBJCE_P(values), values, 0);
    if (EG(exception)) {
        goto done;
    }

    /* Attempt to rewind the iterator. */
    iterator->index = 0;
    if (iterator->funcs->rewind) {
        iterator->funcs->rewind(iterator);
        if (EG(exception)) {
            goto done;
        }
    }

    /* While the iterator has a value for us... */
    while (iterator->funcs->valid(iterator) == SUCCESS) {
        mpd_t *op1;
        mpd_t *op2;
        php_decimal_prec_t prec;

        /* Attempt to access the current value of the iterator. */
        zval *value = iterator->funcs->get_current_data(iterator);

        if (EG(exception)) {
            goto done;
        }

        if (Z_IS_DECIMAL_P(value)) {
            op1  = PHP_DECIMAL_MPD(res);
            op2  = Z_DECIMAL_MPD_P(value);
            prec = MAX(php_decimal_get_precision(Z_DECIMAL_P(value)), php_decimal_get_precision(res));

        } else {
            PHP_DECIMAL_TEMP_MPD(tmp);
            op1 = PHP_DECIMAL_MPD(res);
            op2  = &tmp;
            prec = php_decimal_get_precision(res);

            /* Attempt to parse the value, otherwise bail out. */
            if (php_decimal_parse_scalar(&tmp, value, prec) == FAILURE) {
                goto done;
            }
        }

        /* Set precision, do add. */
        php_decimal_set_precision(res, prec);
        php_decimal_add(res, op1, op2);

        /* Update count, move iterator forward. */
        count++;
        iterator->index++;
        iterator->funcs->move_forward(iterator);
        if (EG(exception)) {
            goto done;
        }
    }

  done:
    if (iterator) {
        zend_iterator_dtor(iterator);
    }

    return EG(exception) ? -1 : count;
}

/**
 * Sets res to the sum of all given values, returning the number of values that
 * were counted, or -1 if the operation failed.
 */
static zend_long php_decimal_sum(php_decimal_t *res, zval *values)
{
    /* Check array first */
    if (Z_TYPE_P(values) == IS_ARRAY) {
        return php_decimal_sum_array(res, Z_ARR_P(values));
    }

    /* Check traversable object */
    if (Z_TYPE_P(values) == IS_OBJECT && instanceof_function(Z_OBJCE_P(values), zend_ce_traversable)) {
        return php_decimal_sum_traversable(res, values);
    }

    php_decimal_expected_iterable(values);
    return -1;
}

/**
 * Sets res to the average of all given values, or NAN if the operation failed.
 */
static php_success_t php_decimal_avg(php_decimal_t *res, zval *values)
{
    zend_long count = php_decimal_sum(res, values);

    if (count == 0) {
        php_decimal_set_zero(res);
        return SUCCESS;
    }

    if (count > 0) {
        PHP_DECIMAL_TEMP_MPD(tmp);
        php_decimal_mpd_set_long(&tmp, count, PHP_DECIMAL_MAX_PREC);
        php_decimal_div(res, PHP_DECIMAL_MPD(res), &tmp);
        return SUCCESS;
    }

    php_decimal_set_nan(res);
    return FAILURE;
}

/**
 * Converts a zend opcode to a binary arithmetic function pointer.
 *
 * Returns NULL if a function is not mapped.
 */
static inline php_decimal_binary_op_t php_decimal_get_operation_for_opcode(zend_uchar opcode)
{
    switch (opcode) {
        case ZEND_ADD: return php_decimal_add;
        case ZEND_SUB: return php_decimal_sub;
        case ZEND_MUL: return php_decimal_mul;
        case ZEND_DIV: return php_decimal_div;
        case ZEND_MOD: return php_decimal_mod;
        case ZEND_POW: return php_decimal_pow;
        default:
            return NULL;
    }
}

/**
 * Attempts a binary operation on two zval's, writing the result to res.
 *
 * We don't know which of the operands is a decimal, if not both.
 */
static php_success_t php_decimal_do_binary_op(php_decimal_binary_op_t op, php_decimal_t *res, zval *op1, zval *op2)
{
    mpd_t *mpd1;
    mpd_t *mpd2;

    php_decimal_prec_t prec;

    /* Could not find a mapping, op is undefined, bail out. */
    if (op == NULL) {
        php_decimal_set_nan(res);
        return FAILURE;
    }

    if (Z_IS_DECIMAL_P(op1)) {
        if (Z_IS_DECIMAL_P(op2)) {
            /* Both operands are decimal */
            mpd1 = Z_DECIMAL_MPD_P(op1);
            mpd2 = Z_DECIMAL_MPD_P(op2);
            prec = MAX(php_decimal_get_precision(Z_DECIMAL_P(op1)), php_decimal_get_precision(Z_DECIMAL_P(op2)));

        } else {
            /* Only op1 is decimal, so attempt to parse op2. */
            PHP_DECIMAL_TEMP_MPD(tmp);
            mpd1 = Z_DECIMAL_MPD_P(op1);
            mpd2 = &tmp;
            prec = php_decimal_get_precision(Z_DECIMAL_P(op1));

            if (php_decimal_parse_scalar(mpd2, op2, prec) == FAILURE) {
                php_decimal_set_nan(res);
                return SUCCESS; /* We don't want the engine to cast. */
            }
        }
    } else {
        /* op1 is NOT a decimal, so op2 must be. */
        PHP_DECIMAL_TEMP_MPD(tmp);
        mpd1 = &tmp;
        mpd2 = Z_DECIMAL_MPD_P(op2);
        prec = php_decimal_get_precision(Z_DECIMAL_P(op2));

        if (php_decimal_parse_scalar(mpd1, op1, prec) == FAILURE) {
            php_decimal_set_nan(res);
            return SUCCESS; /* We don't want the engine to cast. */
        }
    }

    /* Parsed successfully, so we can set the parsed precision and do the op. */
    php_decimal_set_precision(res, prec);
    op(res, mpd1, mpd2);
    return SUCCESS;
}



/******************************************************************************/
/*                               SERIALIZATION                                */
/******************************************************************************/

/**
 * Serialize
 */
static php_success_t php_decimal_serialize(zval *object, unsigned char **buffer, size_t *length, zend_serialize_data *data)
{
    zval tmp;
    smart_str buf = {0};
    php_decimal_t *obj = Z_DECIMAL_P(object);

    php_serialize_data_t serialize_data = (php_serialize_data_t) data;
    PHP_VAR_SERIALIZE_INIT(serialize_data);

    /* Serialize the internal value as a string. */
    ZVAL_STR(&tmp, php_decimal_to_string(obj));
    php_var_serialize(&buf, &tmp, &serialize_data);
    zval_ptr_dtor(&tmp);

    /* Serialize the precision as an integer. */
    ZVAL_LONG(&tmp, php_decimal_get_precision(obj));
    php_var_serialize(&buf, &tmp, &serialize_data);

    PHP_VAR_SERIALIZE_DESTROY(serialize_data);

    *buffer = (unsigned char *) estrndup(ZSTR_VAL(buf.s), ZSTR_LEN(buf.s));
    *length = ZSTR_LEN(buf.s);

    smart_str_free(&buf);

    return SUCCESS;
}

/**
 * Unserialize
 */
static php_success_t php_decimal_unserialize(zval *obj, zend_class_entry *ce, const unsigned char *buffer, size_t length, zend_unserialize_data *data)
{
    zval *value;
    zval *prec;

    php_decimal_t *res = php_decimal();

    php_unserialize_data_t unserialize_data = (php_unserialize_data_t) data;

    const unsigned char *pos = buffer;
    const unsigned char *end = buffer + length;

    PHP_VAR_UNSERIALIZE_INIT(unserialize_data);

    /* Unserialize internal decimal value, which was serialized as a string. */
    value = var_tmp_var(&unserialize_data);
    if (!php_var_unserialize(value, &pos, end, &unserialize_data) || Z_TYPE_P(value) != IS_STRING) {
        goto error;
    }

    /* Unserialize precision, which was serialized as an integer. */
    prec = var_tmp_var(&unserialize_data);
    if (!php_var_unserialize(prec, &pos, end, &unserialize_data) || Z_TYPE_P(prec) != IS_LONG) {
        goto error;
    }

    /* Check that we've parsed the entire serialized string. */
    if (pos != end) {
        goto error;
    }

    /* Check precision is valid. */
    if (php_decimal_precision_is_valid(Z_LVAL_P(prec)) == false) {
        php_decimal_precision_out_of_range(Z_LVAL_P(prec));
        goto error;
    }

    /* Set the precision. */
    php_decimal_set_precision(res, Z_LVAL_P(prec));

    /* Attempt to parse the unserialized string, quietly, delegate to local error. */
    if (php_decimal_mpd_set_string(PHP_DECIMAL_MPD(res), Z_STR_P(value), Z_LVAL_P(prec), true) == FAILURE) {
        goto error;
    }

    /* Success! Set as zval and return. */
    ZVAL_DECIMAL(obj, res);
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    return SUCCESS;

error:
    php_decimal_free(res);
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    php_decimal_unserialize_error();
    return FAILURE;
}


/******************************************************************************/
/*                                COMPARISON                                  */
/******************************************************************************/

/**
 * Normalizes the result of a decimal comparison to be either -1, 0 or 1. This
 * is necessary because anything compared to NAN should be 1, even when operands
 * are flipped ie op2 <=> op1.
 */
static zend_always_inline int php_decimal_normalize_compare_result(int result, int invert)
{
    switch (result) {
        case  0:
        case  1:
        case -1:
            return invert ? -result : result;

        case PHP_DECIMAL_COMPARE_NAN:
            return 1;

        case PHP_DECIMAL_COMPARE_UNKNOWN:
            return invert ? -1 : 1;

        /* Should not be possible. */
        default:
            return 1;
    }
}

/**
 * Attempt to compare op1 and op2. It's possible that the comparison is not
 * defined (when comparing to NAN), so we should return the special comparison
 * flag that indicates an undefined result. Returning 1 here is no good because
 * operations like "greater than" would be true for NAN.
 */
static inline int php_decimal_compare_mpd(mpd_t *op1, mpd_t *op2)
{
    uint32_t status = 0;

    int result = mpd_qcmp(op1, op2, &status);

    if (UNEXPECTED(status & MPD_Invalid_operation)) {
        return PHP_DECIMAL_COMPARE_NAN;
    }

    return result;
}

/**
 * Compares two decimals using value-only comparison, precision is ignored.
 */
static inline int php_decimal_compare(php_decimal_t *op1, php_decimal_t *op2)
{
    int result = php_decimal_compare_mpd(PHP_DECIMAL_MPD(op1), PHP_DECIMAL_MPD(op2));

    if (result == 0) {
        php_decimal_prec_t prec1 = php_decimal_get_precision(op1);
        php_decimal_prec_t prec2 = php_decimal_get_precision(op2);

        return prec1 == prec2 ? 0 : (prec1 < prec2 ? -1 : 1);
    }

    return result;
}

/**
 * Compares a decimal to a non-decimal zval.
 */
static inline int php_decimal_compare_to_scalar(php_decimal_t *obj, zval *op2)
{
    while (1) {
        switch (Z_TYPE_P(op2)) {
            case IS_NULL:
            case IS_FALSE:
                return 1;

            case IS_TRUE:
                return 0;

            case IS_DOUBLE: {
                if (zend_isnan(Z_DVAL_P(op2))) {
                    return PHP_DECIMAL_COMPARE_NAN;
                }

                /* Allow comparing to float. */
                PHP_DECIMAL_TEMP_MPD(tmp);

                php_decimal_mpd_set_double(&tmp, Z_DVAL_P(op2));
                return php_decimal_compare_mpd(PHP_DECIMAL_MPD(obj), &tmp);
            }

            /* TODO not sure if this is necessary... */
            case IS_REFERENCE: {
                op2 = Z_REFVAL_P(op2);
                continue;
            }

            /* Attempt to parse the value, then compare. */
            default: {
                PHP_DECIMAL_TEMP_MPD(tmp);
                if (php_decimal_parse_scalar_quiet(&tmp, op2, PHP_DECIMAL_MAX_PREC) == SUCCESS) {
                    return php_decimal_compare_mpd(PHP_DECIMAL_MPD(obj), &tmp);
                }

                /* Can't return 1 because the value is not necessarily greater. */
                return PHP_DECIMAL_COMPARE_UNKNOWN;
            }
        }
    }
}

/**
 * Compares a decimal to a zval that could also be a decimal.
 */
static inline int php_decimal_compare_to_zval(php_decimal_t *op1, zval *op2)
{
    if (Z_IS_DECIMAL_P(op2)) {
        return php_decimal_compare(op1, Z_DECIMAL_P(op2));
    }

    return php_decimal_compare_to_scalar(op1, op2);
}


/******************************************************************************/
/*                              OBJECT HANDLERS                               */
/******************************************************************************/

/**
 * Compares two zval's, one of which must be a decimal. This is the function
 * used by the compare handler, as well as compareTo.
 */
static php_success_t php_decimal_compare_zval_to_zval(zval *retval, zval *op1, zval *op2)
{
    int result;
    int invert;

    if (Z_IS_DECIMAL_P(op1)) {
        result = php_decimal_compare_to_zval(Z_DECIMAL_P(op1), op2);
        invert = 0;
    } else {
        result = php_decimal_compare_to_zval(Z_DECIMAL_P(op2), op1);
        invert = 1;
    }

    ZVAL_LONG(retval, php_decimal_normalize_compare_result(result, invert));
    return SUCCESS;
}

/**
 * var_dump, print_r etc.
 */
static HashTable *php_decimal_get_debug_info(zval *obj, int *is_temp)
{
    zval tmp;
    HashTable *debug_info;

    ALLOC_HASHTABLE(debug_info);
    zend_hash_init(debug_info, 2, NULL, ZVAL_PTR_DTOR, 0);

    ZVAL_STR(&tmp, php_decimal_to_string(Z_DECIMAL_P(obj)));
    zend_hash_str_update(debug_info, "value", sizeof("value") - 1, &tmp);

    ZVAL_LONG(&tmp, php_decimal_get_precision(Z_DECIMAL_P(obj)));
    zend_hash_str_update(debug_info, "precision", sizeof("precision") - 1, &tmp);

    *is_temp = 1;

    return debug_info;
}

/**
 * Cast to string, int, float or bool.
 */
static php_success_t php_decimal_cast_object(zval *obj, zval *result, int type)
{
    switch (type) {
        case IS_STRING:
            ZVAL_STR(result, php_decimal_to_string(Z_DECIMAL_P(obj)));
            return SUCCESS;

        case IS_LONG:
            ZVAL_LONG(result, php_decimal_to_long(Z_DECIMAL_P(obj)));
            return SUCCESS;

        case IS_DOUBLE:
            ZVAL_DOUBLE(result, php_decimal_to_double(Z_DECIMAL_P(obj)));
            return SUCCESS;

        case _IS_BOOL:
            ZVAL_BOOL(result, 1); /* Objects are always true */
            return SUCCESS;

        default:
            return FAILURE;
    }
}

/**
 * Operator overloading.
 *
 * We don't know which of op1 and op2 is a decimal object (if not both).
 */
static php_success_t php_decimal_do_operation(zend_uchar opcode, zval *result, zval *op1, zval *op2)
{
    php_decimal_binary_op_t op;
    php_decimal_t *res;

    /* This allows for assign syntax, ie. $op1 /= $op2 */
    zval op1_copy;
    if (op1 == result) {
        ZVAL_COPY_VALUE(&op1_copy, op1);
        op1 = &op1_copy;
    }

    /* Attempt to find a binary op function for the opcode. Assume it exists. */
    op  = php_decimal_get_operation_for_opcode(opcode);
    res = php_decimal();

    /* Attempt the binary operation. */
    if (php_decimal_do_binary_op(op, res, op1, op2) == FAILURE) {
        php_decimal_free(res);
        return FAILURE;
    }

    if (op1 == &op1_copy) {
        zval_dtor(op1);
    }

    ZVAL_DECIMAL(result, res);
    return SUCCESS;
}

/**
 * Object property read - not supported.
 */
static zval *php_decimal_read_property(zval *object, zval *member, int type, void **cache_slot, zval *rv)
{
    php_decimal_object_properties_not_supported();
    return &EG(uninitialized_zval);
}

/**
 * Object property write - not supported.
 */
static void php_decimal_write_property(zval *object, zval *member, zval *value, void **cache_slot)
{
    php_decimal_object_properties_not_supported();
}

/**
 * Object property isset/empty - not supported.
 */
static int php_decimal_has_property(zval *object, zval *member, int has_set_exists, void **cache_slot)
{
    php_decimal_object_properties_not_supported();
    return 0;
}

/**
 * Object property unset - not supported.
 */
static void php_decimal_unset_property(zval *object, zval *member, void **cache_slot)
{
    php_decimal_object_properties_not_supported();
}


/******************************************************************************/
/*                            PARAMETER PARSING                               */
/******************************************************************************/

/**
 * No parameters expected, bail out if there were some.
 */
#define PHP_DECIMAL_PARAMS_PARSE_NONE() \
    if (zend_parse_parameters_none() == FAILURE) { \
        return; \
    }

/**
 * Parse a binary operation (op1 OP op2).
 */
#define PHP_DECIMAL_PARSE_BINARY_OP(op) do { \
    php_decimal_t *res = php_decimal(); \
    zval          *op2 = NULL; \
    \
    ZEND_PARSE_PARAMETERS_START(1, 1) \
        Z_PARAM_ZVAL(op2) \
    ZEND_PARSE_PARAMETERS_END(); \
    \
    php_decimal_do_binary_op(op, res, getThis(), op2); \
    RETURN_DECIMAL(res); \
} while (0)

/**
 * Parse a unary operation (OP op1).
 */
#define PHP_DECIMAL_PARSE_UNARY_OP(op) do { \
    php_decimal_t *obj = THIS_DECIMAL(); \
    php_decimal_t *res = php_decimal_with_prec(php_decimal_get_precision(obj)); \
    \
    PHP_DECIMAL_PARAMS_PARSE_NONE(); \
    \
    op(res, PHP_DECIMAL_MPD(obj)); \
    RETURN_DECIMAL(res); \
} while(0)


/******************************************************************************/
/*                              PHP CLASS METHODS                             */
/******************************************************************************/

/**
 * Decimal::__construct
 */
PHP_DECIMAL_ARGINFO(__construct, 0)
PHP_DECIMAL_ARGINFO_ZVAL(value)
PHP_DECIMAL_ARGINFO_LONG(precision)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(__construct)
{
    zval *value = NULL;
    zend_long prec = 0;

    /* Check if already constructed, because decimals are immutable */
    if (PHP_DECIMAL_IS_INITIALIZED(THIS_DECIMAL())) {
        php_decimal_constructor_already_called();
        return;
    }

    ZEND_PARSE_PARAMETERS_START(0, 2)
        Z_PARAM_OPTIONAL
        Z_PARAM_ZVAL(value)
        Z_PARAM_STRICT_LONG(prec)
    ZEND_PARSE_PARAMETERS_END();
    {
        php_decimal_t *obj = THIS_DECIMAL();

        /* No value or precision given: init using default precision, set zero. */
        if (value == NULL) {
            php_decimal_init(obj);
            php_decimal_set_precision(obj, PHP_DECIMAL_DEFAULT_PRECISION);
            php_decimal_set_zero(obj);

        /* Value given, but not precision: init using default precision, parse. */
        } else if (ZEND_NUM_ARGS() == 1) {
            php_decimal_init(obj);
            php_decimal_set_precision(obj, PHP_DECIMAL_DEFAULT_PRECISION);
            php_decimal_parse_into(obj, value);

        /* Both value and precision given: validate, init, parse. */
        } else {
            PHP_DECIMAL_VALID_PRECISION_OR_RETURN(prec);
            php_decimal_init(obj);
            php_decimal_set_precision(obj, prec);
            php_decimal_parse_into(obj, value);
        }
    }
}

/**
 * Decimal::add
 */
PHP_DECIMAL_ARGINFO_RETURN_DECIMAL(add, 1)
PHP_DECIMAL_ARGINFO_ZVAL(value)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(add)
{
    PHP_DECIMAL_PARSE_BINARY_OP(php_decimal_add);
}

/**
 * Decimal::sub
 */
PHP_DECIMAL_ARGINFO_RETURN_DECIMAL(sub, 1)
PHP_DECIMAL_ARGINFO_ZVAL(value)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(sub)
{
    PHP_DECIMAL_PARSE_BINARY_OP(php_decimal_sub);
}

/**
 * Decimal::mul
 */
PHP_DECIMAL_ARGINFO_RETURN_DECIMAL(mul, 1)
PHP_DECIMAL_ARGINFO_ZVAL(value)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(mul)
{
    PHP_DECIMAL_PARSE_BINARY_OP(php_decimal_mul);
}

/**
 * Decimal::div
 */
PHP_DECIMAL_ARGINFO_RETURN_DECIMAL(div, 1)
PHP_DECIMAL_ARGINFO_ZVAL(value)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(div)
{
    PHP_DECIMAL_PARSE_BINARY_OP(php_decimal_div);
}

/**
 * Decimal::mod
 */
PHP_DECIMAL_ARGINFO_RETURN_DECIMAL(mod, 1)
PHP_DECIMAL_ARGINFO_ZVAL(value)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(mod)
{
    PHP_DECIMAL_PARSE_BINARY_OP(php_decimal_mod);
}

/**
 * Decimal::pow
 */
PHP_DECIMAL_ARGINFO_RETURN_DECIMAL(pow, 1)
PHP_DECIMAL_ARGINFO_ZVAL(value)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(pow)
{
    PHP_DECIMAL_PARSE_BINARY_OP(php_decimal_pow);
}

/**
 * Decimal::rem
 */
PHP_DECIMAL_ARGINFO_RETURN_DECIMAL(rem, 1)
PHP_DECIMAL_ARGINFO_ZVAL(value)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(rem)
{
    PHP_DECIMAL_PARSE_BINARY_OP(php_decimal_rem);
}

/**
 * Decimal::ln
 * Decimal::log
 */
PHP_DECIMAL_ARGINFO_RETURN_DECIMAL(ln, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(ln)
{
    PHP_DECIMAL_PARSE_UNARY_OP(php_decimal_ln);
}

/**
 * Decimal::exp
 */
PHP_DECIMAL_ARGINFO_RETURN_DECIMAL(exp, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(exp)
{
    PHP_DECIMAL_PARSE_UNARY_OP(php_decimal_exp);
}

/**
 * Decimal::log10
 */
PHP_DECIMAL_ARGINFO_RETURN_DECIMAL(log10, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(log10)
{
    PHP_DECIMAL_PARSE_UNARY_OP(php_decimal_log10);
}

/**
 * Decimal::sqrt
 */
PHP_DECIMAL_ARGINFO_RETURN_DECIMAL(sqrt, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(sqrt)
{
    PHP_DECIMAL_PARSE_UNARY_OP(php_decimal_sqrt);
}

/**
 * Decimal::round
 */
PHP_DECIMAL_ARGINFO_RETURN_DECIMAL(round, 0)
PHP_DECIMAL_ARGINFO_OPTIONAL_LONG(places)
PHP_DECIMAL_ARGINFO_OPTIONAL_LONG(rounding)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(round)
{
    php_decimal_t *obj = THIS_DECIMAL();
    php_decimal_t *res = php_decimal_with_prec(php_decimal_get_precision(obj));

    zend_long places   = 0;
    zend_long rounding = PHP_DECIMAL_DEFAULT_ROUNDING;

    ZEND_PARSE_PARAMETERS_START(0, 2)
        Z_PARAM_OPTIONAL
        Z_PARAM_STRICT_LONG(places)
        Z_PARAM_STRICT_LONG(rounding)
    ZEND_PARSE_PARAMETERS_END();

    php_decimal_round_mpd(PHP_DECIMAL_MPD(res), PHP_DECIMAL_MPD(obj), places, rounding);
    RETURN_DECIMAL(res);
}

/**
 * Decimal::floor
 */
PHP_DECIMAL_ARGINFO_RETURN_DECIMAL(floor, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(floor)
{
    PHP_DECIMAL_PARSE_UNARY_OP(php_decimal_floor);
}

/**
 * Decimal::ceil
 */
PHP_DECIMAL_ARGINFO_RETURN_DECIMAL(ceil, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(ceil)
{
    PHP_DECIMAL_PARSE_UNARY_OP(php_decimal_ceil);
}


/**
 * Decimal::truncate
 */
PHP_DECIMAL_ARGINFO_RETURN_DECIMAL(truncate, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(truncate)
{
    PHP_DECIMAL_PARSE_UNARY_OP(php_decimal_truncate);
}

/**
 * Decimal::shift
 */
PHP_DECIMAL_ARGINFO_RETURN_DECIMAL(shift, 1)
PHP_DECIMAL_ARGINFO_LONG(places)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(shift)
{
    php_decimal_t *obj = THIS_DECIMAL();
    php_decimal_t *res = php_decimal_with_prec(php_decimal_get_precision(obj));

    zend_long places = 0;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STRICT_LONG(places)
    ZEND_PARSE_PARAMETERS_END();

    php_decimal_shift(res, PHP_DECIMAL_MPD(obj), places);
    RETURN_DECIMAL(res);
}

/**
 * Decimal::precision
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(precision, IS_LONG, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(precision)
{
    PHP_DECIMAL_PARAMS_PARSE_NONE();
    RETURN_LONG(php_decimal_get_precision(THIS_DECIMAL()));
}

/**
 * Decimal::signum
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(signum, IS_LONG, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(signum)
{
    PHP_DECIMAL_PARAMS_PARSE_NONE();
    RETURN_LONG(mpd_arith_sign(THIS_MPD()));
}

/**
 * Decimal::parity
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(parity, IS_LONG, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(parity)
{
    PHP_DECIMAL_PARAMS_PARSE_NONE();
    RETURN_LONG(php_decimal_truncated_parity(THIS_MPD()));
}

/**
 * Decimal::abs
 */
PHP_DECIMAL_ARGINFO_RETURN_DECIMAL(abs, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(abs)
{
    PHP_DECIMAL_PARSE_UNARY_OP(php_decimal_abs);
}

/**
 * Decimal::negate
 */
PHP_DECIMAL_ARGINFO_RETURN_DECIMAL(negate, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(negate)
{
    PHP_DECIMAL_PARSE_UNARY_OP(php_decimal_negate);
}

/**
 * Decimal::isEven
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(isEven, _IS_BOOL, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(isEven)
{
    mpd_t *mpd = THIS_MPD();
    PHP_DECIMAL_PARAMS_PARSE_NONE();
    RETURN_BOOL(mpd_isinteger(mpd) && !mpd_isodd(mpd));
}


/**
 * Decimal::isOdd
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(isOdd, _IS_BOOL, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(isOdd)
{
    mpd_t *mpd = THIS_MPD();
    PHP_DECIMAL_PARAMS_PARSE_NONE();
    RETURN_BOOL(mpd_isinteger(mpd) && mpd_isodd(mpd));
}


/**
 * Decimal::isPositive
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(isPositive, _IS_BOOL, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(isPositive)
{
    PHP_DECIMAL_PARAMS_PARSE_NONE();
    RETURN_BOOL(mpd_ispositive(THIS_MPD()));
}

/**
 * Decimal::isNegative
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(isNegative, _IS_BOOL, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(isNegative)
{
    PHP_DECIMAL_PARAMS_PARSE_NONE();
    RETURN_BOOL(mpd_isnegative(THIS_MPD()));
}

/**
 * Decimal::isNaN
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(isNaN, _IS_BOOL, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(isNaN)
{
    PHP_DECIMAL_PARAMS_PARSE_NONE();
    RETURN_BOOL(mpd_isqnan(THIS_MPD()));
}

/**
 * Decimal::isInf
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(isInf, _IS_BOOL, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(isInf)
{
    PHP_DECIMAL_PARAMS_PARSE_NONE();
    RETURN_BOOL(mpd_isinfinite(THIS_MPD()));
}

/**
 * Decimal::isInteger
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(isInteger, _IS_BOOL, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(isInteger)
{
    PHP_DECIMAL_PARAMS_PARSE_NONE();
    RETURN_BOOL(mpd_isinteger(THIS_MPD()));
}

/**
 * Decimal::isZero
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(isZero, _IS_BOOL, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(isZero)
{
    PHP_DECIMAL_PARAMS_PARSE_NONE();
    RETURN_BOOL(mpd_iszero(THIS_MPD()));
}

/**
 * Decimal::toFixed
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(toFixed, IS_STRING, 0)
PHP_DECIMAL_ARGINFO_OPTIONAL_LONG(places)
PHP_DECIMAL_ARGINFO_OPTIONAL_BOOL(commas)
PHP_DECIMAL_ARGINFO_OPTIONAL_LONG(rounding)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(toFixed)
{
    zend_long places   = 0;
    zend_bool commas   = false;
    zend_long rounding = PHP_DECIMAL_DEFAULT_ROUNDING;

    ZEND_PARSE_PARAMETERS_START(0, 3)
        Z_PARAM_OPTIONAL
        Z_PARAM_STRICT_LONG(places)
        Z_PARAM_BOOL(commas)
        Z_PARAM_STRICT_LONG(rounding)
    ZEND_PARSE_PARAMETERS_END();

    RETURN_STR(php_decimal_format(THIS_DECIMAL(), places, commas, rounding));
}


/**
 * Decimal::__toString
 * Decimal::toString
 * Decimal::jsonSerialize
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(toString, IS_STRING, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(toString)
{
    PHP_DECIMAL_PARAMS_PARSE_NONE();
    RETURN_STR(php_decimal_to_string(THIS_DECIMAL()));
}

/**
 * Decimal::toInt
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(toInt, IS_LONG, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(toInt)
{
    PHP_DECIMAL_PARAMS_PARSE_NONE();
    RETURN_LONG(php_decimal_to_long(THIS_DECIMAL()));
}

/**
 * Decimal::toFloat
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(toFloat, IS_DOUBLE, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(toFloat)
{
    PHP_DECIMAL_PARAMS_PARSE_NONE();
    RETURN_DOUBLE(php_decimal_to_double(THIS_DECIMAL()));
}

/**
 * Decimal::copy
 */
PHP_DECIMAL_ARGINFO_RETURN_DECIMAL(copy, 0)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(copy)
{
    PHP_DECIMAL_PARAMS_PARSE_NONE();
    RETURN_DECIMAL(php_decimal_create_copy(THIS_DECIMAL()));
}

/**
 * Decimal::equals
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(equals, _IS_BOOL, 1)
PHP_DECIMAL_ARGINFO_ZVAL(other)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(equals)
{
    zval *op2 = NULL;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_ZVAL(op2)
    ZEND_PARSE_PARAMETERS_END();

    RETURN_BOOL(php_decimal_compare_to_zval(THIS_DECIMAL(), op2) == 0);
}

/**
 * Decimal::compareTo
 */
PHP_DECIMAL_ARGINFO_RETURN_TYPE(compareTo, IS_LONG, 1)
PHP_DECIMAL_ARGINFO_ZVAL(other)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(compareTo)
{
    zval *op2 = NULL;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_ZVAL(op2)
    ZEND_PARSE_PARAMETERS_END();

    php_decimal_compare_zval_to_zval(return_value, getThis(), op2);
}

PHP_DECIMAL_ARGINFO_RETURN_DECIMAL(sum, 1)
PHP_DECIMAL_ARGINFO_ZVAL(values)
PHP_DECIMAL_ARGINFO_OPTIONAL_LONG(precision)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(sum)
{
    php_decimal_t     *res    = php_decimal();
    zval              *values = NULL;
    php_decimal_prec_t prec   = PHP_DECIMAL_DEFAULT_PRECISION;

    ZEND_PARSE_PARAMETERS_START(1, 2)
        Z_PARAM_ZVAL(values)
        Z_PARAM_OPTIONAL
        Z_PARAM_STRICT_LONG(prec)
    ZEND_PARSE_PARAMETERS_END();

    PHP_DECIMAL_VALID_PRECISION_OR_RETURN(prec);
    php_decimal_set_precision(res, prec);
    php_decimal_sum(res, values);
    RETURN_DECIMAL(res);
}

PHP_DECIMAL_ARGINFO_RETURN_DECIMAL(avg, 1)
PHP_DECIMAL_ARGINFO_ZVAL(values)
PHP_DECIMAL_ARGINFO_OPTIONAL_LONG(precision)
PHP_DECIMAL_ARGINFO_END()
PHP_DECIMAL_METHOD(avg)
{
    php_decimal_t     *res    = php_decimal();
    zval              *values = NULL;
    php_decimal_prec_t prec   = PHP_DECIMAL_DEFAULT_PRECISION;

    ZEND_PARSE_PARAMETERS_START(1, 2)
        Z_PARAM_ZVAL(values)
        Z_PARAM_OPTIONAL
        Z_PARAM_STRICT_LONG(prec)
    ZEND_PARSE_PARAMETERS_END();

    PHP_DECIMAL_VALID_PRECISION_OR_RETURN(prec);
    php_decimal_set_precision(res, prec);
    php_decimal_avg(res, values);
    RETURN_DECIMAL(res);
}

/******************************************************************************/
/*                                 CLASS ENTRY                                */
/******************************************************************************/

static zend_function_entry decimal_methods[] = {
    PHP_DECIMAL_ME(__construct)
    PHP_DECIMAL_ME(copy)

    PHP_DECIMAL_ME(add)
    PHP_DECIMAL_ME(sub)
    PHP_DECIMAL_ME(mul)
    PHP_DECIMAL_ME(div)
    PHP_DECIMAL_ME(rem)
    PHP_DECIMAL_ME(mod)
    PHP_DECIMAL_ME(pow)

    PHP_DECIMAL_ME(ln)
    PHP_DECIMAL_ME(exp)
    PHP_DECIMAL_ME(log10)
    PHP_DECIMAL_ME(sqrt)

    PHP_DECIMAL_ME(floor)
    PHP_DECIMAL_ME(ceil)
    PHP_DECIMAL_ME(truncate)

    PHP_DECIMAL_ME(round)
    PHP_DECIMAL_ME(shift)
    PHP_DECIMAL_ME(precision)

    PHP_DECIMAL_ME(signum)
    PHP_DECIMAL_ME(parity)

    PHP_DECIMAL_ME(abs)
    PHP_DECIMAL_ME(negate)

    PHP_DECIMAL_ME(isEven)
    PHP_DECIMAL_ME(isOdd)

    PHP_DECIMAL_ME(isPositive)
    PHP_DECIMAL_ME(isNegative)

    PHP_DECIMAL_ME(isNaN)
    PHP_DECIMAL_ME(isInf)
    PHP_DECIMAL_ME(isInteger)
    PHP_DECIMAL_ME(isZero)

    PHP_DECIMAL_ME(toFixed)
    PHP_DECIMAL_ME(toString)
    PHP_DECIMAL_ME(toInt)
    PHP_DECIMAL_ME(toFloat)

    PHP_DECIMAL_ME(equals)
    PHP_DECIMAL_ME(compareTo)

    /* Static methods */
    PHP_DECIMAL_STATIC_ME(sum)
    PHP_DECIMAL_STATIC_ME(avg)

    /* Aliases:    Alias          Defined */
    PHP_DECIMAL_AL(__toString,    toString)
    PHP_DECIMAL_AL(jsonSerialize, toString)
    PHP_FE_END
};

/**
 * Decimal functions.
 */
static zend_function_entry decimal_functions[] = {
    PHP_FE_END
};

/**
 * Sets the object handlers.
 */
static void php_decimal_register_class_handlers()
{
    memcpy(&php_decimal_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    /**
     * No need for an offset because we're casting back and forth between
     * zend_object and php_decimal. Malloc should know the size of the block so
     * when the engine frees the zend_object, it will free the php_decimal. We
     * also don't have any class properties and the class is final.
     */
    php_decimal_handlers.offset = 0;

    php_decimal_handlers.free_obj         = php_decimal_free_object;
    php_decimal_handlers.clone_obj        = php_decimal_clone_obj;
    php_decimal_handlers.cast_object      = php_decimal_cast_object;
    php_decimal_handlers.compare          = php_decimal_compare_zval_to_zval;
    php_decimal_handlers.do_operation     = php_decimal_do_operation;
    php_decimal_handlers.get_debug_info   = php_decimal_get_debug_info;
    php_decimal_handlers.read_property    = php_decimal_read_property;
    php_decimal_handlers.write_property   = php_decimal_write_property;
    php_decimal_handlers.has_property     = php_decimal_has_property;
    php_decimal_handlers.unset_property   = php_decimal_unset_property;
}

/**
 * Registers the class entry and constants.
 */
static void php_decimal_register_class_entry()
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, PHP_DECIMAL_FQCN, decimal_methods);

    php_decimal_ce = zend_register_internal_class(&ce);
    php_decimal_ce->ce_flags      |= ZEND_ACC_FINAL;
    php_decimal_ce->create_object  = php_decimal_create_object;
    php_decimal_ce->serialize      = php_decimal_serialize;
    php_decimal_ce->unserialize    = php_decimal_unserialize;

    zend_class_implements(php_decimal_ce, 1, php_json_serializable_ce);

    PHP_DECIMAL_LONG_CONSTANT("ROUND_UP",           PHP_DECIMAL_ROUND_UP);
    PHP_DECIMAL_LONG_CONSTANT("ROUND_DOWN",         PHP_DECIMAL_ROUND_DOWN);
    PHP_DECIMAL_LONG_CONSTANT("ROUND_CEILING",      PHP_DECIMAL_ROUND_CEILING);
    PHP_DECIMAL_LONG_CONSTANT("ROUND_FLOOR",        PHP_DECIMAL_ROUND_FLOOR);
    PHP_DECIMAL_LONG_CONSTANT("ROUND_HALF_UP",      PHP_DECIMAL_ROUND_HALF_UP);
    PHP_DECIMAL_LONG_CONSTANT("ROUND_HALF_DOWN",    PHP_DECIMAL_ROUND_HALF_DOWN);
    PHP_DECIMAL_LONG_CONSTANT("ROUND_HALF_EVEN",    PHP_DECIMAL_ROUND_HALF_EVEN);
    PHP_DECIMAL_LONG_CONSTANT("ROUND_HALF_ODD",     PHP_DECIMAL_ROUND_HALF_ODD);
    PHP_DECIMAL_LONG_CONSTANT("ROUND_TRUNCATE",     PHP_DECIMAL_ROUND_TRUNCATE);

    PHP_DECIMAL_LONG_CONSTANT("DEFAULT_PRECISION",  PHP_DECIMAL_DEFAULT_PRECISION);
    PHP_DECIMAL_LONG_CONSTANT("DEFAULT_ROUNDING",   PHP_DECIMAL_DEFAULT_ROUNDING);

    PHP_DECIMAL_LONG_CONSTANT("MIN_PRECISION", PHP_DECIMAL_MIN_PREC);
    PHP_DECIMAL_LONG_CONSTANT("MAX_PRECISION", PHP_DECIMAL_MAX_PREC);
}

/******************************************************************************/
/*                                    INI                                     */
/******************************************************************************/

PHP_INI_BEGIN()
PHP_INI_END()

/******************************************************************************/
/*                                   MODULE                                   */
/******************************************************************************/

ZEND_DECLARE_MODULE_GLOBALS(decimal)

static void php_decimal_init_globals(zend_decimal_globals *g)
{
    memset(g, 0, sizeof(zend_decimal_globals));
}

/**
 * Module entry
 */
zend_module_entry decimal_module_entry = {
    STANDARD_MODULE_HEADER,
    PHP_DECIMAL_EXTNAME,
    decimal_functions,
    PHP_MINIT(decimal),
    PHP_MSHUTDOWN(decimal),
    PHP_RINIT(decimal),
    PHP_RSHUTDOWN(decimal),
    PHP_MINFO(decimal),
    PHP_DECIMAL_VERSION,
    STANDARD_MODULE_PROPERTIES
};

/**
 * Module information displayed by phpinfo()
 */
PHP_MINFO_FUNCTION(decimal)
{
    php_info_print_table_start();
    php_info_print_table_row(2, "decimal support", "enabled");
    php_info_print_table_row(2, "decimal version", PHP_DECIMAL_VERSION);
    php_info_print_table_row(2, "libmpdec version", MPD_VERSION);
    php_info_print_table_end();

    DISPLAY_INI_ENTRIES();
}

/**
 * Set custom allocators.
 */
static void *php_decimal_mpd_malloc(size_t size)
{
    return emalloc(size);
}

static void *php_decimal_mpd_calloc(size_t nmemb, size_t size)
{
    return ecalloc(nmemb, size);
}

static void *php_decimal_mpd_realloc(void *ptr, size_t size)
{
    return erealloc(ptr, size);
}

static void php_decimal_mpd_free(void *ptr)
{
    efree(ptr);
}

/**
 * Module startup
 */
PHP_MINIT_FUNCTION(decimal)
{
    php_decimal_register_class_entry();
    php_decimal_register_class_handlers();

    ZEND_INIT_MODULE_GLOBALS(decimal, php_decimal_init_globals, NULL);

    /* Initialize the shared context */
    mpd_init(php_decimal_context(), PHP_DECIMAL_DEFAULT_PRECISION);

    /* Set default rounding */
    mpd_qsettraps(php_decimal_context(), PHP_DECIMAL_TRAPS);
    mpd_qsetround(php_decimal_context(), PHP_DECIMAL_DEFAULT_ROUNDING);

    /* Set custom memory allocation functions */
    mpd_callocfunc  = php_decimal_mpd_calloc;
    mpd_mallocfunc  = php_decimal_mpd_malloc;
    mpd_reallocfunc = php_decimal_mpd_realloc;
    mpd_free        = php_decimal_mpd_free;

    /* */
    mpd_traphandler = php_decimal_mpd_traphandler;

    return SUCCESS;
}

/**
 * Module shutdown
 */
PHP_MSHUTDOWN_FUNCTION(decimal)
{
    return SUCCESS;
}

/**
 * Request startup
 */
PHP_RINIT_FUNCTION(decimal)
{
#if defined(COMPILE_DL_DECIMAL) && defined(ZTS)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif

    return SUCCESS;
}

/**
 * Request shutdown
 */
PHP_RSHUTDOWN_FUNCTION(decimal)
{
    return SUCCESS;
}

#ifdef COMPILE_DL_DECIMAL
#ifdef ZTS
    ZEND_TSRMLS_CACHE_DEFINE();
#endif
    ZEND_GET_MODULE(decimal)
#endif
