PHP_ARG_ENABLE(ds, whether to enable ds support,
[  --enable-ds           Enable ds support])

if test "$PHP_COLLECTIONS" != "no"; then
  PHP_NEW_EXTENSION(ds,                       \
                                              \
  src/common.c                                \
                                              \
dnl Internal
  src/ds/ds_vector.c                   \
  src/ds/ds_deque.c                    \
  src/ds/ds_htable.c                   \
  src/ds/ds_set.c                      \
  src/ds/ds_map.c                      \
  src/ds/ds_stack.c                    \
  src/ds/ds_pair.c                     \
  src/ds/ds_priority_queue.c           \
  src/ds/ds_queue.c                    \
                                              \
  src/php/objects/php_vector.c             \
  src/php/objects/php_deque.c              \
  src/php/objects/php_map.c                \
  src/php/objects/php_pair.c               \
  src/php/objects/php_priority_queue.c     \
                                              \
dnl Iterators
  src/php/iterators/php_vector_iterator.c         \
  src/php/iterators/php_deque_iterator.c          \
  src/php/iterators/php_set_iterator.c            \
  src/php/iterators/php_map_iterator.c            \
  src/php/iterators/php_stack_iterator.c          \
  src/php/iterators/php_htable_iterator.c         \
  src/php/iterators/php_priority_queue_iterator.c \
  src/php/iterators/php_queue_iterator.c          \
                                                  \
dnl Handlers
  src/php/handlers/php_common_handlers.c          \
  src/php/handlers/php_vector_handlers.c          \
  src/php/handlers/php_deque_handlers.c           \
  src/php/handlers/php_set_handlers.c             \
  src/php/handlers/php_map_handlers.c             \
  src/php/handlers/php_stack_handlers.c           \
  src/php/handlers/php_pair_handlers.c            \
  src/php/handlers/php_priority_queue_handlers.c  \
  src/php/handlers/php_queue_handlers.c           \
                                              \
dnl Interfaces
  src/php/classes/php_ce_hashable.c               \
  src/php/classes/php_ce_collection.c             \
  src/php/classes/php_ce_sequence.c               \
                                              \
dnl Classes
  src/php/classes/php_ce_vector.c                 \
  src/php/classes/php_ce_deque.c                  \
  src/php/classes/php_ce_set.c                    \
  src/php/classes/php_ce_map.c                    \
  src/php/classes/php_ce_stack.c                  \
  src/php/classes/php_ce_pair.c                   \
  src/php/classes/php_ce_priority_queue.c         \
  src/php/classes/php_ce_queue.c                  \
  src/php/classes/php_ce_ds.c                     \
                                              \
  php_ds.c                                    \
                                              \
  , $ext_shared, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi


