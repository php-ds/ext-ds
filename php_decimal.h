/*
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
#ifndef HAVE_PHP_DECIMAL_H
#define HAVE_PHP_DECIMAL_H

#ifdef PHP_WIN32
#   define PHP_DECIMAL_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#   define PHP_DECIMAL_API __attribute__ ((visibility("default")))
#else
#   define PHP_DECIMAL_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#include <stdio.h>
#include <stdbool.h>
#include "php.h"
#include "zend_smart_str.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"
#include "ext/standard/info.h"
#include "ext/standard/php_var.h"
#include "ext/standard/php_math.h"
#include "ext/standard/php_string.h"
#include "ext/json/php_json.h"
#include "ext/spl/spl_exceptions.h"
#include "mpdecimal.h"

#define PHP_DECIMAL_VERSION "0.0.0"

#define PHP_DECIMAL_EXTNAME "decimal"

#define PHP_DECIMAL_CLASS "Decimal"

#define PHP_DECIMAL_NAMESPACE "Decimal"

#define PHP_DECIMAL_FQCN PHP_DECIMAL_NAMESPACE "\\" PHP_DECIMAL_CLASS

/**
 * Special comparison result flags that are not -1, 0, or 1.
 */
#define PHP_DECIMAL_COMPARE_NAN      2
#define PHP_DECIMAL_COMPARE_UNKNOWN  3

/**
 * Rounding modes.
 */
#define PHP_DECIMAL_ROUND_UP         101
#define PHP_DECIMAL_ROUND_DOWN       102
#define PHP_DECIMAL_ROUND_CEILING    103
#define PHP_DECIMAL_ROUND_FLOOR      104
#define PHP_DECIMAL_ROUND_HALF_UP    105
#define PHP_DECIMAL_ROUND_HALF_DOWN  106
#define PHP_DECIMAL_ROUND_HALF_EVEN  107
#define PHP_DECIMAL_ROUND_HALF_ODD   108
#define PHP_DECIMAL_ROUND_TRUNCATE   109

/**
 * Defines which conditions call the trap handler.
 */
#define PHP_DECIMAL_TRAPS  (MPD_Errors | MPD_Traps)

/**
 * Defaults
 */
#define PHP_DECIMAL_DEFAULT_PRECISION  28
#define PHP_DECIMAL_DEFAULT_ROUNDING   PHP_DECIMAL_ROUND_HALF_EVEN

/**
 * TODO: Not sure if we need to free these with mpd_del(&name)
 */
#define PHP_DECIMAL_TEMP_MPD(name) \
        mpd_uint_t name##_data[MPD_MINALLOC_MAX]; \
        mpd_t      name = {MPD_STATIC|MPD_STATIC_DATA, 0, 0, 0, MPD_MINALLOC_MAX, name##_data} \

/**
 * Checks if a decimal object has been constructed or otherwise intialized.
 */
#define PHP_DECIMAL_IS_INITIALIZED(d) (PHP_DECIMAL_MPD(d)->data != NULL)

/**
 * Precision constants.
 */
#define PHP_DECIMAL_MIN_PREC 1
#define PHP_DECIMAL_MAX_PREC MPD_MAX_PREC

/**
 * Used to perform a task using a temporary precision. There is no need to reset
 * this back to what it was before because all operations will set the precision
 * first. We're also assuming that all bounds checking has been done on _prec.
 */
#define PHP_DECIMAL_WITH_PRECISION(_prec, task) do { \
    php_decimal_context()->prec = _prec; \
    task; \
} while(0)

/**
 * Object and zval access
 */
#define PHP_DECIMAL_MPD(p)      (&(p)->mpd)
#define Z_DECIMAL_P(z)          ((php_decimal_t*) Z_OBJ_P(z))
#define Z_DECIMAL_MPD_P(z)      PHP_DECIMAL_MPD(Z_DECIMAL_P(z))
#define Z_IS_DECIMAL_P(d)       (Z_TYPE_P(d) == IS_OBJECT && Z_OBJCE_P(d) == php_decimal_ce)
#define ZVAL_DECIMAL(z, dec)    ZVAL_OBJ(z, (zend_object*) dec)
#define THIS_DECIMAL()          Z_DECIMAL_P(getThis())
#define THIS_MPD()              Z_DECIMAL_MPD_P(getThis())

#define RETURN_DECIMAL(dec) \
do { \
    php_decimal_t *_dec = dec; \
    if (_dec) { \
        ZVAL_DECIMAL(return_value, _dec); \
    } else { \
        ZVAL_NULL(return_value); \
    } \
    return; \
} while(0)

/**
 * Special numbers should use the 3-letter uppercase representation. This macro
 * checks if mpd is special, and returns a zend_string immediately if it is.
 */
#define PHP_DECIMAL_CHECK_SPECIAL_STRING_RETURN(mpd) do { \
    if (UNEXPECTED(mpd_isspecial(mpd))) { \
        if (mpd_isqnan(mpd)) { \
            return zend_string_init("NAN", 3, 0); \
        } else { \
            return mpd_ispositive(mpd) \
                ? zend_string_init( "INF", 3, 0) \
                : zend_string_init("-INF", 4, 0); \
        } \
    } \
} while (0)

/**
 * Class, method, and function entry
 */
#define PHP_DECIMAL_METHOD(name) \
    PHP_METHOD(Decimal, name)

#define PHP_DECIMAL_FUNCTION(name) \
    PHP_FUNCTION(name)

#define PHP_DECIMAL_STATIC_ME(name) \
    PHP_ME(Decimal, name, php_decimal_method_arginfo_##name, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)

#define PHP_DECIMAL_ME(name) \
    PHP_ME(Decimal, name, php_decimal_method_arginfo_##name, ZEND_ACC_PUBLIC)

#define PHP_DECIMAL_FE(name) \
    ZEND_NS_FE(PHP_DECIMAL_NAMESPACE, name, php_decimal_function_arginfo_##name)

#define PHP_DECIMAL_AL(alias, name) \
    PHP_MALIAS(Decimal, alias, name, php_decimal_method_arginfo_##name, ZEND_ACC_PUBLIC)

#define PHP_DECIMAL_LONG_CONSTANT(name, value) \
    zend_declare_class_constant_long(php_decimal_ce, name, sizeof(name) - 1, value);

/**
 * Arginfo
 */
#define PHP_DECIMAL_ARGINFO_EX(name, required_num_args, entry_type) \
    ZEND_BEGIN_ARG_INFO_EX(php_decimal_##entry_type##_arginfo_##name, 0, 0, required_num_args)

#if PHP_VERSION_ID >= 70200
#define PHP_DECIMAL_ARGINFO_RETURN_DECIMAL_EX(name, required_num_args, entry_type) \
    static const zend_internal_arg_info php_decimal_##entry_type##_arginfo_##name[] = { \
        {(const char*)(zend_uintptr_t)(required_num_args), ZEND_TYPE_ENCODE_CLASS_CONST(PHP_DECIMAL_FQCN, 0), 0, 0},
#else
#define PHP_DECIMAL_ARGINFO_RETURN_DECIMAL_EX(name, required_num_args, entry_type) \
    static const zend_internal_arg_info php_decimal_##entry_type##_arginfo_##name[] = { \
        {(const char*)(zend_uintptr_t)(required_num_args), PHP_DECIMAL_FQCN, IS_OBJECT, 0, 0, 0},
#endif

#if PHP_VERSION_ID >= 70200
#define PHP_DECIMAL_ARGINFO_RETURN_TYPE_EX(name, type, required_num_args, entry_type) \
    ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_decimal_##entry_type##_arginfo_##name, 0, required_num_args, type, 0)
#else
#define PHP_DECIMAL_ARGINFO_RETURN_TYPE_EX(name, type, required_num_args, entry_type) \
    ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_decimal_##entry_type##_arginfo_##name, 0, required_num_args, type, 0, 0)
#endif

#if PHP_VERSION_ID >= 70200
#define PHP_DECIMAL_ARGINFO_DECIMAL(name) {#name, ZEND_TYPE_ENCODE_CLASS_CONST(PHP_DECIMAL_FQCN, 0), 0, 0},
#else
#define PHP_DECIMAL_ARGINFO_DECIMAL(name) {#name, PHP_DECIMAL_FQCN, IS_OBJECT, 0, 0, 0},
#endif

#define PHP_DECIMAL_ARGINFO(name, required_num_args) \
    PHP_DECIMAL_ARGINFO_EX(name, required_num_args, method)

#define PHP_DECIMAL_ARGINFO_RETURN_DECIMAL(name, required_num_args) \
    PHP_DECIMAL_ARGINFO_RETURN_DECIMAL_EX(name, required_num_args, method)

#define PHP_DECIMAL_ARGINFO_RETURN_TYPE(name, type, required_num_args) \
    PHP_DECIMAL_ARGINFO_RETURN_TYPE_EX(name, type, required_num_args, method)

#define PHP_DECIMAL_FUNCTION_ARGINFO(name, required_num_args) \
    PHP_DECIMAL_ARGINFO_EX(name, required_num_args, function)

#define PHP_DECIMAL_FUNCTION_ARGINFO_RETURN_DECIMAL(name, required_num_args) \
    PHP_DECIMAL_ARGINFO_RETURN_DECIMAL_EX(name, required_num_args, function)

#define PHP_DECIMAL_FUNCTION_ARGINFO_RETURN_TYPE(name, type, required_num_args) \
    PHP_DECIMAL_ARGINFO_RETURN_TYPE_EX(name, type, required_num_args, function)

#define PHP_DECIMAL_ARGINFO_ZVAL(name) \
    ZEND_ARG_INFO(0, name)

#define PHP_DECIMAL_ARGINFO_LONG(name) \
    ZEND_ARG_TYPE_INFO(0, name, IS_LONG, 0)

#define PHP_DECIMAL_ARGINFO_FLOAT(name) \
    ZEND_ARG_TYPE_INFO(0, name, IS_DOUBLE, 0)

#define PHP_DECIMAL_ARGINFO_OPTIONAL_LONG(name) \
    ZEND_ARG_TYPE_INFO(0, name, IS_LONG, 1)

#define PHP_DECIMAL_ARGINFO_OPTIONAL_BOOL(name) \
    ZEND_ARG_TYPE_INFO(0, name, _IS_BOOL, 1)

#define PHP_DECIMAL_ARGINFO_END() \
    ZEND_END_ARG_INFO()

/**
 * Custom definitions.
 */
typedef mpd_ssize_t php_decimal_prec_t;
typedef int         php_decimal_rounding_t;
typedef int         php_success_t;

/**
 * PHP decimal object.
 */
typedef struct php_decimal {
    zend_object         std;    /* Zend object, must be first! */
    mpd_t               mpd;    /* Embedded MPD object */
    php_decimal_prec_t  prec;   /* Precision */
} php_decimal_t;

/**
 * Operation function definitions.
 */
typedef void (*php_decimal_binary_op_t)(php_decimal_t *res, mpd_t *op1, mpd_t *op2);
typedef void (*php_decimal_unary_op_t)(php_decimal_t *res, mpd_t *op1);

/**
 * Module and class entry
 */
extern zend_module_entry php_decimal_module_entry;
extern zend_class_entry *php_decimal_ce;

ZEND_MINIT_FUNCTION(decimal);
ZEND_MSHUTDOWN_FUNCTION(decimal);
ZEND_RINIT_FUNCTION(decimal);
ZEND_RSHUTDOWN_FUNCTION(decimal);
ZEND_MINFO_FUNCTION(decimal);

ZEND_BEGIN_MODULE_GLOBALS(decimal)
mpd_context_t ctx;
mpd_t *pi;
ZEND_END_MODULE_GLOBALS(decimal)

#ifdef ZTS
    #define DECIMAL_G(v) TSRMG(decimal_globals_id, zend_decimal_globals *, v)
#else
    #define DECIMAL_G(v) (decimal_globals.v)
#endif

ZEND_EXTERN_MODULE_GLOBALS(decimal)

#if defined(ZTS) && defined(COMPILE_DL_DS)
    ZEND_TSRMLS_CACHE_EXTERN();
#endif

#endif
