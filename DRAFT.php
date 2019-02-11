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
 * Should be thrown when an index or key is not within the given access bounds
 * of a structure, such as a negative index on a list or
 */
class IndexOutOfBoundsException extends \OutOfBoundsException implements AccessException  {}

/**
 * Should be thrown when an empty container is accessed a clear, obvious result.
 */
class EmptyContainerException extends \LogicException implements AccessException {}

/**
 * Should be thrown when an undefined zval is accessed. I do not except any user
 * classes to throw this, but it might be useful to catch it.
 */
class UndefinedValueException extends \LogicException implements AccessException {}

/**
 * Should be thrown when a key is not supported, eg. when an attempt is made to
 * associate a value with NULL in Map, causing ambiguity in `find`.
 */
class InvalidKeyException extends \LogicException implements AccessException {}


/******************************************************************************/
/*                                 ITERATORS                                  */
/******************************************************************************/

/**
 * Yields each key and value from $iter, after applying $mapper to the value.
 */
function map(iterable $iter, callable $mapper): \Iterator
{
    foreach ($iter as $key => $val) {
        yield $key => $mapper($val, $key, $iter);
    }
}

/**
 * Yields each key and value from $iter for which $predicate returns TRUE.
 */
function filter(iterable $iter, callable $predicate): \Iterator
{
    foreach ($iter as $key => $val) {
        if ($predicate($val, $key, $iter)) {
            yield $key => $val;
        }
    }
}

/**
 * Yields each key and value from $iter for which $predicate returns FALSE.
 */
function reject(iterable $iter, callable $predicate): \Iterator
{
    foreach ($iter as $key => $val) {
        if (!$predicate($val, $key, $iter)) {
            yield $key => $val;
        }
    }
}

/**
 * @return mixed|null The key of the first iteration for which $predicate
 *                    returns TRUE, or NULL if none were found.
 */
function find(iterable $iter, callable $predicate)
{
    foreach (filter($iter, $predicate) as $key => $val) {
        return $key;
    }

    return null;
}

/**
 * Reduces the keys and values of $iter to a single result, carrying forward
 * intermediate results until the final iteration returns the final result.
 */
function reduce(iterable $iter, callable $reducer, $initial = null)
{
    $result = $initial;

    foreach ($iter as $key => $val) {
        $result = $reducer($result, $value, $key, $iter);
    }

    return $result;
}

/**
 * @return bool TRUE if the $predicate returns TRUE for any iteration of $iter.
 *
 */
function any(iterable $iter, callable $predicate = null): bool
{
    foreach (filter($iter) as $key => $val) {
        return true;
    }

    return false;
}

/**
 * @return bool TRUE if the $predicate returns TRUE for all iterations of $iter,
 *                   or if $iter did not produce any iterations at all (empty).
 */
function all(iterable $iter, callable $predicate = null): bool
{
    foreach (reject($iter) as $key => $val) {
        return false;
    }

    return true;
}

/**
 * @todo I am not sure how viable this is, but it could be useful to find the
 *       kth smallest value without having to sort the entire container. We must
 *       be able to access the structure by offset in order to implement partial
 *       algorithms like quick-select.
 *
 * See: https://en.wikipedia.org/wiki/Partial_sorting
 */
function sort(OffsetAccess $iter, callable $comparator = null): \Iterator
{
    // Partial sort...
}


/******************************************************************************/
/*                                INTERFACES                                  */
/******************************************************************************/

/**
 * Indicates that a structure contains elements and is aware of the number of
 * items contained at any point in time. How a container must determine the
 * count is implementation-specific.
 *
 * This interface does not imply that a structure is traversable.
 */
interface Container extends \Countable
{
    /**
     * @return bool whether the container is empty, which can be TRUE even when
     *              the number of elements is non-zero.
     */
    function isEmpty(): bool;
}

/**
 * An interface which mirrors some of the iterator functions.
 */
interface Collection extends \Traversable
{
    /**
     * @return static
     */
    function map(callable $mapper): Collection;

    /**
     * @return static
     */
    function filter(callable $predicate): Collection;

    /**
     * @return static
     */
    function reject(callable $predicate): Collection;

    /**
     * @return mixed|null
     */
    function find(callable $predicate);

    /**
     * @return mixed
     */
    function reduce(callable $reducer, $initial = null);

    /**
     * @return bool
     */
    function any(callable $predicate = null): bool;

    /**
     * @return bool
     */
    function all(callable $predicate = null): bool;
}

/**
 * Indicates that the elements of a structure can be represented by an array
 * without altering the structure of the data, without any warnings or errors.
 */
interface Arrayable
{
    /**
     * This method should not fail. There is an implicit expectation that any
     * structure converted to an array must be able to be converted back to
     * the structure in such a way that it is considered equal to the initial.
     */
    function toArray(): array;
}

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
 * Indicates that the elements of a structure can be sorted.
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
interface Hashable
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
 *
 * @todo Naming - OrdinalAccess, LinearAccess, RandomAccess...
 */
interface OffsetAccess extends \Countable
{
    /**
     * @return mixed The value at the
     *
     * @throws IndexOutOfBoundsException if the index is not within [0, size).
     *
     * @todo Potential for a better name, like "skip" or "atIndex" or "at"?
     */
    function offset(int $index);
}

/**
 * Indicates that a structure contains a series of contiguous elements.
 */
interface ReadOnlySequence extends OffsetAccess
{
    /**
     * @return mixed The first value in $this sequence.
     */
    function first();

    /**
     * @return mixed The last value in $this sequence.
     */
    function last();

    /**
     * @return static A partial sequence of $this.
     */
    function slice(int $offset, int $length): ReadOnlySequence;
}

/**
 * A sequence which can be modified, either in-place or as a copy.
 */
interface Sequence extends ReadOnlySequence
{
    /**
     * Appends the given value to the end of the structure.
     */
    function push($value);

    /**
     * Removes and returns the value at the end of the structure.
     *
     * @return mixed The value that was removed from the end of the structure.
     *
     * @throws EmptyContainerException if the structure is empty.
     *
     * @todo should we leave it up to the impl to throw or return NULL? I think
     *       we should aim for consistency by requiring a throw when empty. This
     *       should be consistent with OffsetAccess' first and last.
     */
    function pop();
    /**
     * Sets the value at the given position.
     *
     * @throws IndexOutOfBoundsException if the index is not within [0, size)
     */
    function set(int $index, $value);

    /**
     * Removes the value at the given position, moving all successive
     * values one position towards the front.
     *
     * @todo Naming - remove, pull?
     *
     * @throws IndexOutOfBoundsException if the index is not within [0, size)
     */
    function unset(int $index);

    /**
     * Moves all values between the given index and the end of the sequence one
     * position towards the back, then inserts the given value into the gap.
     *
     * @todo "How do I insert the values of one sequence into another?"
     *       I think we should consider variadic functions across the board,
     *       so that we can do things like `$seq->insert(5, ...$other)` instead
     *       of userland forloops passing a single argument every time.
     *
     * @todo We should also consider constructor syntax, like `new Vector(...?)`
     *
     * @todo Should $value be ...$values? (This applies to all push, unshift etc)
     *
     * @throws IndexOutOfBoundsException if the index is out of range [0, size]
     */
    function insert(int $index, $value);
}

/**
 * Indicates that a structure is designed to operate efficiently at both the
 * front and the back of a linear dataset.
 */
interface Deque extends Sequence
{
    /**
     * Prepends the given value to the front of the structure.
     */
    function unshift($value);

    /**
     * Removes and returns the first value of the structure.
     *
     * @return mixed The value that was removed from the front of the structure.
     *
     * @throws EmptyContainerException if the structure is empty.
     */
    function shift();
}

/**
 * Indicates that a structure is designed to quickly determine whether a given
 * value is already contained by it.
 *
 * Note: The intention will be that all implementations of Set will use a
 *       do_operation handler to override |, ^, -, &.
 *
 * @todo I'm not convinced that we need the four set operation methods, but we
 *       could assume that most applications that use Set will want to remove
 *       values from another set, or determine an intersection at some stage.
 *       I would rather implement them here, and maybe with operator support,
 *       and make use of the advantages we have internally.
 *
 *       We could also implement a set operation interface, which provides the
 *       four methods and operator overloading, but that would mean that not all
 *       implementations of Set support set operations.
 *
 *       What does this mean for Map? Should we not support the same use cases
 *       as array_key_intersect? If Set has these operations, should Map have
 *       methods like `diff`, `intersect`, etc to be consistent with arrays?
 *       If so, should the set operation methods share the same naming?
 */
interface ReadOnlySet
{
    /**
     * @return bool TRUE if the value is in $this set, FALSE otherwise.
     *
     * @todo Naming - contains?
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
interface Set extends ReadOnlySet
{
    /**
     * Adds the given value to $this set if it is not already in $this set.
     */
    function add($value);

    /**
     * @todo what happens when the value could not be found? Silent no-op?
     */
    function remove($value);
}

/**
 * A structure that associates one value with another and provides the ability
 * to query or adjust associations efficiently.
 *
 * @todo Naming - dictionary
 */
interface ReadOnlyMap
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
     * @todo "This is ambiguous! 'has' ...what? has key or has value?"
     *       What about `$map->keys()->has(x)` or `$map->values()->has(x)` if we
     *       can achieve that without intermediaries? That would make the `has`
     *       here a bit redundant, where we would not get much value for the
     *       cost of the ambiguity. The other alternative is hasKey + hasValue,
     *       but that feels more specific than it needs to be.
     *
     * @todo What if we extend "Set" here? `has` would refer to the keys here
     *       though, but I think that is valid and intuitive enough. We would
     *       need to make sure that the set operation methods have names that
     *       also apply to Map, without ambiguity or confusion. I get that this
     *       is a bit strange but it makes sense to me contractually.
     */
    function has($key): bool;

    /**
     * @return Set All keys from the map added in traversal order.
     *
     * @todo We have to come up with a clever way to make this lazy, so we can
     *       expose a view on the structure's keys rather than an eager copy.
     */
    function keys(): Set;

    /**
     * @return Set All keys from the map added in traversal order.
     *
     * @todo We have to come up with a clever way to make this lazy, so we can
     *       expose a view on the structure's values rather than an eager copy.
     */
    function values(): Sequence;
}

/**
 * A map which can be modified, either in-place or as a copy.
 */
interface Map extends ReadOnlyMap
{
    /**
     * Associates the $key with the $value, overriding any previous association.
     *
     * @throws InvalidKeyException if the map implementation does not support
     *                             the given key, eg. NULL
     *
     * @todo Naming - set/put (set/unset symmetry, add/remove, put/pull?)
     */
    function set($key, $value);

    /**
     * Removes the given $key from the map, and does nothing if they key could
     * not be found.
     *
     * @todo Should we return the removed value? (No, ambiguous and two-in-one)
     *
     * @todo Key not found: should we throw, do nothing, or not enforce either?
     */
    function unset($key);
}

/**
 * Indicates that a structure supports tree traversals.
 */
interface Tree
{
    /**
     * Root, Left, Right
     */
    function preorder(): Traversable;

    /**
     * Left, Root, Right
     */
    function inorder(): Traversable;

    /**
     * Left, Right, Root
     */
    function postorder(): Traversable;

    /**
     * Right, Root, Left
     */
    function outorder(): Traversable;
}


/******************************************************************************/
/*                                 CLASSES                                    */
/******************************************************************************/

/**
 * Basic list, dynamic size, contiguous, no ops at the front.
 */
final class Vector implements
    ArrayAccess,
    Arrayable,
    Collection,
    Container,
    Clearable,
    Sortable,
    Reversable,
    Sequence
    {}

/**
 * Not necessarily literally a linked list, but has the same semantics.
 *
 * Very similar to vector, but does support operations at the front. Will most
 * likely be implemented as a circular buffer / "deque", but is an important
 * distinction because the go-to will be a Vector, except when you want to do
 * operations at the front, in which case you know you must use a LinkedList.
 *
 * This could be a bit annoying when you have `Sequence` throughout your app but
 * you really just want to remove the first element in a rare special case. You
 * have to either be able to slice from an index, or shift. Your Sequence is no
 * good because it can't be shifted. You would have to use the Deque interface
 * which Vectors don't support, so you go all the way up the chain to create a
 * LinkedList. This is either fine, or we should allow Vector to be a Deque also,
 * which enables poor performance and raises the question of "when should I use
 * LinkedList rather than Vector?"
 *
 * This structure (the deque, not the list) is my favourite and it would be hard
 * to delete all the work that was put into ext-ds. I believe that it is a core
 * structure because nothing else provides constant time add and remove at both
 * ends of the buffer. If we can agree that this structure has a place, all we
 * have to do is present it clearly. A linked list is fundamental, easy to under-
 * stand, and communicates the behaviour of the structure. If there is any
 * objection to effectively lying about the name, we should come up with another,
 * like... *no ideas*
 */
final class LinkedList implements
    ArrayAccess,
    Arrayable,
    Collection,
    Container,
    Clearable,
    Sortable,
    Reversable,
    Deque
    {}

/**
 * The standard hashtable, implemented as Map in ext-ds currently. It is based
 * on the PHP array but is not identical. We can implement OffsetAccess because
 * insertion order is preserved.
 *
 * @todo Should we implement Arrayable here? We would have to throw if an object
 *       is used as a key, and there will also be inconsistencies eg. "1" and 1.
 */
final class HashMap implements
    ArrayAccess,
    Collection,
    Container,
    Clearable,
    Sortable,
    Reversable,
    OffsetAccess,
    Map
    {}

/**
 * The Set equivalent of HashMap, which might not use a HashMap internally, but
 * will always preserve insertion order.
 */
final class HashSet implements
    ArrayAccess,
    Arrayable,
    Collection,
    Container,
    Clearable,
    Sortable,
    Reversable,
    ReadOnlySequence,
    Set
    {}

/**
 * A structure that is always sorted and does not allow duplicate elements.
 *
 * This would be useful for building sorted datasets, or sets without hashing.
 *
 * See: https://github.com/php-ds/ext-ds/issues/121
 */
final class BinarySearchTree implements
    Arrayable,
    Collection,
    Container,
    Clearable,
    Set,
    Tree,
    {}

/**
 * A first-in-last-out structure. Destructive traversal.
 */
final class Stack implements Traversable, Arrayable, Container, Clearable
{
    public function push($value) {}
    public function pop() {}
    public function peek() {}
}

/**
 * A first-in-first-out structure. Destructive traversal.
 */
final class Queue implements Traversable, Arrayable, Container, Clearable
{
    public function push($value) {}
    public function pop() {}
    public function peek() {}
}

/**
 * Binary heap-based queue with a mixed-type priority value associated with the
 * key. Iteration is destructive.
 *
 * @todo Should we provide the ability to adjust the priority of a value? I think
 *       it would be O(N) in a heap, and introduces ambiguity around duplicate
 *       values. Should it adjust them all, or just the first?
 *
 * @todo Should we support duplicate values at all?
 *
 * @todo Why do we need this if we already have a heap?
 *       Heap supports a comparator, and is guaranteed to be stable. However,
 *       in order to impelement Queue we would HAVE to pass a callable that
 *       always returns 0, falling back to insertion order tiebreak. This feels
 *       hacky, and I think the semantics are stronger with a dedicated type.
 *       In other words it should follow insertion order if no comparator was
 *       given, which Heap would not do.
 */
final class PriorityQueue implements Traversable, Arrayable, Container, Clearable
{
    public function push($value, $priority = 0) {}
    public function pop() {}
    public function peek() {}
}

/**
 * Stable binary heap with an optional comparator, defaulting to minimum. Could
 * potentially remove the nullable which clears up the min/max ambiguity.
 */
final class Heap implements Traversable, Arrayable, Container, Clearable
{
    public function __construct(callable $comparator = null) {}

    public function push($value) {}
    public function pop() {}
    public function peek() {}
}

/**
 * A low-level memory allocation abstraction which could be useful to build
 * structures in userland that do not want to use arrays or other structures
 * as a buffer.
 *
 * @todo how should we handle elements that are undefined, ie. a new allocation
 *       will have undefined values. Internally these will be IS_UNDEF, but we
 *       should not expose that to userland. Attempting to access an undefined
 *       value will throw an exception.
 *
 * @todo What should clear do? I think that capacity should be preserved, but
 *       all values should be set the their initial state, ie. undefined
 */
final class Allocation implements ArrayAccess, Clearable
{
    public function __construct(int $capacity) {}

    public function capacity(): int {}
    public function realloc(int): void {}
    public function clear(): void {}
}
