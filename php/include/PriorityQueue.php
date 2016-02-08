<?php
namespace Ds;

final class PriorityQueue implements Collection
{
    /**
     * Initializes a new priority queue.
     */
    public function __construct() {}

    /**
     * Ensures that enough memory is allocated for a specified capacity. This
     * potentially reduces the number of reallocations as the size increases.
     *
     * @param int $capacity The number of values for which capacity should be
     *                      allocated. Capacity will stay the same if this value
     *                      is less than or equal to the current capacity.
     */
    public function allocate(int $capacity) {}

    /**
     * Returns the current capacity of the priority queue.
     *
     * @return int
     */
    public function capacity(): int {}

    /**
     * @inheritDoc
     */
    public function clear() {}

    /**
     * @inheritDoc
     */
    public function copy() {}

    /**
     * @inheritDoc
     */
    public function count(): int {}

    /**
     * @inheritDoc
     */
    public function isEmpty(): bool {}

    /**
     * Returns the value with the highest priority in the priority queue.
     *
     * @return mixed
     */
    public function peek() {}

    /**
     * Returns and removes the value with the highest priority in the queue.
     *
     * @return mixed
     */
    public function pop() {}

    /**
     * Pushes a value into the queue, with a specified priority.
     *
     * @param mixed $value
     * @param int   $priority
     */
    public function push($value, int $priority) {}

    /**
     * @inheritDoc
     */
    public function toArray(): array {}
}
