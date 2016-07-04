#include "../../common.h"

#include "../../ds/ds_map.h"
#include "../../ds/ds_htable.h"
#include "../objects/php_ds_map.h"

#include "php_map_iterator.h"
#include "php_htable_iterator.h"

zend_object_iterator *php_ds_map_get_iterator(zend_class_entry *ce, zval *obj, int by_ref)
{
    ds_htable_t *table = Z_DS_MAP_P(obj)->table;
    return ds_htable_get_assoc_iterator_ex(ce, obj, by_ref, table);
}
