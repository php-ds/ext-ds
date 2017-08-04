#include "../ds_common.h"

#include "../php/iterators/php_ds_priority_queue_iterator.h"
#include "../php/handlers/php_ds_priority_queue_handlers.h"
#include "../php/classes/php_ds_priority_queue_ce.h"

#include "ds_priority_queue.h"

#define LEFT(x)  (((x) << 1) + 1)
#define RIGHT(x) (((x) << 1) + 2)
#define PARENT(x) ((x - 1) >> 1)

// Insertion stamp, for equal priority comparison fallback.
#define STAMP(n) (Z_NEXT((n).value))

static uint32_t capacity_for_size(uint32_t size)
{
    uint32_t c = MAX(size, DS_PRIORITY_QUEUE_MIN_CAPACITY);

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
static inline int compare(ds_priority_queue_node_t a, ds_priority_queue_node_t b)
{
    return ((a.priority == b.priority) ? (STAMP(a) < STAMP(b) ? 1 : -1) :
            (a.priority >  b.priority) ? 1 : -1);
}

static inline ds_priority_queue_node_t *reallocate_nodes(ds_priority_queue_node_t *nodes, uint32_t capacity)
{
    return erealloc(nodes, capacity * sizeof(ds_priority_queue_node_t));
}

static inline ds_priority_queue_node_t *allocate_nodes(uint32_t capacity)
{
    return ecalloc(capacity, sizeof(ds_priority_queue_node_t));
}

static inline void reallocate_to_capacity(ds_priority_queue_t *queue, uint32_t capacity)
{
    queue->nodes = reallocate_nodes(queue->nodes, capacity);
    queue->capacity = capacity;
}

static inline void increase_capacity(ds_priority_queue_t *queue)
{
    reallocate_to_capacity(queue, queue->capacity * 2);
}

void ds_priority_queue_allocate(ds_priority_queue_t *queue, uint32_t capacity)
{
    if (capacity > queue->capacity) {
        reallocate_to_capacity(queue, capacity_for_size(capacity));
    }
}

ds_priority_queue_t *ds_priority_queue()
{
    ds_priority_queue_t *queue = ecalloc(1, sizeof(ds_priority_queue_t));

    queue->nodes    = allocate_nodes(DS_PRIORITY_QUEUE_MIN_CAPACITY);
    queue->capacity = DS_PRIORITY_QUEUE_MIN_CAPACITY;
    queue->size     = 0;
    queue->next     = 0;

    return queue;
}

uint32_t ds_priority_queue_capacity(ds_priority_queue_t *queue)
{
    return queue->capacity;
}

void ds_priority_queue_push(ds_priority_queue_t *queue, zval *value, zend_long priority)
{
    uint32_t parent;
    uint32_t index;

    ds_priority_queue_node_t *nodes;
    ds_priority_queue_node_t *node;

    if (queue->size == queue->capacity) {
        increase_capacity(queue);
    }

    nodes = queue->nodes;

    for (index = queue->size; index > 0; index = parent) {

        // Move up the heap
        parent = PARENT(index);

        if (priority <= nodes[parent].priority) {
            break;
        }

        nodes[index] = nodes[parent];
    }

    node = &queue->nodes[index];

    // Initialize the new node
    STAMP(*node) = ++queue->next;
    ZVAL_COPY(&node->value, value);
    node->priority = priority;

    queue->size++;
}

static inline void ds_priority_queue_compact(ds_priority_queue_t *queue)
{
    if (queue->size <= (queue->capacity / 4) && (queue->capacity / 2) >= DS_PRIORITY_QUEUE_MIN_CAPACITY) {
        reallocate_to_capacity(queue, queue->capacity / 2);
    }
}

void ds_priority_queue_pop(ds_priority_queue_t *queue, zval *return_value)
{
    uint32_t index, swap;

    ds_priority_queue_node_t bottom;
    ds_priority_queue_node_t *nodes = queue->nodes;

    const uint32_t size = queue->size;
    const uint32_t half = (size - 1) / 2;

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

    // Reduce the size of the buffer if the size has dropped below a threshold.
    ds_priority_queue_compact(queue);
}

static ds_priority_queue_node_t *copy_nodes(ds_priority_queue_t *queue)
{
    ds_priority_queue_node_t *copies = allocate_nodes(queue->capacity);

    ds_priority_queue_node_t *src = queue->nodes;
    ds_priority_queue_node_t *end = queue->nodes + queue->size;
    ds_priority_queue_node_t *dst = copies;

    for (; src < end; ++src, ++dst) {
        ZVAL_COPY(&dst->value, &src->value); // Also copies stamp
        dst->priority = src->priority;
    }

    return copies;
}

ds_priority_queue_t *ds_priority_queue_clone(ds_priority_queue_t * queue)
{
    ds_priority_queue_t *clone = ecalloc(1, sizeof(ds_priority_queue_t));

    clone->nodes    = copy_nodes(queue);
    clone->capacity = queue->capacity;
    clone->size     = queue->size;
    clone->next     = queue->next;

    return clone;
}

zval *ds_priority_queue_peek(ds_priority_queue_t *queue)
{
    if (queue->size == 0) {
        NOT_ALLOWED_WHEN_EMPTY();
        return NULL;
    }

    return &queue->nodes[0].value;
}

static int priority_sort(const void *a, const void *b)
{
    return compare(*((ds_priority_queue_node_t *) b), *((ds_priority_queue_node_t *) a));
}

ds_priority_queue_node_t* ds_priority_queue_create_sorted_buffer(ds_priority_queue_t *queue)
{
    ds_priority_queue_node_t *buffer = allocate_nodes(queue->size);

    memcpy(buffer, queue->nodes, queue->size * sizeof(ds_priority_queue_node_t));
    qsort(buffer, queue->size, sizeof(ds_priority_queue_node_t), priority_sort);

    return buffer;
}

void ds_priority_queue_to_array(ds_priority_queue_t *queue, zval *array)
{
    if (DS_PRIORITY_QUEUE_IS_EMPTY(queue)) {
        array_init(array);

    } else {
        ds_priority_queue_node_t *pos, *end, *buf;

        buf = ds_priority_queue_create_sorted_buffer(queue);
        pos = buf;
        end = buf + queue->size;

        array_init_size(array, queue->size);

        for (; pos < end; ++pos) {
            add_next_index_zval(array, &pos->value);
            Z_TRY_ADDREF_P(&pos->value);
        }

        efree(buf);
    }
}

void ds_priority_queue_clear(ds_priority_queue_t *queue)
{
    ds_priority_queue_node_t *pos = queue->nodes;
    ds_priority_queue_node_t *end = pos + queue->size;

    for (; pos < end; ++pos) {
        DTOR_AND_UNDEF(&pos->value);
    }

    queue->size = 0;
    reallocate_to_capacity(queue, DS_PRIORITY_QUEUE_MIN_CAPACITY);
}

void ds_priority_queue_free(ds_priority_queue_t *queue)
{
    ds_priority_queue_clear(queue);
    efree(queue->nodes);
    efree(queue);
}
