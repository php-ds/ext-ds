<?php
namespace Ds;

final class Vector implements Sequence
{
    /**
     * @inheritDoc
     */
    public function __construct($values = null) {}

    /**
     * @inheritDoc
     */
    public function allocate(int $capacity) {}

    /**
     * @inheritDoc
     */
    public function capacity(): int {}

    /**
     * @inheritDoc
     */
    public function clear() {}

    /**
     * @inheritDoc
     */
    public function contains(...$values): bool {}

    /**
     * @inheritDoc
     */
    public function copy(): Vector {}

    /**
     * @inheritDoc
     */
    public function count(): int {}

    /**
     * @inheritDoc
     */
    public function filter(callable $callback = null): Vector {}

    /**
     * @inheritDoc
     */
    public function find($value) {}

    /**
     * @inheritDoc
     */
    public function first() {}

    /**
     * @inheritDoc
     */
    public function get() {}

    /**
     * @inheritDoc
     */
    public function insert(int $index, ...$values) {}

    /**
     * @inheritDoc
     */
    public function isEmpty(): bool {}

    /**
     * @inheritDoc
     */
    public function join(string $glue = null): string {}

    /**
     * @inheritDoc
     */
    public function last() {}

    /**
     * @inheritDoc
     */
    public function map(callable $callback): Vector {}

    /**
     * @inheritDoc
     */
    public function pop() {}

    /**
     * @inheritDoc
     */
    public function push(...$values) {}

    /**
     * @inheritDoc
     */
    public function pushAll($values) {}

    /**
     * @inheritDoc
     */
    public function reduce(callable $callback, $initial = null) {}

    /**
     * @inheritDoc
     */
    public function remove(int $index) {}

    /**
     * @inheritDoc
     */
    public function reverse(): Vector {}

    /**
     * @inheritDoc
     */
    public function rotate(int $rotations) {}

    /**
     * @inheritDoc
     */
    public function set(int $index, $value) {}

    /**
     * @inheritDoc
     */
    public function shift() {}

    /**
     * @inheritDoc
     */
    public function slice(int $offset, int $length = null): Vector {}

    /**
     * @inheritDoc
     */
    public function sort(callable $comparator = null): Vector {}

    /**
     * @inheritDoc
     */
    public function toArray(): array {}

    /**
     * @inheritDoc
     */
    public function unshift(...$values) {}
}
