#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "main/php.h"
#include "ext/standard/info.h"
#include "ext/standard/php_var.h"
#include "php_ds.h"

#include "src/common.h"
#include "src/php/classes/php_key_ce.h"
#include "src/php/classes/php_seq_ce.h"
#include "src/php/classes/php_set_ce.h"
#include "src/php/classes/php_map_ce.h"
#include "src/php/classes/php_pair_ce.h"
#include "src/php/classes/php_heap_ce.h"

#include "src/php/objects/php_seq.h"
#include "src/php/objects/php_map.h"
#include "src/php/objects/php_set.h"
#include "src/php/objects/php_heap.h"

ZEND_DECLARE_MODULE_GLOBALS(ds);

static inline void php_ds_init_globals(zend_ds_globals *dsg) {
	memset(dsg, 0, sizeof(zend_ds_globals));
}

/* {{{ arginfo for namespace functions */
ZEND_BEGIN_ARG_INFO_EX(arginfo_ds_seq, 0, 0, 0)
    ZEND_ARG_INFO(0, values)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_ds_map, 0, 0, 0)
    ZEND_ARG_INFO(0, values)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_ds_set, 0, 0, 0)
    ZEND_ARG_INFO(0, values)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_ds_heap, 0, 0, 0)
    ZEND_ARG_INFO(0, values)
    ZEND_ARG_TYPE_INFO(0, comparator, IS_CALLABLE, 1)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ \Ds\seq(iterable $values = []): Ds\Seq */
PHP_FUNCTION(seq)
{
    zval *values = NULL;

    ZEND_PARSE_PARAMETERS_START(0, 1)
        Z_PARAM_OPTIONAL
        Z_PARAM_ZVAL(values)
    ZEND_PARSE_PARAMETERS_END();

    ds_seq_t *seq = ds_seq();

    if (values && Z_TYPE_P(values) != IS_NULL) {
        ds_seq_push_all(seq, values);
    }

    ZVAL_DS_SEQ(return_value, seq);
}
/* }}} */

/* {{{ \Ds\map(iterable $values = []): Ds\Map */
PHP_FUNCTION(map)
{
    zval *values = NULL;

    ZEND_PARSE_PARAMETERS_START(0, 1)
        Z_PARAM_OPTIONAL
        Z_PARAM_ZVAL(values)
    ZEND_PARSE_PARAMETERS_END();

    ds_map_t *map = ds_map();

    if (values && Z_TYPE_P(values) != IS_NULL) {
        ds_map_put_all(map, values);
    }

    ZVAL_DS_MAP(return_value, map);
}
/* }}} */

/* {{{ \Ds\set(iterable $values = []): Ds\Set */
PHP_FUNCTION(set)
{
    zval *values = NULL;

    ZEND_PARSE_PARAMETERS_START(0, 1)
        Z_PARAM_OPTIONAL
        Z_PARAM_ZVAL(values)
    ZEND_PARSE_PARAMETERS_END();

    ds_set_t *set = ds_set();

    if (values && Z_TYPE_P(values) != IS_NULL) {
        ds_set_add_all(set, values);
    }

    ZVAL_DS_SET(return_value, set);
}
/* }}} */

/* {{{ \Ds\heap(iterable $values = [], ?callable $comparator = null): Ds\Heap */
PHP_FUNCTION(heap)
{
    zval *values = NULL;
    zend_fcall_info fci = empty_fcall_info;
    zend_fcall_info_cache fci_cache = empty_fcall_info_cache;

    ZEND_PARSE_PARAMETERS_START(0, 2)
        Z_PARAM_OPTIONAL
        Z_PARAM_ZVAL(values)
        Z_PARAM_FUNC_OR_NULL(fci, fci_cache)
    ZEND_PARSE_PARAMETERS_END();

    ds_heap_t *heap = ds_heap();
    zend_object *obj_zend = php_ds_heap_create_object_ex(heap);
    php_ds_heap_t *obj = php_ds_heap_fetch_object(obj_zend);

    if (ZEND_FCI_INITIALIZED(fci)) {
        obj->has_comparator = true;
        obj->fci            = fci;
        obj->fci_cache      = fci_cache;
        Z_TRY_ADDREF(obj->fci.function_name);
    }

    if (values && Z_TYPE_P(values) != IS_NULL) {
        ds_heap_compare_func_t compare = php_ds_heap_get_compare(obj);
        ds_heap_push_all(heap, values, compare);
    }

    ZVAL_OBJ(return_value, obj_zend);
}
/* }}} */

static const zend_function_entry ds_functions[] = {
    ZEND_NS_FE("Ds", seq,  arginfo_ds_seq)
    ZEND_NS_FE("Ds", map,  arginfo_ds_map)
    ZEND_NS_FE("Ds", set,  arginfo_ds_set)
    ZEND_NS_FE("Ds", heap, arginfo_ds_heap)
    PHP_FE_END
};

PHP_MINIT_FUNCTION(ds)
{
	ZEND_INIT_MODULE_GLOBALS(ds, php_ds_init_globals, NULL);

    // Interfaces
    php_ds_register_key();

    // Classes
    php_ds_register_pair();
    php_ds_register_seq();
    php_ds_register_map();
    php_ds_register_set();
    php_ds_register_heap();

    return SUCCESS;
}

PHP_RINIT_FUNCTION(ds)
{
#if defined(COMPILE_DL_DS) && defined(ZTS)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif

    return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(ds)
{
    return SUCCESS;
}

PHP_MINFO_FUNCTION(ds)
{
    php_info_print_table_start();
    php_info_print_table_row(2, "ds support", "enabled");
    php_info_print_table_row(2, "ds version", PHP_DS_VERSION);
    php_info_print_table_end();
}

static const zend_module_dep ds_deps[] = {
    ZEND_MOD_REQUIRED("json")
    ZEND_MOD_REQUIRED("spl")
    ZEND_MOD_END
};

zend_module_entry ds_module_entry = {
    STANDARD_MODULE_HEADER_EX,
    NULL,
    ds_deps,
    "ds",
    ds_functions,
    PHP_MINIT(ds),
    NULL,
    PHP_RINIT(ds),
    PHP_RSHUTDOWN(ds),
    PHP_MINFO(ds),
    PHP_DS_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_DS
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE();
#endif
ZEND_GET_MODULE(ds)
#endif
