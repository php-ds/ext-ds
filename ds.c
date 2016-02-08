

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "main/php.h"
#include "ext/standard/info.h"
#include "ext/standard/php_var.h"
#include "ds.h"

#include "src/classes/php_ce_hashable.h"
#include "src/classes/php_ce_collection.h"
#include "src/classes/php_ce_sequence.h"
#include "src/classes/php_ce_vector.h"
#include "src/classes/php_ce_deque.h"
#include "src/classes/php_ce_set.h"
#include "src/classes/php_ce_map.h"
#include "src/classes/php_ce_stack.h"
#include "src/classes/php_ce_pair.h"
#include "src/classes/php_ce_priority_queue.h"
#include "src/classes/php_ce_queue.h"
#include "src/classes/php_ce_ds.h"

ZEND_DECLARE_MODULE_GLOBALS(ds);

static inline void php_ds_init_globals(zend_ds_globals *dsg) {
	memset(dsg, 0, sizeof(zend_ds_globals));
}

PHP_MINIT_FUNCTION(ds)
{
	ZEND_INIT_MODULE_GLOBALS(ds, php_ds_init_globals, NULL);

    // Interfaces
    register_hashable();
    register_collection();
    register_sequence();

    // Classes
    register_vector();
    register_deque();
    register_stack();
    register_queue();
    register_map();
    register_set();
    register_priority_queue();
    register_pair();

    // Shortcuts
    register_ds();

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
    php_info_print_table_header(2, "ds support", "enabled");
    php_info_print_table_end();
}

zend_module_entry ds_module_entry = {
    STANDARD_MODULE_HEADER,
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
