#include "php.h"

#include "../common.h"
#include "../internal/php_set.h"
#include "../internal/php_htable.h"

#include "php_set_iterator.h"
#include "php_htable_iterator.h"

zend_object_iterator *set_get_iterator(zend_class_entry *ce, zval *obj, int by_ref)
{
    HTable *table = (Z_SET_P(obj))->table;
    return htable_get_key_iterator_ex(ce, obj, by_ref, table);
}
