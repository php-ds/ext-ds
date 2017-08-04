// #include "../iterators/php_ds_tuple_iterator.h"
// #include "../handlers/php_ds_tuple_handlers.h"
// #include "../classes/php_ds_tuple_ce.h"

// #include "php_ds_tuple.h"

// zend_object *php_ds_tuple_create_object_ex(ds_tuple_t *tuple)
// {
//     php_ds_set_t *obj = ecalloc(1, sizeof(php_ds_tuple_t));
//     zend_object_std_init(&obj->std, php_ds_tuple_ce);
//     obj->std.handlers = &php_ds_tuple_handlers;
//     obj->tuple = tuple;
//     return &obj->std;
// }

// zend_object *php_ds_tuple_create_object(zend_class_entry *ce)
// {
//     return php_ds_tuple_create_object_ex(ds_set());
// }

// zend_object *php_ds_tuple_create_clone(ds_set_t *set)
// {
//     return php_ds_tuple_create_object_ex(ds_set_clone(set));
// }
