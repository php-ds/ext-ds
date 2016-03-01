#ifndef PHP_DS_H
#define PHP_DS_H

extern zend_module_entry ds_module_entry;

#define phpext_ds_ptr &ds_module_entry

/* Replace with version number for your extension */
#define PHP_DS_VERSION "0.0.1"

#ifdef PHP_WIN32
#  define PHP_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#  define PHP_API __attribute__ ((visibility("default")))
#else
#  define PHP_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

ZEND_BEGIN_MODULE_GLOBALS(ds)
zend_fcall_info        user_compare_fci;
zend_fcall_info_cache  user_compare_fci_cache;
ZEND_END_MODULE_GLOBALS(ds)

#ifdef ZTS
#define DSG(v) TSRMG(ds_globals_id, zend_ds_globals *, v)
#else
#define DSG(v) (ds_globals.v)
#endif

ZEND_EXTERN_MODULE_GLOBALS(ds);

#if defined(ZTS) && defined(COMPILE_DL_DS)
ZEND_TSRMLS_CACHE_EXTERN();
#endif

#endif
