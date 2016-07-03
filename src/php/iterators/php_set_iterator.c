#include "../../common.h"

#include "../../ds/ds_set.h"
#include "../../ds/ds_htable.h"

#include "php_set_iterator.h"
#include "php_htable_iterator.h"

zend_object_iterator *set_get_iterator(zend_class_entry *ce, zval *obj, int by_ref)
{
    ds_htable_t *table = (Z_SET_P(obj))->table;
    return ds_htable_get_key_iterator_ex(ce, obj, by_ref, table);
}
