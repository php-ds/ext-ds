#include "../../common.h"

#include "../parameters.h"
#include "../arginfo.h"

#include "../objects/php_heap.h"
#include "../iterators/php_heap_iterator.h"
#include "../handlers/php_heap_handlers.h"

#include "php_ds_ce_common.h"
#include "php_heap_ce.h"

#define METHOD(name) PHP_METHOD(Heap, name)

#define SEPARATE() ds_heap_separate(&(THIS_DS_HEAP()->heap))

zend_class_entry *php_ds_heap_ce;

METHOD(__construct)
{
    php_ds_heap_t *obj = THIS_DS_HEAP();

    zval *values = NULL;
    zend_fcall_info fci = empty_fcall_info;
    zend_fcall_info_cache fci_cache = empty_fcall_info_cache;

    ZEND_PARSE_PARAMETERS_START(0, 2)
        Z_PARAM_OPTIONAL
        Z_PARAM_ZVAL(values)
        Z_PARAM_FUNC_OR_NULL(fci, fci_cache)
    ZEND_PARSE_PARAMETERS_END();

    if (ZEND_FCI_INITIALIZED(fci)) {
        obj->has_comparator = true;
        obj->fci            = fci;
        obj->fci_cache      = fci_cache;
        Z_TRY_ADDREF(obj->fci.function_name);
    }

    if (values && Z_TYPE_P(values) != IS_NULL) {
        SEPARATE();
        ds_heap_compare_func_t compare = php_ds_heap_get_compare(obj);
        ds_heap_push_all(obj->heap, values, compare);
    }
}

METHOD(allocate)
{
    PARSE_LONG(capacity);
    if (capacity < 0) {
        CAPACITY_INVALID(capacity);
        return;
    }
    SEPARATE();
    ds_heap_allocate(THIS_DS_HEAP()->heap, capacity);
}

METHOD(capacity)
{
    PARSE_NONE;
    RETURN_LONG(ds_heap_capacity(THIS_DS_HEAP()->heap));
}

METHOD(push)
{
    php_ds_heap_t *obj = THIS_DS_HEAP();
    ds_heap_compare_func_t compare = php_ds_heap_get_compare(obj);

    PARSE_VARIADIC_ZVAL();
    SEPARATE();

    for (int i = 0; i < argc; i++) {
        ds_heap_push(obj->heap, &argv[i], compare);
    }
}

METHOD(pop)
{
    php_ds_heap_t *obj = THIS_DS_HEAP();
    PARSE_NONE;
    SEPARATE();
    ds_heap_pop(obj->heap, return_value, php_ds_heap_get_compare(obj));
}

METHOD(peek)
{
    PARSE_NONE;
    RETURN_ZVAL_COPY(ds_heap_peek(THIS_DS_HEAP()->heap));
}

METHOD(count)
{
    PARSE_NONE;
    RETURN_LONG(DS_HEAP_SIZE(THIS_DS_HEAP()->heap));
}

METHOD(clear)
{
    PARSE_NONE;
    SEPARATE();
    ds_heap_clear(THIS_DS_HEAP()->heap);
}

METHOD(isEmpty)
{
    PARSE_NONE;
    RETURN_BOOL(DS_HEAP_IS_EMPTY(THIS_DS_HEAP()->heap));
}

METHOD(copy)
{
    PARSE_NONE;
    RETURN_OBJ(php_ds_heap_create_clone(THIS_DS_HEAP()));
}

METHOD(toArray)
{
    php_ds_heap_t *obj = THIS_DS_HEAP();
    PARSE_NONE;
    ds_heap_to_array(obj->heap, return_value, php_ds_heap_get_compare(obj));
}

METHOD(jsonSerialize)
{
    php_ds_heap_t *obj = THIS_DS_HEAP();
    PARSE_NONE;
    ds_heap_to_array(obj->heap, return_value, php_ds_heap_get_compare(obj));
}

METHOD(__serialize)
{
    php_ds_heap_t *obj = THIS_DS_HEAP();
    ds_heap_t *heap = obj->heap;
    PARSE_NONE;

    array_init_size(return_value, heap->size);

    // Serialize in heap-popped order for stable output.
    ds_heap_t *clone = ds_heap_clone(heap);
    ds_heap_compare_func_t compare = php_ds_heap_get_compare(obj);

    while (clone->size > 0) {
        zval value;
        ds_heap_pop(clone, &value, compare);
        add_next_index_zval(return_value, &value);
    }

    ds_heap_free(clone);
}

METHOD(__unserialize)
{
    PARSE_ZVAL(data);
    SEPARATE();
    php_ds_heap_t *obj = THIS_DS_HEAP();
    ds_heap_compare_func_t compare = php_ds_heap_get_compare(obj);

    zval *value;
    ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(data), value) {
        ds_heap_push(obj->heap, value, compare);
    } ZEND_HASH_FOREACH_END();
}

METHOD(getIterator) {
    PARSE_NONE;
    ZVAL_COPY(return_value, getThis());
}

void php_ds_register_heap()
{
    zend_class_entry ce;

    zend_function_entry methods[] = {
        PHP_DS_ME(Heap, __construct)
        PHP_DS_ME(Heap, allocate)
        PHP_DS_ME(Heap, capacity)
        PHP_DS_ME(Heap, push)
        PHP_DS_ME(Heap, pop)
        PHP_DS_ME(Heap, peek)
        PHP_DS_ME(Heap, getIterator)

        PHP_DS_COLLECTION_ME_LIST(Heap)
        PHP_FE_END
    };

    INIT_CLASS_ENTRY(ce, PHP_DS_NS(Heap), methods);

    php_ds_heap_ce = zend_register_internal_class(&ce);
    php_ds_heap_ce->ce_flags      |= ZEND_ACC_FINAL;
    php_ds_heap_ce->create_object  = php_ds_heap_create_object;
    php_ds_heap_ce->get_iterator   = php_ds_heap_get_iterator;
    php_ds_heap_ce->serialize      = php_ds_heap_serialize;
    php_ds_heap_ce->unserialize    = php_ds_heap_unserialize;

    zend_declare_class_constant_long(
        php_ds_heap_ce,
        STR_AND_LEN("MIN_CAPACITY"),
        DS_HEAP_MIN_CAPACITY
    );

    zend_class_implements(php_ds_heap_ce, 3,
        spl_ce_Aggregate,
        spl_ce_Countable,
        php_json_serializable_ce);

    php_ds_register_heap_handlers();
}
