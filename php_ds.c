#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "main/php.h"
#include "ext/standard/info.h"
#include "ext/standard/php_var.h"
#include "php_ds.h"

#include "src/php/classes/php_hashable_ce.h"
#include "src/php/classes/php_collection_ce.h"
#include "src/php/classes/php_sequence_ce.h"
#include "src/php/classes/php_vector_ce.h"
#include "src/php/classes/php_deque_ce.h"
#include "src/php/classes/php_set_ce.h"
#include "src/php/classes/php_map_ce.h"
#include "src/php/classes/php_stack_ce.h"
#include "src/php/classes/php_pair_ce.h"
#include "src/php/classes/php_priority_queue_ce.h"
#include "src/php/classes/php_queue_ce.h"

ZEND_DECLARE_MODULE_GLOBALS(ds);

static inline void php_ds_init_globals(zend_ds_globals *dsg) {
	memset(dsg, 0, sizeof(zend_ds_globals));
}

PHP_MINIT_FUNCTION(ds)
{
	ZEND_INIT_MODULE_GLOBALS(ds, php_ds_init_globals, NULL);

    // Interfaces
    php_ds_register_hashable();
    php_ds_register_collection();
    php_ds_register_sequence();

    // Classes
    php_ds_register_vector();
    php_ds_register_deque();
    php_ds_register_stack();
    php_ds_register_queue();
    php_ds_register_map();
    php_ds_register_set();
    php_ds_register_priority_queue();
    php_ds_register_pair();

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
    NULL,
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
