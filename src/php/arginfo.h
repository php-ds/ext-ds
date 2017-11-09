#ifndef DS_ARGINFO_H
#define DS_ARGINFO_H

#define ARGINFO_NONE(name) \
ZEND_BEGIN_ARG_INFO_EX(arginfo_##name, 0, 0, 0) \
ZEND_END_ARG_INFO()

#define ARGINFO_OPTIONAL_ZVAL(name, z) \
ZEND_BEGIN_ARG_INFO_EX(arginfo_##name, 0, 0, 0) \
ZEND_ARG_TYPE_INFO(0, z, 0, 1) \
ZEND_END_ARG_INFO()

#define ARGINFO_OPTIONAL_ZVAL_OPTIONAL_ZVAL(name, z1, z2) \
ZEND_BEGIN_ARG_INFO_EX(arginfo_##name, 0, 0, 0) \
ZEND_ARG_TYPE_INFO(0, z1, 0, 1) \
ZEND_ARG_TYPE_INFO(0, z2, 0, 1) \
ZEND_END_ARG_INFO()

#define ARGINFO_CALLABLE_OPTIONAL_ZVAL(name, c, z) \
ZEND_BEGIN_ARG_INFO_EX(arginfo_##name, 0, 0, 1) \
ZEND_ARG_TYPE_INFO(0, c, IS_CALLABLE, 0) \
ZEND_ARG_TYPE_INFO(0, z, 0, 1) \
ZEND_END_ARG_INFO()

#define ARGINFO_OPTIONAL_STRING(name, s) \
ZEND_BEGIN_ARG_INFO_EX(arginfo_##name, 0, 0, 0) \
ZEND_ARG_TYPE_INFO(0, s, IS_STRING, 0) \
ZEND_END_ARG_INFO()

#define ARGINFO_OPTIONAL_CALLABLE(name, c) \
ZEND_BEGIN_ARG_INFO_EX(arginfo_##name, 0, 0, 0) \
ZEND_ARG_TYPE_INFO(0, c, IS_CALLABLE, 1) \
ZEND_END_ARG_INFO()

#define ARGINFO_CALLABLE(name, c) \
ZEND_BEGIN_ARG_INFO_EX(arginfo_##name, 0, 0, 1) \
ZEND_ARG_TYPE_INFO(0, c, IS_CALLABLE, 0) \
ZEND_END_ARG_INFO()

#define ARGINFO_VARIADIC_ZVAL(name, v) \
ZEND_BEGIN_ARG_INFO_EX(arginfo_##name, 0, 0, 0) \
ZEND_ARG_VARIADIC_INFO(0, v) \
ZEND_END_ARG_INFO()

#define ARGINFO_LONG(name, i) \
ZEND_BEGIN_ARG_INFO_EX(arginfo_##name, 0, 0, 1) \
ZEND_ARG_TYPE_INFO(0, i, IS_LONG, 0) \
ZEND_END_ARG_INFO()

#define ARGINFO_LONG_ZVAL(name, i, z) \
ZEND_BEGIN_ARG_INFO_EX(arginfo_##name, 0, 0, 2) \
ZEND_ARG_TYPE_INFO(0, i, IS_LONG, 0) \
ZEND_ARG_INFO(0, z) \
ZEND_END_ARG_INFO()

#define ARGINFO_ZVAL_LONG(name, z, i) \
ZEND_BEGIN_ARG_INFO_EX(arginfo_##name, 0, 0, 2) \
ZEND_ARG_INFO(0, z) \
ZEND_ARG_TYPE_INFO(0, i, IS_LONG, 0) \
ZEND_END_ARG_INFO()

#define ARGINFO_ZVAL(name, z) \
ZEND_BEGIN_ARG_INFO_EX(arginfo_##name, 0, 0, 1) \
ZEND_ARG_INFO(0, z) \
ZEND_END_ARG_INFO()

#define ARGINFO_ZVAL_ZVAL(name, z1, z2) \
ZEND_BEGIN_ARG_INFO_EX(arginfo_##name, 0, 0, 2) \
ZEND_ARG_INFO(0, z1) \
ZEND_ARG_INFO(0, z2) \
ZEND_END_ARG_INFO()

#define ARGINFO_ZVAL_OPTIONAL_ZVAL(name, z1, z2) \
ZEND_BEGIN_ARG_INFO_EX(arginfo_##name, 0, 0, 1) \
ZEND_ARG_INFO(0, z1) \
ZEND_ARG_TYPE_INFO(0, z2, 0, 1) \
ZEND_END_ARG_INFO()

#define ARGINFO_LONG_VARIADIC_ZVAL(name, i, v) \
ZEND_BEGIN_ARG_INFO_EX(arginfo_##name, 0, 0, 1) \
ZEND_ARG_TYPE_INFO(0, i, IS_LONG, 0) \
ZEND_ARG_VARIADIC_INFO(0, v) \
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
#define DS_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(name, return_reference, required_num_args, class_name, allow_null) \
	static const zend_internal_arg_info arginfo_##name[] = { \
		{ (const char*)(zend_uintptr_t)(required_num_args), ZEND_TYPE_ENCODE_CLASS_CONST(PHP_DS_NS_NAME#class_name, allow_null), return_reference, 0 },
#else
#define DS_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(name, return_reference, required_num_args, class_name, allow_null) \
	static const zend_internal_arg_info arginfo_##name[] = { \
		{ (const char*)(zend_uintptr_t)(required_num_args), PHP_DS_NS_NAME#class_name, IS_OBJECT, return_reference, allow_null, 0 },
#endif

#if PHP_VERSION_ID >= 70200
#define DS_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(name, return_reference, required_num_args, type, allow_null) \
	static const zend_internal_arg_info arginfo_##name[] = { \
		{ (const char*)(zend_uintptr_t)(required_num_args), ZEND_TYPE_ENCODE(type, allow_null), return_reference, 0 },
#else
#define DS_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(name, return_reference, required_num_args, type, allow_null) \
	static const zend_internal_arg_info arginfo_##name[] = { \
		{ (const char*)(zend_uintptr_t)(required_num_args), NULL, type, return_reference, allow_null, 0 },
#endif

#define ARGINFO_ZVAL_RETURN_BOOL(name, z) \
    DS_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(name, 0, 1, _IS_BOOL, 0) \
    ZEND_ARG_INFO(0, z) \
    ZEND_END_ARG_INFO()

#define ARGINFO_CALLABLE_RETURN_DS(name, c, col) \
    DS_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(name, 0, 1, col, 1) \
    ZEND_ARG_TYPE_INFO(0, c, IS_CALLABLE, 0) \
    ZEND_END_ARG_INFO()

#define ARGINFO_OPTIONAL_ZVAL_RETURN_DS(name, z, col) \
    DS_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(name, 0, 0, col, 1) \
    ZEND_ARG_TYPE_INFO(0, z, 0, 1) \
    ZEND_END_ARG_INFO()

#define ARGINFO_OPTIONAL_STRING_RETURN_STRING(name, s) \
    DS_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(name, 0, 0, IS_STRING, 0) \
    ZEND_ARG_TYPE_INFO(0, s, IS_STRING, 0) \
    ZEND_END_ARG_INFO()

#define ARGINFO_ZVAL_RETURN_DS(name, z, col) \
    DS_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(name, 0, 1, col, 1) \
    ZEND_ARG_TYPE_INFO(0, z, 0, 0) \
    ZEND_END_ARG_INFO()

#define ARGINFO_OPTIONAL_CALLABLE_RETURN_DS(name, c, col) \
    DS_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(name, 0, 0, col, 1) \
    ZEND_ARG_TYPE_INFO(0, c, IS_CALLABLE, 1) \
    ZEND_END_ARG_INFO()

#define ARGINFO_LONG_OPTIONAL_LONG_RETURN_DS(name, i1, i2, col) \
    DS_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(name, 0, 1, col, 1) \
    ZEND_ARG_TYPE_INFO(0, i1, IS_LONG, 0) \
    ZEND_ARG_TYPE_INFO(0, i2, IS_LONG, 1) \
    ZEND_END_ARG_INFO()

#define ARGINFO_LONG_RETURN_DS(name, i, col) \
    DS_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(name, 0, 1, col, 1) \
    ZEND_ARG_TYPE_INFO(0, i, IS_LONG, 0) \
    ZEND_END_ARG_INFO()

#define ARGINFO_NONE_RETURN_LONG(name) \
    DS_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(name, 0, 0, IS_LONG, 0) \
    ZEND_END_ARG_INFO()

#define ARGINFO_NONE_RETURN_STRING(name) \
    DS_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(name, 0, 0, IS_STRING, 0) \
    ZEND_END_ARG_INFO()

#define ARGINFO_NONE_RETURN_DS(name, class_name) \
    DS_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(name, 0, 0, class_name, 1) \
    ZEND_END_ARG_INFO()

#define ARGINFO_NONE_RETURN_OBJ(name, class_name) \
    DS_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(name, 0, 0, class_name, 1) \
    ZEND_END_ARG_INFO()

#define ARGINFO_NONE_RETURN_BOOL(name) \
    DS_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(name, 0, 0, _IS_BOOL, 0) \
    ZEND_END_ARG_INFO()

#define ARGINFO_VARIADIC_ZVAL_RETURN_BOOL(name, v) \
    DS_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(name, 0, 0, _IS_BOOL, 0) \
    ZEND_ARG_VARIADIC_INFO(0, v) \
    ZEND_END_ARG_INFO()

#define ARGINFO_NONE_RETURN_ARRAY(name) \
    DS_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(name, 0, 0, IS_ARRAY, 0) \
    ZEND_END_ARG_INFO()

#define ARGINFO_DS_RETURN_DS(name, obj, cls, col) \
    DS_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(name, 0, 1, col, 1) \
    ZEND_ARG_OBJ_INFO(0, obj, Ds\\cls, 0) \
    ZEND_END_ARG_INFO()

#endif
