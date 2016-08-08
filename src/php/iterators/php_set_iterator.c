#include "../../common.h"

#include "../../ds/ds_set.h"
#include "../../ds/ds_htable.h"
#include "../objects/php_set.h"

#include "php_set_iterator.h"
#include "php_htable_iterator.h"

zend_object_iterator *php_ds_set_get_iterator(zend_class_entry *ce, zval *obj, int by_ref)
{
    ds_htable_t *table = (Z_DS_SET_P(obj))->table;
    return php_ds_htable_get_key_iterator_ex(ce, obj, by_ref, table);
}
