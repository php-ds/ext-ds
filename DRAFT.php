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
use RuntimeException;
use Traversable;

/**
 * @todo How do we handle comparison? Proposal: == for objects, === otherwise.
 */

/******************************************************************************/
/*                                EXCEPTIONS                                  */
/******************************************************************************/

/**
 * Should be thrown when a structure is accessed unexpectedly, or in a way that
 * is undefined, or to avoid ambiguous results. This is a marker interface to
 * support catching all access exceptions.
 */
interface AccessException {}

/**
 * Marker interface for all state-related exceptions, such as heap corruption.
 */
interface StateException {}

/**
 * Should be thrown when an empty container is accessed a clear, obvious result.
 */
class EmptyStateException implements StateException {}

/**
 * Should be thrown when an index or key is not within the given access bounds
 * of a structure, such as attempting to access beyond the length.
 */
class OffsetException extends RuntimeException implements AccessException  {}

/**
 * Should be thrown when a key is not supported, eg. when an attempt is made to
 * associate a value with NULL in Map, causing ambiguity in `find`.
 */
class KeyException extends RuntimeException implements AccessException {}


/******************************************************************************/
/*                                INTERFACES                                  */
/******************************************************************************/

/**
 * Marker interface to indicate that a class is immutable.
 */
interface Immutable {}

/**
 * Indicates that a structure can be cleared.
 */
interface Clearable
{
    function clear();
}

/**
 * Indicates that the order of the elements of a structure can be reversed.
 */
interface Reversable
{
    /**
     * @return static A reversed copy or this structure reversed in-place.
     */
    function reverse();
}

/**
 * Indicates that a structure can be sorted by value.
 *
 * This interface does not guarantee that the sorting algorithm will be stable.
 * Implementations should use a domain-specific tiebreaker when required.
 */
interface Sortable
{
    /**
     * @return static A sorted copy, or $this structure sorted in-place.
     */
    function sort(callable $comparator = null);
}

/**
 * Indicates that an object is designed to be used in hash-based structures.
 */
interface Hashable extends Immutable
{
    /**
     * @return mixed A scalar value (or hashable delegate) that will be hashed.
     */
    function getHashSource();
}

/**
 * Indicates that a structure can be accessed using a zero-based integer index
 * indicating the position of an element from the beginning of the structure.
 *
 * We extend Countable because it should always be possible to determine bounds.
 */
interface OffsetAccess extends Countable
{
    /**
     * @return mixed The value at the
     *
     * @throws OffsetException if the index is not within [0, size).
     */
    function offset(int $offset);

    /**
     * @return mixed The first value in $this structure.
     *
     * @throws EmptyStateException
     */
    function first();

    /**
     * @return mixed The last value in $this structure.
     *
     * @throws EmptyStateException
     */
    function last();
}

/**
 * Indicates that a structure contains elements and is aware of the number of
 * items contained at any point in time. How a container must determine the
 * count is implementation-specific.
 *
 * This interface does not imply that a structure is traversable.
 */
interface Container extends Countable
{
    /**
     * @return bool whether the container is empty, which can be TRUE even when
     *              the number of elements is non-zero.
     */
    function isEmpty(): bool;
}

/**
 * Indicates that a structure contains a series of contiguous elements.
 */
interface Sequence extends OffsetAccess
{
    /**
     * Returns the value at the given offset.
     *
     * @throws OffsetException if the index is not within [0, size)
     */
    function get(int $offset);
}

/**
 * A sequence which can be modified, either in-place or as a copy.
 */
interface MutableSequence extends Sequence
{
    /**
     * Sets the value at the given offset.
     *
     * @throws OffsetException if the index is not within [0, size)
     */
    function set(int $offset, $value);

    /**
     * Removes the value at the given offset, moving all successive
     * values one position towards the front.
     *
     * @throws OffsetException if the index is not within [0, size)
     */
    function unset(int $offset);

    /**
     * Moves all values between the given index and the end of the sequence
     * towards the back, then inserts the given values into the gap.
     *
     * @throws OffsetException if the index is out of range [0, size]
     */
    function insert(int $offset, ...$values);
}

/**
 * Indicates that a structure is designed to quickly determine whether a given
 * value is already contained by it.
 *
 * Note: The intention will be that all implementations of Set will use a
 *       do_operation handler to override |, ^, -, &.
 */
interface Set
{
    /**
     * @return bool TRUE if the value is in $this set, FALSE otherwise.
     */
    function has($value): bool;

    /**
     * Operator: |
     *
     * @todo what about the + operator?
     *
     * @return static A set containing values in both $this and $other.
     *
     * @todo Naming - or, union, merge
     */
    function or(Set $other): Set;

    /**
     * Operator: ^
     *
     * @return static A set containing values in either $this or $other,
     *                but not in both.
     */
    function xor(Set $other): Set;

    /**
     * Operator: -
     *
     * @return static A set containing values in $this but not in $other.
     *
     * @todo Naming - not, diff, without
     */
    function not(Set $other): Set;

    /**
     * Operator: &
     *
     * @return static A set containing values in $this that are also in $other.
     *
     * @todo Naming - and, intersect
     */
    function and(Set $other): Set;
}

/**
 * A set which can be modified, either in-place or as a copy.
 */
interface MutableSet extends Set
{
    /**
     * Adds the given value to $this set if it is not already in $this set.
     */
    function add(...$values);

    /**
     * Removes a given value from $this set, or does nothing if that value could
     * not be found. The caller can use `has` to determine membership before
     * removal. This method therefore promises only that the given value is not
     * a member of the set on return.
     */
    function remove($value);
}

/**
 * Indicates that a structure is a set that is always sorted.
 */
interface SortedSet extends Set {}

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
     * @return mixed The value associated with the $key, or $default if the key
     *               could not be found and a $default was provided.
     *
     * @throws AccessException if the $key was not found with default given.
     */
    function get($key, $default = null);

    /**
     * @return bool TRUE if $this map has an association for the given $key,
     *              FALSE otherwise.
     *
     * @todo if we don't extend Set, should we remove this in favour of
     *       $map->keys()->has()? This would only work if keys() is O(1), which
     *       would not always be the case.
     */
    function has($key): bool;

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
     * @throws KeyException if the map implementation does not support
     *                             the given key, eg. NULL
     */
    function set($key, $value);

    /**
     * Removes the given $key from the map, and does nothing if they key could
     * not be found. This emulates `unset`
     */
    function unset($key);
}

/**
 * Indicates that a structure can receive and produce values efficiently in a
 * semantically defined order. Both operations should be O(1).
 */
interface Transferable
{
    /**
     * Offers one or more values to the transfer.
     */
    function send(...$values);

    /**
     * Removes and returns the next value.
     *
     * @throws EmptyStateException
     */
    function poll();
}

/******************************************************************************/
/*                                 CLASSES                                    */
/******************************************************************************/

/**
 * A transfer adapter for stacks.
 */
final class Stack implements
    Container,
    Transferable
    {
        /* Transferable */
        // send (push)
        // poll (pop)

        /**
         * Adds a value to the top of the stack.
         */
        function push(...$values);

        /**
         * Removes and returns the value at the top of the stack.
         *
         * @throws EmptyStateException
         */
        function pop();
    }

/**
 * A transfer adapter for queues.
 */
final class Queue implements
    Container,
    Transferable
    {
        /* Transferable */
        // send (push)
        // poll (shift)

        /**
         * Adds a value to the queue.
         */
        function push(...$values);

        /**
         * Removes and returns the next value in the queue.
         */
        function shift();
    }

/**
 * A fixed-size immutable sequence.
 */
final class Tuple implements
    Traversable,
    Container,
    Sequence,
    Hashable
    {
        /* Countable | Container */
        // count
        // isEmpty

        /* OffsetAccess | Sequence */
        // offset
        // first
        // last
        // get

        /* Hashable | Immutable */
        // getHashSource
    }

/**
 * List structure, dynamic size, contiguous, no ops at the front.
 */
final class Vector implements
    ArrayAccess,
    Traversable,
    Container,
    Clearable,
    Sortable,
    Reversable,
    MutableSequence,
    Stack
    {
        /* ArrayAccess */
        // offsetGet
        // offsetSet
        // offsetUnset
        // offsetExists

        /* Countable | Container */
        // count
        // isEmpty

        /* Clearable */
        // clear

        /* Sortable */
        // sort

        /* Reversable */
        // reverse

        /* OffsetAccess | Sequence | MutableSequence */
        // offset
        // first
        // last
        //
        // get
        // set
        // unset
        // insert

        /**
         * Adds one or more values to the end of the vector.
         */
        function push(...$values);

        /**
         * Removes and returns the value at the end of the vector.
         *
         * @throws EmptyStateException
         */
        function pop();
    }

/**
 * Double-ended-queue, supports prepend and append, but nothing in-between.
 */
final class Deque implements
    Container,
    Clearable,
    Transferable,
    Sequence
    {
        /* Countable | Container */
        // count
        // isEmpty

        /* Clearable */
        // clear

        /* Transferable */
        // send (push)
        // poll (shift)

        /* OffsetAccess | Sequence */
        // offset
        // first
        // last
        // get

        /**
         * Adds one or more values to the end of the deque.
         */
        function push(...$values);

        /**
         * Removes and returns the value at the end of the deque.
         *
         * @throws EmptyStateException
         */
        function pop();

        /**
         * Adds one or more values to the start of the deque.
         */
        public function unshift(...$values);

        /**
         * Removes and returns the value at the start of the deque.
         *
         * @throws EmptyStateException
         */
        function shift();
    }

/**
 * The standard hashtable, implemented as Map in ext-ds currently. It is based
 * on the PHP array but is not identical. We can implement OffsetAccess because
 * insertion order is preserved.
 */
final class HashMap implements
    ArrayAccess,
    Traversable,
    Container,
    Clearable,
    Sortable,
    Reversable,
    OffsetAccess,
    MutableMap
    {
        /* ArrayAccess */
        // offsetGet
        // offsetSet
        // offsetUnset
        // offsetExists

        /* Countable | Container */
        // count
        // isEmpty

        /* Clearable */
        // clear

        /* Sortable */
        // sort

        /* Reversable */
        // reverse

        /* OffsetAccess */
        // offset
        // first
        // last

        /* Map | MutableMap */
        // get
        // has
        // keys
        // values
        //
        // set
        // unset
    }

/**
 * The Set equivalent of HashMap, which might not use a HashMap internally, but
 * will always preserve insertion order.
 */
final class HashSet implements
    ArrayAccess,
    Traversable,
    Container,
    Clearable,
    Sortable,
    Reversable,
    Transferable,
    OffsetAccess,
    MutableSet
    {
        /* ArrayAccess */
        // offsetGet
        // offsetSet
        // offsetUnset
        // offsetExists

        /* Container | Countable */
        // count
        // isEmpty

        /* Clearable */
        // clear

        /* Sortable */
        // sort

        /* Reversable */
        // reverse

        /* Transferable */
        // send (add)
        // poll (remove[0])

        /* OffsetAccess */
        // offset
        // first
        // last

        /* Set | MutableSet */
        // has
        // or
        // xor
        // not
        // and
        //
        // add
        // remove
    }

/**
 * A structure that is always sorted and does not allow duplicate elements.
 *
 * This would be useful for building sorted datasets, or sets without hashing.
 *
 * See: https://github.com/php-ds/ext-ds/issues/121
 */
final class BinarySearchTree implements
    Traversable,
    Container,
    Clearable,
    MutableSet,
    SortedSet
    {
        /* Container | Countable */
        // count
        // isEmpty

        /* Clearable */
        // clear

        /* Set | MutableSet */
        // has
        // or
        // xor
        // not
        // and
        //
        // add
        // remove

        /**
         * Creates a new bst using values from $iter.
         */
        public function __construct(callable $comparator = null) {}

        /**
         * Root, Left, Right
         */
        public function preorder(): iterable {}

        /**
         * Left, Root, Right
         */
        public function inorder(): iterable {}

        /**
         * Left, Right, Root
         */
        public function postorder(): iterable {}

        /**
         * Right, Root, Left
         */
        public function outorder(): iterable {}
    }

/**
 * Stable heap with an optional comparator, defaulting to minimum. We could
 * potentially remove the nullable which clears up the min/max ambiguity.
 */
final class Heap implements
    Container,
    Clearable,
    Transferable
    {
        /* Container | Countable */
        // count
        // isEmpty

        /* Clearable */
        // clear

        /* Transferable */
        // send (push)
        // poll (shift)

        /**
         * Creates a new heap using an optional comparator.
         */
        public function __construct(callable $comparator = null) {}

        /**
         * Adds a value to the heap.
         */
        public function push(...$values) {}

        /**
         * Removes and returns the value at the top of the heap.
         *
         * @throws EmptyStateException
         */
        public function shift() {}
    }
