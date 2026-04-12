PHP_ARG_ENABLE(ds, whether to enable ds support,
[  --enable-ds           Enable ds support])

if test "$PHP_DS" != "no"; then
  PHP_NEW_EXTENSION(ds,                       \
                                              \
  src/common.c                                \
                                              \
dnl Internal
  src/ds/ds_seq.c                      \
  src/ds/ds_htable.c                   \
  src/ds/ds_set.c                      \
  src/ds/ds_map.c                      \
  src/ds/ds_heap.c                     \
                                                  \
  src/php/objects/php_seq.c                       \
  src/php/objects/php_map.c                       \
  src/php/objects/php_pair.c                      \
  src/php/objects/php_heap.c                      \
  src/php/objects/php_set.c                       \
                                                  \
dnl Iterators
  src/php/iterators/php_seq_iterator.c            \
  src/php/iterators/php_set_iterator.c            \
  src/php/iterators/php_map_iterator.c            \
  src/php/iterators/php_htable_iterator.c         \
  src/php/iterators/php_heap_iterator.c            \
                                                  \
dnl Handlers
  src/php/handlers/php_common_handlers.c          \
  src/php/handlers/php_seq_handlers.c             \
  src/php/handlers/php_set_handlers.c             \
  src/php/handlers/php_map_handlers.c             \
  src/php/handlers/php_pair_handlers.c            \
  src/php/handlers/php_heap_handlers.c             \
                                                  \
dnl Classes
  src/php/classes/php_key_ce.c                    \
  src/php/classes/php_seq_ce.c                    \
  src/php/classes/php_set_ce.c                    \
  src/php/classes/php_map_ce.c                    \
  src/php/classes/php_pair_ce.c                   \
  src/php/classes/php_heap_ce.c                    \
                                                  \
  php_ds.c                                        \
                                                  \
  , $ext_shared, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)

  PHP_ADD_BUILD_DIR($ext_builddir/src, 1)
  PHP_ADD_BUILD_DIR($ext_builddir/src/ds, 1)
  PHP_ADD_BUILD_DIR($ext_builddir/src/php, 1)
  PHP_ADD_BUILD_DIR($ext_builddir/src/php/objects, 1)
  PHP_ADD_BUILD_DIR($ext_builddir/src/php/classes, 1)
  PHP_ADD_BUILD_DIR($ext_builddir/src/php/iterators, 1)
  PHP_ADD_BUILD_DIR($ext_builddir/src/php/handlers, 1)

  PHP_ADD_EXTENSION_DEP(ds, spl)
fi
