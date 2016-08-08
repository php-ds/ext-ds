#include "php_common_handlers.h"
#include "php_set_handlers.h"
#include "../../ds/ds_set.h"
#include "../objects/php_set.h"
#include "../classes/php_set_ce.h"

zend_object_handlers php_ds_set_handlers;

static zval *php_ds_set_read_dimension(zval *obj, zval *offset, int type, zval *rv)
{
    ds_set_t *set = Z_DS_SET_P(obj);

    if (offset == NULL) {
        ds_set_add(set, obj);
        return NULL;
    }

    if (Z_TYPE_P(offset) != IS_LONG) {
        INTEGER_INDEX_REQUIRED(offset);
        return NULL;
    }

    // Only support read, not write.
    if (type != BP_VAR_R) {
        return &EG(uninitialized_zval);
    }

    return ds_set_get(set, Z_LVAL_P(offset));
}

static void php_ds_set_write_dimension(zval *obj, zval *offset, zval *value)
{
    if (offset == NULL) {
        ds_set_add(Z_DS_SET_P(obj), value);
        return;
    }

    ARRAY_ACCESS_BY_KEY_NOT_SUPPORTED();
}

static int php_ds_set_count_elements(zval *obj, zend_long *count)
{
    *count = DS_SET_SIZE(Z_DS_SET_P(obj));
    return SUCCESS;
}

static void php_ds_set_free_object(zend_object *object)
{
    php_ds_set_t *obj = (php_ds_set_t*) object;
    zend_object_std_dtor(&obj->std);
    ds_set_free(obj->set);
}

static HashTable *php_ds_set_get_debug_info(zval *obj, int *is_temp)
{
    zval arr;
    ds_set_t *set = Z_DS_SET_P(obj);

    *is_temp = 1;

    ds_set_to_array(set, &arr);
    return Z_ARRVAL(arr);
}

static zend_object *php_ds_set_clone_obj(zval *obj)
{
    return php_ds_set_create_clone(Z_DS_SET_P(obj));
}

// static inline bool is_php_ds_set(zval *op)
// {
//     return Z_TYPE_P(op) == IS_OBJECT &&
//         instanceof_function(Z_OBJCE_P(op), php_ds_set_ce);
// }

// static int ds_set_do_operation_ex(zend_uchar opcode, zval *result, zval *op1, zval *op2)
// {
//     ds_set_t *set = Z_DS_SET_P(op1);

//     switch (opcode) {

//         // &, intersect which creates a new set.
//         case ZEND_BW_AND:
//             if (is_php_ds_set(op2)) {
//                 if (op1 == result) {
//                     // &=, intersect which modifies the set.
//                     ds_set_assign_intersect(set, Z_DS_SET_P(op2));
//                 } else {
//                     ZVAL_DS_SET(result, ds_set_intersect(set, Z_DS_SET_P(op2)));
//                 }
//                 return SUCCESS;
//             }
//             break;

//         // |, union which creates a new set.
//         case ZEND_BW_OR:
//             if (is_php_ds_set(op2)) {
//                 ZVAL_DS_SET(result, ds_set_union(set, Z_DS_SET_P(op2)));
//                 return SUCCESS;
//             }
//             break;

//         // |=, union which modifies the set.
//         case ZEND_ASSIGN_BW_OR:
//             if (is_php_ds_set(op2)) {
//                 ds_set_assign_union(set, Z_DS_SET_P(op2));
//                 return SUCCESS;
//             }
//             break;

//         // ^, xor which creates a new set.
//         case ZEND_BW_XOR:
//             if (is_php_ds_set(op2)) {
//                 ZVAL_DS_SET(result, ds_set_xor(set, Z_DS_SET_P(op2)));
//                 return SUCCESS;
//             }
//             break;

//         // ^=, xor which modifies the set.
//         case ZEND_ASSIGN_BW_XOR:
//             if (is_php_ds_set(op2)) {
//                 ds_set_assign_xor(set, Z_DS_SET_P(op2));
//                 return SUCCESS;
//             }
//             break;

//         // -, diff which creates a new set.
//         case ZEND_SUB:
//             if (is_php_ds_set(op2)) {
//                 ZVAL_DS_SET(result, ds_set_diff(set, Z_DS_SET_P(op2)));
//                 return SUCCESS;
//             }
//             break;

//         // -=, diff which modifies the set.
//         case ZEND_ASSIGN_SUB:
//             if (is_php_ds_set(op2)) {
//                 ds_set_assign_diff(set, Z_DS_SET_P(op2));
//                 return SUCCESS;
//             }
//             break;
//     }

//     // Operator not supported?
//     return FAILURE;
// }

// static int ds_set_do_operation(zend_uchar opcode, zval *result, zval *op1, zval *op2)
// {
//     zval op1_copy;
//     int retval;

//     if (result == op1) {
//         ZVAL_COPY_VALUE(&op1_copy, op1);
//         op1 = &op1_copy;
//     }

//     retval = ds_set_do_operation_ex(opcode, result, op1, op2);

//     if (retval == SUCCESS && op1 == &op1_copy) {
//         zval_dtor(op1);
//     }

//     return retval;
// }

static HashTable *php_ds_set_get_gc(zval *obj, zval **gc_data, int *gc_count)
{
    ds_set_t *set = Z_DS_SET_P(obj);

    if (DS_SET_IS_EMPTY(set)) {
        *gc_data  = NULL;
        *gc_count = 0;

    } else {
        *gc_data  = (zval*) set->table->buckets;
        *gc_count = (int)   set->table->next * 2;
    }

    return NULL;
}

void php_ds_register_set_handlers()
{
    memcpy(&php_ds_set_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    php_ds_set_handlers.offset = XtOffsetOf(php_ds_set_t, std);

    php_ds_set_handlers.get_gc              = php_ds_set_get_gc;
    php_ds_set_handlers.free_obj            = php_ds_set_free_object;
    php_ds_set_handlers.clone_obj           = php_ds_set_clone_obj;
    php_ds_set_handlers.get_debug_info      = php_ds_set_get_debug_info;
    php_ds_set_handlers.count_elements      = php_ds_set_count_elements;
    php_ds_set_handlers.read_dimension      = php_ds_set_read_dimension;
    php_ds_set_handlers.write_dimension     = php_ds_set_write_dimension;
    php_ds_set_handlers.cast_object         = php_ds_default_cast_object;
    // php_ds_set_handlers.do_operation        = ds_set_do_operation;
}
