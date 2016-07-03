#include "../common.h"
#include "../php/classes/php_ce_priority_queue.h"
#include "../php/handlers/php_priority_queue_handlers.h"
#include "ds_priority_queue.h"

#define LEFT(x) (((x) << 1) + 1)
#define RIGHT(x) (((x) << 1) + 2)
#define PARENT(x) ((x - 1) >> 1)

// Insertion stamp, for equal priority comparison fallback.
#define STAMP(n) (Z_NEXT((n).value))

static uint32_t capacity_for_size(uint32_t size)
{
    uint32_t c = MAX(size, PRIORITY_QUEUE_MIN_CAPACITY);

    c--;
    c |= c >> 1;
    c |= c >> 2;
    c |= c >> 4;
    c |= c >> 8;
    c |= c >> 16;
    c++;

    return c;
}

// Priority comparison, with insertion stamp fallback.
static inline int compare(PriorityNode a, PriorityNode b)
{
    return ((a.priority == b.priority) ? (STAMP(a) < STAMP(b) ? 1 : -1) :
            (a.priority >  b.priority) ? 1 : -1);
}

static inline PriorityNode *reallocate_nodes(PriorityNode *nodes, uint32_t capacity)
{
    return erealloc(nodes, capacity * sizeof(PriorityNode));
}

static inline PriorityNode *allocate_nodes(uint32_t capacity)
{
    return ecalloc(capacity, sizeof(PriorityNode));
}

static inline void reallocate_to_capacity(PriorityQueue *queue, uint32_t capacity)
{
    queue->nodes = reallocate_nodes(queue->nodes, capacity);
    queue->capacity = capacity;
}

static inline void increase_capacity(PriorityQueue *queue)
{
    reallocate_to_capacity(queue, queue->capacity << 1);
}

void priority_queue_user_allocate(PriorityQueue *queue, uint32_t capacity)
{
    if (capacity > queue->capacity) {
        reallocate_to_capacity(queue, capacity_for_size(capacity));
    }
}

PriorityQueue *priority_queue_init()
{
    PriorityQueue *queue = ecalloc(1, sizeof(PriorityQueue));
    zend_object_std_init(&queue->std, priority_queue_ce);

    queue->std.handlers = &priority_queue_handlers;

    queue->nodes    = allocate_nodes(PRIORITY_QUEUE_MIN_CAPACITY);
    queue->capacity = PRIORITY_QUEUE_MIN_CAPACITY;
    queue->size     = 0;
    queue->next     = 0;

    return queue;
}

zend_object *priority_queue_init_object(zend_class_entry *ce)
{
    return &priority_queue_init()->std;
}

uint32_t priority_queue_capacity(PriorityQueue *queue)
{
    return queue->capacity;
}

void priority_queue_push(PriorityQueue *queue, zval *value, zend_long priority)
{
    uint32_t parent;
    uint32_t index;

    PriorityNode *nodes;
    PriorityNode *node;

    if (queue->size == queue->capacity) {
        increase_capacity(queue);
    }

    nodes = queue->nodes;

    for (index = queue->size; index > 0; index = parent) {

        //
        parent = PARENT(index);

        if (priority <= nodes[parent].priority) {
            break;
        }

        nodes[index] = nodes[parent];
    }

    //
    node = &queue->nodes[index];

    //
    STAMP(*node) = ++queue->next;
    ZVAL_COPY(&node->value, value);
    node->priority = priority;

    queue->size++;
}

void priority_queue_pop(PriorityQueue *queue, zval *return_value)
{
    uint32_t index, swap;

    PriorityNode bottom;
    PriorityNode *nodes = queue->nodes;

    const uint32_t size = queue->size;
    const uint32_t half = (size - 1) >> 1;

    if (size == 0) {
        NOT_ALLOWED_WHEN_EMPTY();
        ZVAL_NULL(return_value);
        return;
    }

    if (return_value) {
        ZVAL_COPY(return_value, &(nodes[0].value));
    }

    bottom = nodes[size - 1];
    DTOR_AND_UNDEF(&(nodes[0].value));

    queue->size--;

    for (index = 0; index < half; index = swap) {
        swap = LEFT(index);

        if (swap < queue->size && compare(nodes[swap], nodes[swap + 1]) < 0) {
            swap++;
        }

        if (compare(nodes[swap], bottom) < 0) {
            break;
        }

        nodes[index] = nodes[swap];
    }
    nodes[index] = bottom;

    if (queue->size <= queue->capacity >> 2) {
        reallocate_to_capacity(queue, queue->capacity >> 1);
    }
}

static PriorityNode *copy_nodes(PriorityQueue *queue)
{
    PriorityNode *copies = allocate_nodes(queue->capacity);

    PriorityNode *src = queue->nodes;
    PriorityNode *dst = copies;
    PriorityNode *end = src + queue->size;

    for (; src < end; ++src, ++dst) {
        ZVAL_COPY(&dst->value, &src->value);
        dst->priority = src->priority;
    }

    return copies;
}

zend_object *priority_queue_init_clone(PriorityQueue *queue)
{
    PriorityQueue *clone = ecalloc(1, sizeof(PriorityQueue));
    zend_object_std_init(&clone->std, priority_queue_ce);

    clone->std.handlers = &priority_queue_handlers;

    clone->nodes    = copy_nodes(queue);
    clone->capacity = queue->capacity;
    clone->size     = queue->size;
    clone->next     = queue->next;

    return &clone->std;
}

zval *priority_queue_peek(PriorityQueue *queue)
{
    if (queue->size == 0) {
        NOT_ALLOWED_WHEN_EMPTY();
        return NULL;
    }

    return &queue->nodes[0].value;
}

static int priority_sort(const void *a, const void *b)
{
    return compare(*((PriorityNode *) b), *((PriorityNode *) a));
}

PriorityNode* create_sorted_buffer(PriorityQueue *queue)
{
    PriorityNode *buffer = allocate_nodes(queue->size);

    memcpy(buffer, queue->nodes, queue->size * sizeof(PriorityNode));
    qsort(buffer, queue->size, sizeof(PriorityNode), priority_sort);

    return buffer;
}

void priority_queue_to_array(PriorityQueue *queue, zval *array)
{
    if (PRIORITY_QUEUE_IS_EMPTY(queue)) {
        array_init(array);

    } else {

        PriorityNode *pos, *end;

        PriorityNode *buffer = create_sorted_buffer(queue);

        pos = buffer;
        end = pos + queue->size;

        array_init_size(array, queue->size);

        for (; pos < end; ++pos) {
            add_next_index_zval(array, &pos->value);
            Z_TRY_ADDREF_P(&pos->value);
        }

        efree(buffer);
    }
}

void priority_queue_clear(PriorityQueue *queue)
{
    PriorityNode *pos = queue->nodes;
    PriorityNode *end = pos + queue->size;

    for (; pos < end; ++pos) {
        DTOR_AND_UNDEF(&pos->value);
    }

    queue->size = 0;
    reallocate_to_capacity(queue, PRIORITY_QUEUE_MIN_CAPACITY);
}

void priority_queue_destroy(PriorityQueue *queue)
{
    priority_queue_clear(queue);
    efree(queue->nodes);
}

int priority_queue_serialize(zval *object, unsigned char **buffer, size_t *length, zend_serialize_data *data)
{
    PriorityQueue *queue = Z_PRIORITY_QUEUE_P(object);

    php_serialize_data_t serialize_data = (php_serialize_data_t) data;
    PHP_VAR_SERIALIZE_INIT(serialize_data);

    if (queue->size == 0) {
        SERIALIZE_SET_ZSTR(ZSTR_EMPTY_ALLOC());

    } else {

        PriorityNode *nodes = create_sorted_buffer(queue);
        PriorityNode *pos   = nodes;
        PriorityNode *end   = nodes + queue->size;

        smart_str buf = {0};

        for (; pos < end; ++pos) {

            zval priority;
            ZVAL_LONG(&priority, pos->priority);

            php_var_serialize(&buf, &pos->value, &serialize_data);
            php_var_serialize(&buf, &priority, &serialize_data);
        }

        smart_str_0(&buf);
        SERIALIZE_SET_ZSTR(buf.s);
        zend_string_release(buf.s);

        efree(nodes);
    }

    PHP_VAR_SERIALIZE_DESTROY(serialize_data);
    return SUCCESS;
}

int priority_queue_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t length, zend_unserialize_data *data)
{
    PriorityQueue *queue = priority_queue_init();

    php_unserialize_data_t unserialize_data = (php_unserialize_data_t) data;

    const unsigned char *pos = buffer;
    const unsigned char *max = buffer + length;

    PHP_VAR_UNSERIALIZE_INIT(unserialize_data);

    ZVAL_OBJ(object, &queue->std);

    while (*pos != '}') {

        zval *value, *priority;

        value = var_tmp_var(&unserialize_data);
        if (php_var_unserialize(value, &pos, max, &unserialize_data)) {
            var_push_dtor(&unserialize_data, value);
        } else {
            goto error;
        }

        priority = var_tmp_var(&unserialize_data);
        if (php_var_unserialize(priority, &pos, max, &unserialize_data)) {
            var_push_dtor(&unserialize_data, priority);
        } else {
            goto error;
        }

        if (Z_TYPE_P(priority) != IS_LONG) {
            goto error;
        }

        priority_queue_push(queue, value, Z_LVAL_P(priority));
    }

    if (*(++pos) != '\0') {
        goto error;
    }

    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    return SUCCESS;

error:
    PHP_VAR_UNSERIALIZE_DESTROY(unserialize_data);
    UNSERIALIZE_ERROR();
    return FAILURE;
}
