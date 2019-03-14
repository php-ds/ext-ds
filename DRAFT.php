<?php
/*
|--------------------------------------------------------------------------
| PHP Data Structures - API Draft - v2.0
|
| Target PHP Version: 7.3.1
|
| https://pecl.php.net/package/ds
|
|--------------------------------------------------------------------------
|
| This file is will be used for planning the API of the next major version of
| the data structures extension. Please feel free to suggest modifications.
*/
namespace Ds;

use Countable;
use LogicException;
use Traversable;

/******************************************************************************/
/*                                EXCEPTIONS                                  */
/******************************************************************************/

/**
 * A marker interface that is implemented by all exceptions within this library. 
 */
interface Exception implements Throwable {}

/**
 * Should be thrown when an empty container is polled in an empty state.
 */
class EmptyContainerException extends RuntimeException implements Exception {}

/**
 * Should be thrown when a key is not supported.
 */
class InvalidKeyException extends LogicException implements Exception {}

/**
 * Should be thrown when an index or key is not within the given access bounds
 * of a structure, such as attempting to access beyond the length.
 */
class InvalidOffsetException extends LogicException implements Exception {}

/******************************************************************************/
/*                                INTERFACES                                  */
/******************************************************************************/

/**
 * Marker interface to indicate that a class is immutable.
 */
interface Immutable 
{
}

/**
 * Indicates that a structure can be cleared.
 */
interface Clearable
{
    function clear(): void;
}

/**
 * Indicates that a structure can be sorted.
 */
interface Sortable
{
    /**
     * Sorts the keys and/or values of $this structure in place.
     *
     * @param (callable(mixed $a, mixed $b): int)|null $comparator The comparator used to sort this this sortable.
     */
    function sort(callable $comparator = null): void;
}

/**
 * Indicates that an object is designed to be equated using `==`.
 * The operator will be overloaded for all classes that implement Equatable.
 */
interface Equatable 
{
    /**
     * @param static $other The other structure to compare with.
     *
     * @return bool Whether this structure equates to the given one.
     */
    function equals($other): bool;
}

/**
 * Indicates that an object is designed to be used in hash-based structures.
 */
interface Hashable extends Equatable, Immutable
{
    /**
     * @return mixed A scalar value to be used when generating a hashcode.
     */
    function getHashSource(): string;
}

/**
 * Indicates that a structure contains elements and is aware of the number of
 * items contained at any point in time.
 */
interface Container extends \Countable
{
    /**
     * @return int The number of items in this container.
     */
    function count(): int;

    /**
     * @return bool Whether this container is empty.
     */
    function isEmpty(): bool;
}

/**
 * Indicates that a structure can be accessed using a zero-based integer index
 * indicating the position of an element from the beginning of the structure.
 */
interface Sequence extends Container
{
    /**
     * @param mixed $value The value to look for.
     *
     * @return mixed The value at the given offset.
     *
     * @throws InvalidOffsetException if the $offset is not within [0, size).
     */
    function get(int $offset);
    
    /**
     * @param mixed $value The value to search for.
     *
     * @return int The offset of the given value, or -1 if the value could not be found.
     */
    function indexOf($value): int;

    /**
     * @param mixed $value The value to search for.
     *
     * @return bool TRUE if this sequence contains the given value, FALSE otherwise.
     */
    function contains($value): bool;

    /**
     * @return mixed The first value in $this structure.
     *
     * @throws EmptyContainerException
     */
    function first();

    /**
     * @return mixed The last value in $this structure.
     *
     * @throws EmptyContainerException
     */
    function last();
}

/**
 * A sequence which can be modified, either in-place or as a copy.
 */
interface MutableSequence extends Sequence
{
    /**
     * Sets the value at the given offset.
     *
     * @param int   $offset The offset at which the value should be set.
     * @param mixed $value  The value to set.
     *
     * @throws InvalidOffsetException if the index is not within [0, size)
     */
    function set(int $offset, $value): void;

    /**
     * Removes the value at the given offset, moving all successive
     * values one position towards the front.
     *
     * @param int The offset to be removed.
     *
     * @throws InvalidOffsetException if the index is not within [0, size)
     */
    function unset(int $offset): void;

    /**
     * Moves all values between the given index and the end of the sequence
     * towards the back, then inserts the given values into the gap.
     *
     * @param int   $offset    The offset to insert values at.
     * @param mixed ...$values The value to insert at the specified offset.
     *
     * @throws InvalidOffsetException if the index is out of range [0, size]
     */
    function insert(int $offset, ...$values): void;
}

/**
 * Indicates that a structure is a sequence that is always sorted.
 */
interface SortedSequence extends Sequence 
{
}

/**
 * Indicates that a structure is designed to quickly determine whether a given
 * value is already contained by it.
 * 
 * 
 */
interface Set
{
    /**
     * @param mixed $value The value to search for.
     *
     * @return bool TRUE if this set contains the given value, FALSE otherwise.
     */
    function contains($value): bool;

    /**
     * @param self $other The other set to union with.
     *
     * @return static A set containing values in both $this and $other.
     */
    function or(Set $other): Set;

    /**
     * @param self $other The other set to xor with.
     *
     * @return static A set containing values in either $this or $other,
     *                but not in both.
     */
    function xor(Set $other): Set;

    /**
     * @param self $other The other set to diff against.
     *
     * @return static A set containing values in $this but not in $other.
     */
    function not(Set $other): Set;

    /**
     * @param self $other The other set to intersect with.
     *
     * @return static A set containing values in $this that are also in $other.
     */
    function and(Set $other): Set;
}

/**
 * A set which can be modified, either in-place or as a copy.
 */
interface MutableSet extends Set
{
    /**
     * @param mixed ...$values The values to be added.
     *
     * Adds the given value to $this set if it is not already in $this set.
     */
    function add(...$values): void;

    /**
     * @param mixed $value The value to be removed.
     *
     * Removes a given value from $this set, or does nothing if that value could
     * not be found. The caller can use `has` to determine membership before
     * removal. This method therefore promises only that the given value is not
     * a member of the set on return.
     */
    function remove($value): void;
}

/**
 * Indicates that a structure is a set that is always sorted.
 */
interface SortedSet extends Set 
{
}

/**
 * A structure that associates one value with another and provides the ability
 * to query or adjust associations efficiently.
 *
 * @todo Should Map extend Set (not mutable)?
 *       What about the set operation methods then?
 *
 * @todo Should Map require some kind of merge, diff, and intersection support?
 */
interface Map
{
    /**
     * @todo if NULL keys are not allowed, should we throw if $key is NULL?
     *
     * @param mixed $key     The key at which the value should be looked for.
     * @param mixed $default The fallback value of no value exists at specified key.
     *
     * @return mixed The value associated with the $key, or $default if the key
     *               could not be found and a $default was provided.
     *
     * @throws AccessException if the $key was not found with default given.
     */
    function get($key, $default = null);

    /**
     * @return Set All keys from the map added in traversal order.
     *
     * @todo Should this return an iterator?
     */
    function keys(): Set;

    /**
     * @return Set All keys from the map added in traversal order.
     *
     * @todo Should this return an iterator?
     */
    function values(): Sequence;
}

/**
 * A map which can be modified, either in-place or as a copy.
 */
interface MutableMap extends Map
{
    /**
     * Associates the $key with the $value, overriding any previous association.
     *
     * @param mixed $key   The key to set the value for.
     * @param mixed $value The value to set.
     *
     * @throws InvalidKeyException if the map implementation does not support
     *                             the given key, eg. NULL
     */
    function set($key, $value): void;

    /**
     * Removes the given $key from the map, and does nothing if they key could
     * not be found. This emulates `unset`
     *
     * @param mixed $key The key at which the value should be unset.
     */
    function unset($key): void;
}

/**
 * Indicates that a structure is a map that is always sorted.
 */
interface SortedMap extends Map 
{
}

/**
 * Indicates that a structure can receive and produce values efficiently in a
 * semantically defined order. Both operations should be O(1).
 */
interface Transferable
{
    /**
     * Offers one or more values to this transferable.
     *
     * @param mixed ...$values The values that should be transferred.
     */
    function send(...$values): void;

    /**
     * Removes and returns the next value produced by this transferable.
     *
     * @return mixed The transferred value.
     *
     * @throws EmptyContainerException
     */
    function poll();
}


/******************************************************************************/
/*                                 CLASSES                                    */
/******************************************************************************/

/**
 *
 */
final class Allocation implements
    ArrayAccess,
    Equatable,  /* Same type, capacity and values at each offset. */
    Clearable,
    Sortable
    {
        public const TYPE_BOOL      = 0; // undefined => false
        public const TYPE_INTEGER   = 1; // undefined => 0
        public const TYPE_FLOAT     = 2; // undefined => 0.0
        public const TYPE_STRING    = 3; // undefined => NULL
        public const TYPE_ARRAY     = 4; // undefined => NULL
        public const TYPE_OBJECT    = 5; // undefined => NULL

        /**
         * Allow buffers to be truncated to 0.
         */
        public const MIN_CAPACITY = 0;

        /**
         * We must be able to accept and return capacity as integer, so we can
         * only support up to max int values. This is absolutely massive though
         * so should be incredibly rare to approach.
         *
         * @todo Should we consider when USE_ZEND_ALLOC might be 0? Is this a
         *       potential security or buffer overflow issue? I'm not sure...
         */
        public const MAX_CAPACITY = PHP_INT_MAX;

        /**
         * @param int $capacity Initial capacity of the structure.
         * @param int $type     The type for which the allocation is being made (one of Allocation::Type_* constants).
         *
         * @throws UnexpectedValueException if the type is not a valid constant.
         */
        public function __construct(int $capacity, int $type) {}

        /**
         * @return int The type of the values in this buffer.
         */
        public function type(): int {}

        /**
         * @return int The current capacity of this buffer.
         */
        public function capacity(): int {}

        /**
         * Re-allocates this buffer to a new capacity, which may truncate the
         * current buffer. It is up to user to manage this case.
         *
         * @param int $capacity The new capacity to which this buffer should be resized.
         *
         * @throws RangeException if the capacity is not within the valid range:
         *                        MIN_CAPACITY <= $capacity <= MAX_CAPACITY
         */
        public function realloc(int $capacity): void {}

        /**
         * Returns the value at the given offset as a standard value of the type
         * that this buffer stores. Type validation is done on write. If nothing
         * has been stored at the given offset, this method will return an
         * appropriate "null" value for the type (see notes on constants).
         *
         * @param mixed $offset The offset at which to look for a value.
         *
         * @return mixed The value at the specified offset.
         *
         * @throws InvalidOffsetException if the offset is not within [0, capacity)
         */
        public function offsetGet($offset) {}

        /**
         * Sets the value at the given offset after validating its value type.
         *
         * Note: NULL is not supported. Use `unset` to clear a value by offset.
         *
         * @param mixed $offset The offset at which to set the new value.
         * @param mixed $value  The value to set at the specified offset.
         *
         * @throws InvalidOffsetException if the offset is not within [0, capacity)
         */
        public function offsetSet($offset, $value): void {}

        /**
         * Sets the value at the given offset to NULL.
         *
         * @param mixed $offset The offset at which to unset a value.
         *
         * @throws InvalidOffsetException if the offset is not within [0, capacity),
         *                         unless called as part of a silent `unset`.
         */
        public function offsetUnset($offset): void {}

        /**
         * Returns whether there is a non-NULL value at the given offset. This
         * method returns FALSE if the offset is not within [0, capacity).
         *
         * @param mixed $offset The offset at which to look for a value.
         *
         * @return bool Whether the specified offset contains a value or not.
         */
        public function offsetExists($offset): bool {}
    }

/**
 * A fixed-size immutable sequence.
 */
final class Tuple implements
    Traversable,
    Equatable,
    Sequence,  /* Container, \Countable */
    Hashable   /* Equatable, Immutable */
    {
        /**
         * @param mixed[] $source The values used to fill this tuple.
         */
        public function __construct(iterable $source) {}
    }

/**
 * List structure, dynamic size, contiguous, no ops at the front.
 */
final class Vector implements
    ArrayAccess,
    Traversable,
    Equatable,      /* Same values in the same order. */
    Clearable,
    Container,
    Sortable,
    MutableSequence /* Sequence, Container, \Countable */
    {
        /**
         * Adds one or more values to the end of the vector.
         *
         * @param mixed ...$values The values to append to the vector.
         */
        function push(...$values): void;

        /**
         * Removes and returns the value at the end of the vector.
         *
         * @return mixed The value at the end of the vector.
         *
         * @throws EmptyContainerException
         */
        function pop();
    }

/**
 * Double-ended-queue, supports prepend and append, but nothing in-between.
 */
final class Deque implements
    Equatable,        /* Same values in the same order. */
    Clearable,
    Container,
    Sortable,
    MutableSequence,  /* Sequence, Container, \Countable */
    Transferable
    {
        /**
         * Adds one or more values to the end of the deque.
         *
         * @param mixed ...$values The values to push at the end of this deque.
         */
        function push(...$values): void;

        /**
         * Removes and returns the value at the end of the deque.
         *
         * @return mixed The value at the end of this deque.
         *
         * @throws EmptyContainerException
         */
        function pop();

        /**
         * Adds one or more values to the start of the deque.
         *
         * @param mixed ...$values The values to insert to the start of this deque.
         */
        public function unshift(...$values): void;

        /**
         * Removes and returns the value at the start of the deque.
         *
         * @return mixed The value at the beginning of this deque.
         *
         * @throws EmptyContainerException
         */
        function shift();
    }

/**
 * An immutable sequence that is evaluated and sorted on first access.
 */
final class SetSequence implements
    ArrayAccess,
    Traversable,
    Equatable,      /* Same values in the same order. */
    Container,
    Immutable,
    SortedSet,      /* Set */
    SortedSequence, /* Sequence, Container, \Countable */
    {
        /**
         * @param mixed[] $source The values used to fill this sequence.
         */
        public function __construct(iterable $source) {}
    }

/**
 * The Set equivalent of HashMap, which might not use a HashMap internally, but
 * will always preserve insertion order.
 */
final class HashSet implements
    ArrayAccess,
    Traversable,
    Equatable,    /* Same values in the same order. */
    Clearable,
    Container,
    Transferable,
    Sortable,
    Sequence,     /* Container, \Countable */
    MutableSet    /* Set */
    {}

/**
 * A structure that is always sorted and does not allow duplicate elements.
 *
 * This would be useful for building sorted datasets, or sets without hashing.
 *
 * See: https://github.com/php-ds/ext-ds/issues/121
 */
final class TreeSet implements
    Traversable,
    Equatable,  /* Same values in the same order. */
    Clearable,
    Container,
    MutableSet, /* Set */
    SortedSet   /* Set */
    {
        /**
         * Creates a new tree set using an optional comparator.
         *
         * @param (callable(mixed $a, mixed $b): int)|null $comparator The comparator used to sort this tree set.
         */
        public function __construct(callable $comparator = null) {}
    }

 /**
 * A set that allows duplicate values, and keeps a count of the number of times
 * a value appears in the set. This is a set because we can determine whether
 * it contains a value _at all_, and it is also a sequence because the traversal
 * can yield a value more than once.
 */
final class MultiSet implements
    ArrayAccess,
    Traversable,
    Equatable,    /* Same values and frequencies. */
    Transferable,
    Container,
    Clearable,
    MutableSet    /* Set */
    {
        /**
         * Creates a new multiset using values from $iter.
         *
         * @param (callable(mixed $a, mixed $b): int)|null $comparator The comparator used to sort this multi set.
         */
        public function __construct(callable $comparator = null) {}

        /**
         * Adjusts the count of a value by a given count. If the resulting count
         * becomes <= 0, the value will be removed. If a value is not already in
         * the set, it will be added before the count is adjusted.
         *
         *   > 0: Add
         *   < 0: Remove
         *
         * @param mixed $value The value to be adjusted.
         * @param int   $count The amount by which the value should be adjusted.
         *
         * @return int The resulting frequency after the adjustment.
         */
        public function adjust($value, int $count = 1): int {}

        /**
         * @param mixed $value The value of which the frequency is to be inspected.
         *
         * @return int The number of instances of a given value that $this set
         *             contains, which could be 0 if not in the set at all.
         */
        public function freq($value): int {}

        /**
         * Returns an iterator of the values with the highest frequencies, where
         * the key is the element and the value is the frequency.
         *
         * @return iterable<mixed, int> The values as keys and their frequency/multiplicity as values.
         */
        public function rank(): iterable {}

        /**
         * Creates an iterable where the key is the element in the set and the
         * value is the frequency / multiplicity.
         *
         * @return iterable<mixed, int> The values as keys and their frequency/multiplicity as values.
         */
        public function enumerate(): iterable {}
    }

/**
 * This structure is based on the same internal structure of the PHP array,
 * but is not identical. We can use hashable objects as keys, and insertion
 * order is still preserved.
 */
final class HashMap implements
    ArrayAccess,
    Traversable,
    Equatable,  /* Same key => value associations, ordering not considered. */
    Container,
    Clearable,
    Sortable,
    MutableMap  /* Map */
    {}

/**
 *
 */
final class TreeMap implements
    ArrayAccess,
    Traversable,
    Equatable,  /* Same key => value associations. */
    Container,
    Clearable,
    MutableMap, /* Map */
    SortedMap   /* Map */
    {}

/**
 * A basic first-in-last-out structure.
 */
final class Stack implements
    Container,
    Transferable
    {
        /**
         * Adds a value to the top of the stack.
         *
         * @param mixed ...$values The values to be pushed onto the stack.
         */
        function push(...$values): void;

        /**
         * Removes and returns the value at the top of the stack.
         *
         * @return mixed The value at the top.
         *
         * @throws EmptyContainerException
         */
        function pop();
    }

/**
 * A basic first-in-first-out structure.
 */
final class Queue implements
    Container,
    Transferable
    {
        /**
         * Adds a value to the queue.
         *
         * @param mixed ...$values The values to be pushed into the queue.
         */
        function push(...$values): void;

        /**
         * Removes and returns the next value in the queue.
         *
         * @return mixed The next value.
         */
        function shift();
    }

/**
 * Stable heap with an optional comparator, defaulting to minimum.
 */
final class Heap implements
    Container,
    Clearable,
    Transferable
    {
        /**
         * Creates a new heap using an optional comparator.
         *
         * @param (callable(mixed $a, mixed $b): int)|null $comparator The comparator used to sort this heap.
         */
        public function __construct(callable $comparator = null) {}

        /**
         * Adds a value to the heap.
         *
         * @param mixed ...$values The values to be pushed onto the heap.
         */
        public function push(...$values): void {}

        /**
         * Removes and returns the value at the top of the heap.
         *
         * @return mixed The value at the top.
         *
         * @throws EmptyContainerException
         */
        public function shift() {}
    }

/**
 * A queue that yields values in order of priority, from high to low.
 */
final class PriorityQueue implements
    Container,
    Clearable,
    Transferable
    {
        /**
         * Creates a new priority queue using an optional comparator.
         *
         * @param (callable(mixed $a, mixed $b): int)|null $comparator The comparator used to sort this heap.
         */
        public function __construct(callable $comparator = null) {}

        /**
         * Adjusts the priority of a given value, setting it to the return value
         * of the given mutator, then ensures that heap invariants are resolved.
         *
         * @param mixed           $value   The value of which the priority should be adjusted.
         * @param callable(): int $mutator The mutator returning the new priority.
         */
        public function adjust($value, callable $mutator): void {}

        /**
         * Adds a value to the priority queue, using a given initial priority.
         *
         * @param mixed $value    The value to be pushed into the queue.
         * @param int   $priority The priority of the inserted value.
         */
        public function push($value, int $priority): void {}

        /**
         * Removes and returns the value at the front of the priority queue.
         *
         * @return mixed The next value.
         *
         * @throws EmptyContainerException
         */
        public function shift() {}
    }
