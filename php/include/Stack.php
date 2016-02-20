<?php
namespace Ds;

final class Stack implements Collection
{
    /**
     * Creates an instance using the values of an array or Traversable object.
     *
     * @param array|\Traversable $values
     */
    public function __construct($values = null) {}

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
     * Returns the current capacity of the stack.
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
     * Returns the value at the top of the stack without removing it.
     *
     * @return mixed
     */
    public function peek() {}

    /**
     * Returns and removes the value at the top of the stack
     *
     * @return mixed
     */
    public function pop() {}

    /**
     * Pushes zero or more values onto the top of the stack.
     *
     * @param mixed ...$values
     */
    public function push(...$values) {}

    /**
     * @inheritDoc
     */
    public function toArray(): array {}
}
