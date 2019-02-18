#ifndef HAVE_PHP_DS_H
#define HAVE_PHP_DS_H

#ifdef PHP_WIN32
#   define PHP_DS_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#   define PHP_DS_API __attribute__ ((visibility("default")))
#else
#   define PHP_DS_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#include <stdio.h>
#include <stdbool.h>
#include "php.h"

extern zend_module_entry php_ds_module_entry;

ZEND_MINIT_FUNCTION(ds);
ZEND_MSHUTDOWN_FUNCTION(ds);
ZEND_RINIT_FUNCTION(ds);
ZEND_RSHUTDOWN_FUNCTION(ds);
ZEND_MINFO_FUNCTION(ds);

ZEND_BEGIN_MODULE_GLOBALS(ds)
ZEND_END_MODULE_GLOBALS(ds)

#ifdef ZTS
    #define DS_G(v) TSRMG(ds_globals_id, zend_ds_globals *, v)
#else
    #define DS_G(v) (ds_globals.v)
#endif

ZEND_EXTERN_MODULE_GLOBALS(ds)

#if defined(ZTS) && defined(COMPILE_DL_DS)
    ZEND_TSRMLS_CACHE_EXTERN();
#endif

#endif
