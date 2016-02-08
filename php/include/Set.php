<?php
namespace Ds;

final class Set implements Collection
{
    /**
     * Creates a new set using the values of an array or Traversable object.
     * The keys of either will not be preserved.
     *
     * @param array|Traversable $values
     */
    public function __construct($values = null) {}

    /**
     * Adds zero or more values to the set.
     *
     * @param mixed ...$values
     */
    public function add(...$values) {}

    /**
     * Adds all values in an array or iterable object to the set.
     *
     * @param array|Traversable $values
     */
    public function addAll($values) {}

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
     * Returns the current capacity of the set.
     *
     * @return int
     */
    public function capacity(): int {}

    /**
     * @inheritDoc
     */
    public function clear() {}

    /**
     * Determines whether the set contains all of zero or more values.
     *
     * @param mixed ...$values
     *
     * @return bool true if at least one value was provided and the set
     *              contains all given values, false otherwise.
     */
    public function contains(...$values): bool {}

    /**
     * @inheritDoc
     */
    public function copy() {}

    /**
     * @inheritDoc
     */
    public function count(): int {}

    /**
     * Creates a new set using values from this set that aren't in another set.
     *
     * Formally: A \ B = {x ∈ A | x ∉ B}
     * Bitwise:  A - B
     *
     * @param Set $set
     *
     * @return Set
     */
    public function difference(Set $set): Set {}

    /**
     * Creates a new set using values in either this set or in another set,
     * but not in both.
     *
     * Formally: A ⊖ B = {x : x ∈ (A \ B) ∪ (B \ A)}
     * Bitwise:  A ^ B
     *
     * @param Set $set
     *
     * @return Set
     */
    public function exclusive(Set $set): Set {}

    /**
     * Returns a new set containing only the values for which a callback
     * returns true. A boolean test will be used if a callback is not provided.
     *
     * @param callable|null $callback Accepts a value, returns a boolean result:
     *                                true : include the value,
     *                                false: skip the value.
     *
     * @return Set
     */
    public function filter(callable $callback = null): Set {}

    /**
     * Returns the first value in the set.
     *
     * @return mixed the first value in the set.
     */
    public function first() {}

    /**
     * Returns the value at a specified position in the set.
     *
     * Note: O(n) if a value has been removed at a position < n, otherwise O(1).
     *
     * @throws OutOfRangeException
     */
    public function get(int $position) {}

    /**
     * Creates a new set using values common to both this set and another set.
     * In other words, returns a copy of this set with all values removed that
     * aren't in the other set.
     *
     * Formally: A ∩ B = {x : x ∈ A ∧ x ∈ B}
     * Bitwise:  A & B
     *
     * @param Set $set
     *
     * @return Set
     */
    public function intersection(Set $set): Set {}

    /**
     * @inheritDoc
     */
    public function isEmpty(): bool {}

    /**
     * Joins all values of the set into a string, adding an optional 'glue'
     * between them. Returns an empty string if the set is empty.
     *
     * @param string $glue
     *
     * @return string
     */
    public function join(string $glue = null): string {}

    /**
     * Returns the last value in the set.
     *
     * @return mixed the last value in the set.
     */
    public function last() {}

    /**
     * Iteratively reduces the set to a single value using a callback.
     *
     * @param callable $callback Accepts the carry and current value, and
     *                           returns an updated carry value.
     *
     * @param mixed|null $initial Optional initial carry value.
     *
     * @return mixed The carry value of the final iteration, or the initial
     *               value if the set was empty.
     */
    public function reduce(callable $callback, $initial = null) {}

    /**
     * Removes zero or more values from the set.
     *
     * @param mixed ...$values
     */
    public function remove(...$values) {}

    /**
     * Returns a reversed copy of the set.
     */
    public function reverse(): Set {}

    /**
     * Returns a subset of a given length starting at a specified offset.
     *
     * @param int $offset If the offset is non-negative, the set will start
     *                    at that offset in the set. If offset is negative,
     *                    the set will start that far from the end.
     *
     * @param int $length If a length is given and is positive, the resulting
     *                    set will have up to that many values in it.
     *                    If the requested length results in an overflow, only
     *                    values up to the end of the set will be included.
     *
     *                    If a length is given and is negative, the set
     *                    will stop that many values from the end.
     *
     *                    If a length is not provided, the resulting set
     *                    will contains all values between the offset and the
     *                    end of the set.
     *
     * @return Set
     */
    public function slice(int $offset, int $length = null): Set {}

    /**
     * Returns a sorted copy of the set, based on an optional callable
     * comparator. Natural ordering will be used if a comparator is not given.
     *
     * @param callable|null $comparator Accepts two values to be compared.
     *                                  Should return the result of a <=> b.
     *
     * @return Sequence
     */
    public function sort(callable $comparator = null) {}

    /**
     * @inheritDoc
     */
    public function toArray(): array {}

    /**
     * Creates a new set that contains the values of this set as well as the
     * values of another set.
     *
     * Formally: A ∪ B = {x: x ∈ A ∨ x ∈ B}
     * Bitwise:  A | B
     *
     * @param Set $set
     *
     * @return Set
     */
    public function union(Set $set): Set {}
}
