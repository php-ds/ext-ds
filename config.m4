dnl $Id$
dnl config.m4 for extension ds

PHP_ARG_ENABLE(ds, whether to enable ds support,
[  --enable-ds           Enable ds support])

if test "$PHP_COLLECTIONS" != "no"; then
  PHP_NEW_EXTENSION(ds,                       \
                                              \
  src/common.c                                \
                                              \
dnl Internal
  src/internal/php_deque.c                    \
  src/internal/php_vector.c                   \
  src/internal/php_htable.c                   \
  src/internal/php_set.c                      \
  src/internal/php_map.c                      \
  src/internal/php_stack.c                    \
  src/internal/php_pair.c                     \
  src/internal/php_priority_queue.c           \
  src/internal/php_queue.c                    \
                                              \
dnl Iterators
  src/iterators/php_vector_iterator.c         \
  src/iterators/php_deque_iterator.c          \
  src/iterators/php_set_iterator.c            \
  src/iterators/php_map_iterator.c            \
  src/iterators/php_stack_iterator.c          \
  src/iterators/php_htable_iterator.c         \
  src/iterators/php_priority_queue_iterator.c \
  src/iterators/php_queue_iterator.c          \
                                              \
dnl Handlers
  src/handlers/php_vector_handlers.c          \
  src/handlers/php_deque_handlers.c           \
  src/handlers/php_set_handlers.c             \
  src/handlers/php_map_handlers.c             \
  src/handlers/php_stack_handlers.c           \
  src/handlers/php_pair_handlers.c            \
  src/handlers/php_priority_queue_handlers.c  \
  src/handlers/php_queue_handlers.c           \
                                              \
dnl Interfaces
  src/classes/php_ce_hashable.c               \
  src/classes/php_ce_collection.c             \
  src/classes/php_ce_sequence.c               \
                                              \
dnl Classes
  src/classes/php_ce_vector.c                 \
  src/classes/php_ce_deque.c                  \
  src/classes/php_ce_set.c                    \
  src/classes/php_ce_map.c                    \
  src/classes/php_ce_stack.c                  \
  src/classes/php_ce_pair.c                   \
  src/classes/php_ce_priority_queue.c         \
  src/classes/php_ce_queue.c                  \
  src/classes/php_ce_ds.c                     \
                                              \
  ds.c                                        \
                                              \
  , $ext_shared, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi


