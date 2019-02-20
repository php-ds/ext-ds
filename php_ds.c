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

#include "php_ds.h"
#include "src/vector.h"
#include "src/buffer.h"

ZEND_DECLARE_MODULE_GLOBALS(ds)

static void php_ds_init_globals(zend_ds_globals *g)
{
    memset(g, 0, sizeof(zend_ds_globals));
}

/**
 * Module entry
 */
zend_module_entry ds_module_entry = {
    STANDARD_MODULE_HEADER,
    "ds",
    NULL,
    PHP_MINIT(ds),
    PHP_MSHUTDOWN(ds),
    PHP_RINIT(ds),
    PHP_RSHUTDOWN(ds),
    PHP_MINFO(ds),
    "0.0.0",
    STANDARD_MODULE_PROPERTIES};

/**
 * Module information displayed by phpinfo()
 */
PHP_MINFO_FUNCTION(ds)
{
}

/**
 * Module startup
 */
PHP_MINIT_FUNCTION(ds)
{
    ds_register_vector();
    ds_register_buffer();

    ZEND_INIT_MODULE_GLOBALS(ds, php_ds_init_globals, NULL);
    return SUCCESS;
}

/**
 * Module shutdown
 */
PHP_MSHUTDOWN_FUNCTION(ds)
{
    return SUCCESS;
}

/**
 * Request startup
 */
PHP_RINIT_FUNCTION(ds)
{
#if defined(COMPILE_DL_DS) && defined(ZTS)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif

    return SUCCESS;
}

/**
 * Request shutdown
 */
PHP_RSHUTDOWN_FUNCTION(ds)
{
    return SUCCESS;
}

#ifdef COMPILE_DL_DS
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE();
#endif
ZEND_GET_MODULE(ds)
#endif
