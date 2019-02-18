PHP_ARG_ENABLE(ds, whether to enable ds support,
[  --enable-ds           Enable ds support])

if test "$PHP_DS" != "no"; then
  PHP_NEW_EXTENSION(ds, php_ds.c src/vector.c src/buffer.c, $ext_shared, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
  PHP_ADD_BUILD_DIR($ext_builddir/src, 1)
fi
