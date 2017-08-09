#ifndef PHP_DS_PARAMETERS_H
#define PHP_DS_PARAMETERS_H

#include "php.h"
#include "ext/standard/php_var.h"

#define PHP_DS_PARAM_PARSE_1(spec, a)        if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), spec, a) == FAILURE) return
#define PHP_DS_PARAM_PARSE_2(spec, a, b)     if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), spec, a, b) == FAILURE) return
#define PHP_DS_PARAM_PARSE_3(spec, a, b, c)  if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), spec, a, b, c) == FAILURE) return

#define PHP_DS_PARAM_PARSE_NONE if (zend_parse_parameters_none() == FAILURE) return

#define PHP_DS_PARAM_PARSE_OPTIONAL_ZVAL(v) \
zval *v = NULL; \
PHP_DS_PARAM_PARSE_1("|z", &v)

#define PHP_DS_PARAM_PARSE_OPTIONAL_ZVAL_OPTIONAL_ZVAL(v1, v2) \
zval *v1 = NULL; \
zval *v2 = NULL; \
PHP_DS_PARAM_PARSE_2("|zz", &v1, &v2)

#define PHP_DS_PARAM_PARSE_STRING() \
char  *str; \
size_t len; \
PHP_DS_PARAM_PARSE_2("s", &str, &len)

#define PHP_DS_PARAM_PARSE_CALLABLE_AND_OPTIONAL_ZVAL(v)                 \
zend_fcall_info       fci       = empty_fcall_info;         \
zend_fcall_info_cache fci_cache = empty_fcall_info_cache;   \
zval *v = NULL;                                             \
PHP_DS_PARAM_PARSE_3("f|z", &fci, &fci_cache, &v)

#define PHP_DS_PARAM_PARSE_CALLABLE()                                    \
zend_fcall_info       fci       = empty_fcall_info;         \
zend_fcall_info_cache fci_cache = empty_fcall_info_cache;   \
PHP_DS_PARAM_PARSE_2("f", &fci, &fci_cache)

#define PHP_DS_PARAM_PARSE_LONG_AND_LONG(a, b) \
zend_long a = 0; \
zend_long b = 0; \
PHP_DS_PARAM_PARSE_2("ll", &a, &b)

#define PHP_DS_PARAM_PARSE_LONG_AND_ZVAL(l, z) \
zend_long l = 0; \
zval *z = NULL; \
PHP_DS_PARAM_PARSE_2("lz", &l, &z)

#define PHP_DS_PARAM_PARSE_ZVAL_LONG(z, l) \
zval *z = NULL; \
zend_long l = 0; \
PHP_DS_PARAM_PARSE_2("zl", &z, &l)

#define PHP_DS_PARAM_PARSE_VARIADIC_ZVAL() \
zval *argv = NULL; \
int argc = 0; \
PHP_DS_PARAM_PARSE_2("*", &argv, &argc)

#define PHP_DS_PARAM_PARSE_LONG_AND_VARIADIC_ZVAL(_l) \
zval *argv = NULL; \
int argc = 0; \
zend_long _l = 0; \
PHP_DS_PARAM_PARSE_3("l*", &_l, &argv, &argc)

#define PHP_DS_PARAM_PARSE_LONG(a) \
zend_long a = 0; \
PHP_DS_PARAM_PARSE_1("l", &a)

#define PHP_DS_PARAM_PARSE_COMPARE_CALLABLE() \
DSG(user_compare_fci) = empty_fcall_info; \
DSG(user_compare_fci_cache) = empty_fcall_info_cache; \
PHP_DS_PARAM_PARSE_2("f", &DSG(user_compare_fci), &DSG(user_compare_fci_cache))

#define PHP_DS_PARAM_PARSE_ZVAL(z) \
zval *z = NULL; \
PHP_DS_PARAM_PARSE_1("z", &z)

#define PHP_DS_PARAM_PARSE_OBJ(o, ce) \
zval *o = NULL; \
PHP_DS_PARAM_PARSE_2("O", &o, ce)

#define PHP_DS_PARAM_PARSE_ZVAL_ZVAL(z1, z2) \
zval *z1 = NULL; \
zval *z2 = NULL; \
PHP_DS_PARAM_PARSE_2("zz", &z1, &z2)

#define PHP_DS_PARAM_PARSE_ZVAL_OPTIONAL_ZVAL(z1, z2) \
zval *z1 = NULL; \
zval *z2 = NULL; \
PHP_DS_PARAM_PARSE_2("z|z", &z1, &z2)

#endif
