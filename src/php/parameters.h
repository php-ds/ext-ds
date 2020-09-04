#ifndef DS_PARAMETERS_H
#define DS_PARAMETERS_H

#define SETUP_CALLABLE_VARS() \
zend_fcall_info fci = empty_fcall_info; \
zend_fcall_info_cache fci_cache = empty_fcall_info_cache;

#define PARSE_1(spec, a)        if (zend_parse_parameters(ZEND_NUM_ARGS(), spec, a) == FAILURE) return
#define PARSE_2(spec, a, b)     if (zend_parse_parameters(ZEND_NUM_ARGS(), spec, a, b) == FAILURE) return
#define PARSE_3(spec, a, b, c)  if (zend_parse_parameters(ZEND_NUM_ARGS(), spec, a, b, c) == FAILURE) return

#define PARSE_NONE if (zend_parse_parameters_none() == FAILURE) return

#define PARSE_OPTIONAL_ZVAL(v) \
zval *v = NULL; \
PARSE_1("|z", &v)

#define PARSE_OPTIONAL_ZVAL_OPTIONAL_ZVAL(v1, v2) \
zval *v1 = NULL; \
zval *v2 = NULL; \
PARSE_2("|zz", &v1, &v2)

#define PARSE_STRING() \
char  *str; \
size_t len; \
PARSE_2("s", &str, &len)

#define PARSE_CALLABLE_AND_OPTIONAL_ZVAL(v) \
SETUP_CALLABLE_VARS(); \
zval *v = NULL; \
PARSE_3("f|z", &fci, &fci_cache, &v)

#define PARSE_CALLABLE() \
SETUP_CALLABLE_VARS(); \
PARSE_2("f", &fci, &fci_cache)

#define PARSE_LONG_AND_LONG(a, b) \
zend_long a = 0; \
zend_long b = 0; \
PARSE_2("ll", &a, &b)

#define PARSE_LONG_AND_ZVAL(l, z) \
zend_long l = 0; \
zval *z = NULL; \
PARSE_2("lz", &l, &z)

#define PARSE_ZVAL_AND_LONG(z, l) \
zval *z = NULL; \
zend_long l = 0; \
PARSE_2("zl", &z, &l)

#define PARSE_VARIADIC_ZVAL() \
zval *argv = NULL; \
int argc = 0; \
PARSE_2("*", &argv, &argc)

#define PARSE_LONG_AND_VARIADIC_ZVAL(_l) \
zval *argv = NULL; \
int argc = 0; \
zend_long _l = 0; \
PARSE_3("l*", &_l, &argv, &argc)

#define PARSE_LONG(a) \
zend_long a = 0; \
PARSE_1("l", &a)

#define PARSE_COMPARE_CALLABLE() \
DSG(user_compare_fci) = empty_fcall_info; \
DSG(user_compare_fci_cache) = empty_fcall_info_cache; \
PARSE_2("f", &DSG(user_compare_fci), &DSG(user_compare_fci_cache))

#define PARSE_ZVAL(z) \
zval *z = NULL; \
PARSE_1("z", &z)

#define PARSE_OBJ(o, ce) \
zval *o = NULL; \
PARSE_2("O", &o, ce)

#define PARSE_ZVAL_ZVAL(z1, z2) \
zval *z1 = NULL; \
zval *z2 = NULL; \
PARSE_2("zz", &z1, &z2)

#define PARSE_ZVAL_OPTIONAL_ZVAL(z1, z2) \
zval *z1 = NULL; \
zval *z2 = NULL; \
PARSE_2("z|z", &z1, &z2)

#endif
