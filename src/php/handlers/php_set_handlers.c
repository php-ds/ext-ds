#include "php_set_handlers.h"
#include "php_common_handlers.h"
// #include "php.h"
// #include "ext/spl/spl_exceptions.h"
#include "../../ds/ds_set.h"
#include "../classes/php_ce_set.h"
// #include "php_set_handlers.h"

zend_object_handlers set_handlers;

static zval *set_read_dimension(zval *obj, zval *offset, int type, zval *return_value)
{
    Set *set = Z_SET_P(obj);

    if (offset == NULL) {
        set_add(set, obj);
        return NULL;
    }

    if (Z_TYPE_P(offset) != IS_LONG) {
        INTEGER_INDEX_REQUIRED(offset);
        return NULL;
    }

    return set_get(set, Z_LVAL_P(offset));
}

static void set_write_dimension(zval *obj, zval *offset, zval *value)
{
    Set *set = Z_SET_P(obj);

    if (offset == NULL) {
        set_add(set, value);
        return;
    }

    ARRAY_ACCESS_BY_KEY_NOT_SUPPORTED();
}

static int set_count_elements(zval *obj, zend_long *count)
{
    *count = SET_SIZE(Z_SET_P(obj));
    return SUCCESS;
}

static void set_free_object(zend_object *object)
{
    Set *set = (Set*) object;
    zend_object_std_dtor(&set->std);
    set_free(set);
}

static HashTable *set_get_debug_info(zval *obj, int *is_temp)
{
    zval arr;
    Set *set = Z_SET_P(obj);

    *is_temp = 1;

    set_to_array(set, &arr);
    return Z_ARRVAL(arr);
}

static zend_object *set_clone_obj(zval *obj)
{
    Set *set = Z_SET_P(obj);
    return set_create_clone(set);
}

static inline bool is_set(zval *op)
{
    return Z_TYPE_P(op) == IS_OBJECT && instanceof_function(Z_OBJCE_P(op), set_ce);
}

static int set_do_operation_ex(zend_uchar opcode, zval *result, zval *op1, zval *op2)
{
    Set *set = Z_SET_P(op1);

    switch (opcode) {

        // &, intersect which creates a new Set.
        case ZEND_BW_AND:
            if (is_set(op2)) {
                if (op1 == result) {
                    // &=, intersect which modifies the Set.
                    set_assign_intersect(set, Z_SET_P(op2));
                } else {
                    set_intersect(set, Z_SET_P(op2), result);
                }
                return SUCCESS;
            }
            break;

        // |, union which creates a new Set.
        case ZEND_BW_OR:
            if (is_set(op2)) {
                set_union(set, Z_SET_P(op2), result);
                return SUCCESS;
            }
            break;

        // |=, union which modifies the Set.
        case ZEND_ASSIGN_BW_OR:
            if (is_set(op2)) {
                set_assign_union(set, Z_SET_P(op2));
                return SUCCESS;
            }
            break;

        // ^, xor which creates a new Set.
        case ZEND_BW_XOR:
            if (is_set(op2)) {
                set_xor(set, Z_SET_P(op2), result);
                return SUCCESS;
            }
            break;

        // ^=, xor which modifies the Set.
        case ZEND_ASSIGN_BW_XOR:
            if (is_set(op2)) {
                set_assign_xor(set, Z_SET_P(op2));
                return SUCCESS;
            }
            break;

        // -, diff which creates a new Set.
        case ZEND_SUB:
            if (is_set(op2)) {
                set_diff(set, Z_SET_P(op2), result);
                return SUCCESS;
            }
            break;

        // -=, diff which modifies the Set.
        case ZEND_ASSIGN_SUB:
            if (is_set(op2)) {
                set_assign_diff(set, Z_SET_P(op2));
                return SUCCESS;
            }
            break;
    }

    // Operator not supported?
    return FAILURE;
}

static int set_do_operation(zend_uchar opcode, zval *result, zval *op1, zval *op2)
{
    zval op1_copy;
    int retval;

    if (result == op1) {
        ZVAL_COPY_VALUE(&op1_copy, op1);
        op1 = &op1_copy;
    }

    retval = set_do_operation_ex(opcode, result, op1, op2);

    if (retval == SUCCESS && op1 == &op1_copy) {
        zval_dtor(op1);
    }

    return retval;
}

void register_set_handlers()
{
    memcpy(&set_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    set_handlers.offset = XtOffsetOf(Set, std);

    set_handlers.free_obj            = set_free_object;
    set_handlers.clone_obj           = set_clone_obj;
    set_handlers.get_debug_info      = set_get_debug_info;
    set_handlers.count_elements      = set_count_elements;
    set_handlers.read_dimension      = set_read_dimension;
    set_handlers.write_dimension     = set_write_dimension;
    set_handlers.cast_object         = ds_default_cast_object;
    // set_handlers.do_operation        = set_do_operation;
}
