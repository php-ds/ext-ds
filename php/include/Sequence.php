<?php
namespace Ds;

interface Sequence extends Collection
{
    /**
     * Creates a new sequence using the values of either an array or iterable
     * object. The keys of either will not be preserved.
     *
     * @param array|\Traversable $values
     */
    function __construct($values = null);

    /**
     * Ensures that enough memory is allocated for a specified capacity. This
     * potentially reduces the number of reallocations as the size increases.
     *
     * @param int $capacity The number of values for which capacity should be
     *                      allocated. Capacity will stay the same if this value
     *                      is less than or equal to the current capacity.
     */
    function allocate(int $capacity);

    /**
     * Returns the current capacity of the sequence.
     *
     * @return int
     */
    function capacity(): int;

    /**
     * Determines whether the sequence contains all of zero or more values.
     *
     * @param mixed ...$values
     *
     * @return bool true if at least one value was provided and the sequence
     *              contains all given values, false otherwise.
     */
    function contains(...$values): bool;

    /**
     * Returns a new sequence containing only the values for which a callback
     * returns true. A boolean test will be used if a callback is not provided.
     *
     * @param callable|null $callback Accepts a value, returns a boolean result:
     *                                true : include the value,
     *                                false: skip the value.
     *
     * @return Sequence
     */
    function filter(callable $callback = null);

    /**
     * Returns the index of a given value, or false if it could not be found.
     *
     * @param mixed $value
     *
     * @return int|bool
     */
    function find($value);

    /**
     * Returns the first value in the sequence.
     *
     * @return mixed
     *
     * @throws UnderflowException if the sequence is empty.
     */
    function first();

    /**
     * Returns the value at a given index (position) in the sequence.
     *
     * @param int $index
     *
     * @return mixed
     *
     * @throws OutOfRangeException if the index is not in the range [0, size-1]
     */
    function get(int $index);

    /**
     * Inserts zero or more values at a given index.
     *
     * Each value after the index will be moved one position to the right.
     * Values may be inserted at an index equal to the size of the sequence.
     *
     * @param int   $index
     * @param mixed ...$values
     *
     * @throws OutOfRangeException if the index is not in the range [0, n]
     */
    function insert(int $index, ...$values);

    /**
     * Joins all values of the sequence into a string, adding an optional 'glue'
     * between them. Returns an empty string if the sequence is empty.
     *
     * @param string $glue
     *
     * @return string
     */
    function join(string $glue = null): string;

    /**
     * Returns the last value in the sequence.
     *
     * @return mixed
     *
     * @throws UnderflowException if the sequence is empty.
     */
    function last();

    /**
     * Returns a new sequence using the results of applying a callback to each
     * value.
     *
     * @param callable $callback
     *
     * @return Sequence
     */
    function map(callable $callback);

    /**
     * Removes the last value in the sequence, and returns it.
     *
     * @return mixed what was the last value in the sequence.
     *
     * @throws UnderflowException if the sequence is empty.
     */
    function pop();

    /**
     * Adds zero or more values to the end of the sequence.
     *
     * @param mixed ...$values
     */
    function push(...$values);

    /**
     * Adds all values in an array or iterable object to the sequence.
     *
     * @param array|\Traversable $values
     */
    function pushAll($values);

    /**
     * Iteratively reduces the sequence to a single value using a callback.
     *
     * @param callable $callback Accepts the carry and current value, and
     *                           returns an updated carry value.
     *
     * @param mixed|null $initial Optional initial carry value.
     *
     * @return mixed The carry value of the final iteration, or the initial
     *               value if the sequence was empty.
     */
    function reduce(callable $callback, $initial = null);

    /**
     * Removes and returns the value at a given index in the sequence.
     *
     * @return mixed the removed value.
     *
     * @throws OutOfRangeException if the index is not in the range [0, size-1]
     */
    function remove(int $index);

    /**
     * Returns a reversed copy of the sequence.
     */
    function reverse();

    /**
     * Rotates the sequence by a given number of rotations, which is equivalent
     * to successive calls to 'shift' and 'push' if the number of rotations is
     * positive, or 'pop' and 'unshift' if negative.
     *
     * @param int $rotations The number of rotations (can be negative).
     */
    function rotate(int $rotations);

    /**
     * Replaces the value at a given index in the sequence with a new value.
     *
     * @param int   $index
     * @param mixed $value
     *
     * @throws OutOfRangeException if the index is not in the range [0, size-1]
     */
    function set(int $index, $value);

    /**
     * Removes and returns the first value in the sequence.
     *
     * @return mixed what was the first value in the sequence.
     *
     * @throws UnderflowException if the sequence was empty.
     */
    function shift();

    /**
     * Returns a sub-sequence of a given length starting at a specified offset.
     *
     * @param int $offset If the offset is non-negative, the sequence will start
     *                    at that offset in the sequence. If offset is negative,
     *                    the sequence will start that far from the end.
     *
     * @param int $length If a length is given and is positive, the resulting
     *                    sequence will have up to that many values in it.
     *                    If the requested length results in an overflow, only
     *                    values up to the end of the sequence will be included.
     *
     *                    If a length is given and is negative, the sequence
     *                    will stop that many values from the end.
     *
     *                    If a length is not provided, the resulting sequence
     *                    will contains all values between the offset and the
     *                    end of the sequence.
     *
     * @return Sequence
     */
    function slice(int $offset, int $length = null);

    /**
     * Returns a sorted copy of the sequence, based on an optional callable
     * comparator. Natural ordering will be used if a comparator is not given.
     *
     * @param callable|null $comparator Accepts two values to be compared.
     *                                  Should return the result of a <=> b.
     *
     * @return Sequence
     */
    function sort(callable $comparator = null);

    /**
     * Adds zero or more values to the front of the sequence.
     *
     * @param mixed ...$values
     */
    function unshift(...$values);
}
