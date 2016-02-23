<?php
namespace Ds;

final class Map implements Collection
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
     * Returns the current capacity of the map.
     *
     * @return int
     */
    public function capacity(): int {}

    /**
     * @inheritDoc
     */
    public function clear() {}

    /**
     * Returns whether an association for all of zero or more keys exist.
     *
     * @param mixed ...$keys
     *
     * @return bool true if at least one value was provided and the map
     *              contains all given keys, false otherwise.
     */
    public function containsKey(...$keys): bool {}

    /**
     * Returns whether an association for all of zero or more values exist.
     *
     * @param mixed ...$values
     *
     * @return bool true if at least one value was provided and the map
     *              contains all given values, false otherwise.
     */
    public function containsValues(...$values): bool {}

    /**
     * @inheritDoc
     */
    public function copy() {}

    /**
     * @inheritDoc
     */
    public function count(): int {}

    /**
     * Returns a new map containing only the values for which a callback
     * returns true. A boolean test will be used if a callback is not provided.
     *
     * @param callable|null $callback Accepts a key and a value, and returns:
     *                                true : include the value,
     *                                false: skip the value.
     *
     * @return Map
     */
    public function filter(callable $callback = null): Map {}

    /**
     * Returns the value associated with a key, or an optional default if the
     * key is not associated with a value.
     *
     * @param mixed $key
     * @param mixed $default
     *
     * @return mixed The associated value or fallback default if provided.
     *
     * @throws \OutOfBoundsException if no default was provided and the key is
     *                               not associated with a value.
     */
    public function get($key, $default = null) {}

    /**
     * @inheritDoc
     */
    public function isEmpty(): bool {}

    /**
     * Returns a set of all the keys in the map.
     *
     * @return Set
     */
    public function keys(): Set {}

    /**
     * Returns a new map using the results of applying a callback to each value.
     * The keys will be identical in both maps.
     *
     * @param callable $callback Accepts two arguments: key and value, should
     *                           return what the updated value will be.
     *
     * @return Map
     */
    public function map(callable $callback = null): Map {}

    /**
     * Returns a sequence of pairs representing all associations.
     *
     * @return Sequence
     */
    public function pairs(): Sequence {}

    /**
     * Associates a key with a value, replacing a previous association if there
     * was one.
     *
     * @param mixed $key
     * @param mixed $value
     */
    public function put($key, $value) {}

    /**
     * Creates associations for all keys and corresponding values of either an
     * array or iterable object.
     *
     * @param array|\Traversable $values
     */
    public function putAll($values) {}

    /**
     * Iteratively reduces the map to a single value using a callback.
     *
     * @param callable $callback Accepts the carry, key, and value, and
     *                           returns an updated carry value.
     *
     * @param mixed|null $initial Optional initial carry value.
     *
     * @return mixed The carry value of the final iteration, or the initial
     *               value if the map was empty.
     */
    public function reduce(callable $callback, $initial = null) {}

    /**
     * Removes a key's association from the map and returns the associated value
     * or a provided default if provided.
     *
     * @param mixed $key
     * @param mixed $default
     *
     * @return mixed The associated value or fallback default if provided.
     *
     * @throws \OutOfBoundsException if no default was provided and the key is
     *                               not associated with a value.
     */
    public function remove($key, $default = null) {}

    /**
     * Returns a reversed copy of the map.
     */
    public function reverse(): Map {}

    /**
     * Returns a sub-sequence of a given length starting at a specified offset.
     *
     * @param int $offset If the offset is non-negative, the map will start
     *                    at that offset in the map. If offset is negative,
     *                    the map will start that far from the end.
     *
     * @param int $length If a length is given and is positive, the resulting
     *                    set will have up to that many pairs in it.
     *                    If the requested length results in an overflow, only
     *                    pairs up to the end of the map will be included.
     *
     *                    If a length is given and is negative, the map
     *                    will stop that many pairs from the end.
     *
     *                    If a length is not provided, the resulting map
     *                    will contains all pairs between the offset and the
     *                    end of the map.
     *
     * @return Map
     */
    public function slice(int $offset, int $length = null): Map {}

    /**
     * Returns a sorted copy of the map, based on an optional callable
     * comparator. The map will be sorted by key if a comparator is not given.
     *
     * @param callable|null $comparator Accepts two values to be compared.
     *                                  Should return the result of a <=> b.
     *
     * @return Sequence
     */
    public function sort(callable $comparator = null): Map {}

    /**
     * @inheritDoc
     */
    public function toArray(): array {}

    /**
     * Returns a sequence of all the associated values in the Map.
     *
     * @return Sequence
     */
    public function values(): Sequence {}
}
