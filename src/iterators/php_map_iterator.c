#include "php.h"

#include "../common.h"
#include "../internal/php_map.h"
#include "../internal/php_htable.h"

#include "php_map_iterator.h"
#include "php_htable_iterator.h"

zend_object_iterator *map_get_iterator(zend_class_entry *ce, zval *obj, int by_ref)
{
    HTable *table = Z_MAP_P(obj)->table;
    return htable_get_assoc_iterator_ex(ce, obj, by_ref, table);
}
