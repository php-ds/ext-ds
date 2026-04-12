#include "../common.h"
#include "ds_heap.h"

#define LEFT(x)   (((x) * 2) + 1)
#define RIGHT(x)  (((x) * 2) + 2)
#define PARENT(x) (((x) - 1) / 2)

static inline uint32_t ds_heap_get_capacity_for_size(uint32_t size)
{
    return ds_next_power_of_2(size, DS_HEAP_MIN_CAPACITY);
}

static inline void ds_heap_reallocate(ds_heap_t *heap, uint32_t capacity)
{
    heap->nodes = erealloc(heap->nodes, capacity * sizeof(zval));
    heap->capacity = capacity;
}

static inline void ds_heap_increase_capacity(ds_heap_t *heap)
{
    ds_heap_reallocate(heap, heap->capacity * 2);
}

static inline void ds_heap_compact(ds_heap_t *heap)
{
    if (heap->size <= (heap->capacity / 4) && (heap->capacity / 2) >= DS_HEAP_MIN_CAPACITY) {
        ds_heap_reallocate(heap, heap->capacity / 2);
    }
}

int ds_heap_default_compare(zval *a, zval *b)
{
    zval retval;
    if (compare_function(&retval, a, b) == SUCCESS) {
        return (int) zval_get_long(&retval);
    }
    return 0;
}

ds_heap_t *ds_heap()
{
    ds_heap_t *heap = ecalloc(1, sizeof(ds_heap_t));

    heap->nodes    = ds_allocate_zval_buffer(DS_HEAP_MIN_CAPACITY);
    heap->capacity = DS_HEAP_MIN_CAPACITY;
    heap->size     = 0;

    return heap;
}

ds_heap_t *ds_heap_clone(ds_heap_t *heap)
{
    ds_heap_t *clone = ecalloc(1, sizeof(ds_heap_t));

    clone->nodes    = ds_allocate_zval_buffer(heap->capacity);
    clone->capacity = heap->capacity;
    clone->size     = heap->size;

    for (uint32_t i = 0; i < heap->size; i++) {
        ZVAL_COPY(&clone->nodes[i], &heap->nodes[i]);
    }

    return clone;
}

void ds_heap_separate(ds_heap_t **heap)
{
    if ((*heap)->refs > 0) {
        (*heap)->refs--;
        *heap = ds_heap_clone(*heap);
    }
}

void ds_heap_release(ds_heap_t *heap)
{
    if (heap->refs > 0) {
        heap->refs--;
    } else {
        ds_heap_free(heap);
    }
}

void ds_heap_allocate(ds_heap_t *heap, uint32_t capacity)
{
    capacity = ds_heap_get_capacity_for_size(capacity);

    if (capacity > heap->capacity) {
        ds_heap_reallocate(heap, capacity);
    }
}

uint32_t ds_heap_capacity(ds_heap_t *heap)
{
    return heap->capacity;
}

zval *ds_heap_peek(ds_heap_t *heap)
{
    if (heap->size == 0) {
        NOT_ALLOWED_WHEN_EMPTY();
        return NULL;
    }

    return &heap->nodes[0];
}

void ds_heap_push(ds_heap_t *heap, zval *value, ds_heap_compare_func_t compare)
{
    uint32_t index;
    uint32_t parent;

    if (heap->size == heap->capacity) {
        ds_heap_increase_capacity(heap);
    }

    // Sift up: find the correct position for the new value.
    for (index = heap->size; index > 0; index = parent) {
        parent = PARENT(index);

        if (compare(value, &heap->nodes[parent]) <= 0) {
            break;
        }

        // Move parent down.
        ZVAL_COPY_VALUE(&heap->nodes[index], &heap->nodes[parent]);
    }

    ZVAL_COPY(&heap->nodes[index], value);
    heap->size++;
}

void ds_heap_pop(ds_heap_t *heap, zval *return_value, ds_heap_compare_func_t compare)
{
    uint32_t index;
    uint32_t swap;
    uint32_t half;
    zval bottom;

    if (heap->size == 0) {
        NOT_ALLOWED_WHEN_EMPTY();
        ZVAL_NULL(return_value);
        return;
    }

    // Return the root.
    if (return_value) {
        ZVAL_COPY(return_value, &heap->nodes[0]);
    }

    heap->size--;

    if (heap->size == 0) {
        zval_ptr_dtor(&heap->nodes[0]);
        ds_heap_compact(heap);
        return;
    }

    // Move last element to root position and sift down.
    ZVAL_COPY_VALUE(&bottom, &heap->nodes[heap->size]);
    zval_ptr_dtor(&heap->nodes[0]);

    half = heap->size / 2;

    for (index = 0; index < half; index = swap) {
        swap = LEFT(index);

        // Pick the larger child.
        if (swap + 1 < heap->size && compare(&heap->nodes[swap], &heap->nodes[swap + 1]) < 0) {
            swap++;
        }

        // If the bottom value is >= the larger child, we're done.
        if (compare(&bottom, &heap->nodes[swap]) >= 0) {
            break;
        }

        ZVAL_COPY_VALUE(&heap->nodes[index], &heap->nodes[swap]);
    }

    ZVAL_COPY_VALUE(&heap->nodes[index], &bottom);
    ds_heap_compact(heap);
}

typedef struct _ds_heap_push_all_data {
    ds_heap_t               *heap;
    ds_heap_compare_func_t   compare;
} ds_heap_push_all_data;

static int ds_heap_iterator_add(zend_object_iterator *iterator, void *puser)
{
    ds_heap_push_all_data *data = (ds_heap_push_all_data *) puser;
    ds_heap_push(data->heap, iterator->funcs->get_current_data(iterator), data->compare);
    return ZEND_HASH_APPLY_KEEP;
}

void ds_heap_push_all(ds_heap_t *heap, zval *values, ds_heap_compare_func_t compare)
{
    if (!values) {
        return;
    }

    if (ds_is_array(values)) {
        zval *value;
        ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(values), value) {
            ds_heap_push(heap, value, compare);
        }
        ZEND_HASH_FOREACH_END();
        return;
    }

    if (ds_is_traversable(values)) {
        ds_heap_push_all_data data = { heap, compare };
        spl_iterator_apply(values, ds_heap_iterator_add, &data);
        return;
    }

    ARRAY_OR_TRAVERSABLE_REQUIRED();
}

void ds_heap_to_array(ds_heap_t *heap, zval *array, ds_heap_compare_func_t compare)
{
    if (heap->size == 0) {
        array_init(array);
        return;
    }

    // Clone the heap and pop all elements in order.
    ds_heap_t *clone = ds_heap_clone(heap);

    array_init_size(array, clone->size);

    while (clone->size > 0) {
        zval value;
        ds_heap_pop(clone, &value, compare);
        add_next_index_zval(array, &value);
    }

    ds_heap_free(clone);
}

void ds_heap_clear(ds_heap_t *heap)
{
    for (uint32_t i = 0; i < heap->size; i++) {
        zval_ptr_dtor(&heap->nodes[i]);
    }

    heap->size = 0;
    ds_heap_reallocate(heap, DS_HEAP_MIN_CAPACITY);
}

void ds_heap_free(ds_heap_t *heap)
{
    for (uint32_t i = 0; i < heap->size; i++) {
        zval_ptr_dtor(&heap->nodes[i]);
    }

    efree(heap->nodes);
    efree(heap);
}
