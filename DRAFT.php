<?php
/*
|--------------------------------------------------------------------------
| PHP Data Structures - API Draft - v2.0
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
 * Indicates that the elements of a structure can be represented by an array
 * without altering the structure of the data, without any warnings or errors.
 */
interface Arrayable
{
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
 * Indicates that a structure can be accessed using a zero-based integer index
 * indicating the position of an element from the beginning of the structure.
 *
 * @todo Naming - OrdinalAccess, LinearAccess, RandomAccess...
 */
interface OffsetAccess
{
    /**
     * @return mixed The value at the
     *
     * @throws IndexOutOfBoundsException if the index is not within [0, size).
     *
     * @todo Potential for a better name, like "skip" or "atIndex" or "at"?
     *
     * @todo We could change this to `get`, but that conflicts with structures
     *       like HashMap in which `get` does a table lookup by key. If we do
     *       want to change this to `get`, it would mean that HashMap can't
     *       implement it, which may not be such a bad thing? Feels _slightly_
     *       odd for HashMap to implement linear access, but because it already
     *       does (similar internal structure as arrays, insertion order pres.),
     *       it's an easy value-add at no cost. Will need some help with this.
     */
    function offset(int $index);
}


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
 * Indicates that a structure contains a series of contiguous elements.
 */
interface Sequence
{
    /**
     * @return mixed The value at the given position.
     *
     * @throws IndexOutOfBoundsException if the index is not within [0, size)
     */
    function get(int $index);

    /**
     * @return int the index of the value, or NULL if it could not be found.
     */
    function indexOf($value): ?int;

    /**
     * Returns a subsection of the sequence.
     *
     * Note: We can do this internally without creating a copy of the slice by
     *       sharing the same contiguous memory with an offset and length.
     */
    function slice(int $offset, int $length): Sequence;

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
     * @throws IndexOutOfBoundsException if the index is out of range [0, size]
     */
    function insert(int $index, $value);
}


/**
 * Indicates that a structure is designed to operate efficiently at both the
 * front and the back of a linear dataset.
 */
interface Deque
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
 * A first-in-last-out structure.
 *
 * @todo Should stacks be iterable, and if so, do they have to be destructive?
 *
 *       It gets tricky because stacks yield from the back as values as popped
 *       off the stack, but implementing structures like a Vector would iterate
 *       from the front, which would not be intuitive at all. Perhaps the best
 *       solution here is that implementations must iterate in reverse.
 *
 *       We could potentially do some internal magic to force reverse iteration?
 *       (Override iterator creation when a userland object implements Stack)
 *
 * @todo If we decide that this interface must be traversed in FILO order, then
 *       Vector can not implement it. We could instead make this a final class,
 *       using a vector internally? A stack is a stack, no?
 *
 *       Vector would need to find `push` and `pop` elsewhere, which are both
 *       constant time operations at the end of the buffer. LinkedList has these
 *       too, but they come as part of Deque, which Vector does not implement.
 *
 *       Maybe Deque can be a composite of two separate interfaces? Or we make
 *       `push` and `pop` part of Sequence? That doesn't feel quite right because
 *       a sequence of values does not necessarily imply that end-ops are O(1).
 */
interface Stack
{
    function push($value);

    /**
     * @throws EmptyContainerException if the stack is empty.
     */
    function pop();

    /**
     * @throws EmptyContainerException if the stack is empty.
     */
    function peek();
}


/**
 * A first-in-first-out structure.
 *
 * @todo Should queues be iterable, and if so, do they have to be destructive.
 *
 * @todo If Stack becomes a class, should Queue remain an interface?
 */
interface Queue
{
    function push($value);

    /**
     * @throws EmptyContainerException if the stack is empty.
     */
    function pop();

    /**
     * @throws EmptyContainerException if the stack is empty.
     */
    function peek();
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
interface Set
{
    /**
     * Adds the given value to $this set if it is not already in $this set.
     */
    function add($value);

    /**
     * @todo what happens when the value could not be found? Silent no-op?
     */
    function remove($value);

    /**
     * @return bool TRUE if the value is in $this set, FALSE otherwise.
     */
    function has($value): bool;

    /**
     * @return static
     *
     * @todo Naming - or, union, merge
     */
    function or(Set $other): Set;

    /**
     * @return static
     */
    function xor(Set $other): Set;

    /**
     * @return static
     *
     * @todo Naming - not, diff, without
     */
    function not(Set $other): Set;

    /**
     * @return static
     *
     * @todo Naming - and, intersect
     */
    function and(Set $other): Set;
}


/**
 * A structure that associates one value with another and provides the ability
 * to query or adjust associations efficiently.
 *
 * @todo Naming - dictionary
 */
interface Map
{
    /**
     * Associates the $key with the $value, overriding any previous association.
     *
     * Note: Might not be able to support NULL keys. (See `find`)
     *
     * @todo Naming - set/put (set/unset symmetry, add/remove, put/pull?)
     */
    function set($key, $value);

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
     * Removes the given $key from the map, and does nothing if they key could
     * not be found.
     *
     * @todo Should we return the removed value? (No, ambiguous and two-in-one)
     *
     * @todo Key not found: should we throw, do nothing, or not enforce either?
     */
    function unset($key);

    /**
     * @return bool TRUE if $this map has an association for the given $key,
     *              FALSE otherwise.
     *
     * @todo "This is ambiguous! 'has' ...what? has key or has value?"
     *       What about `$map->keys()->has(x)` or `$map->values()->has(x)` if we
     *       can achieve that without intermediaries? That would make the `has`
     *       here a bit redundant, where we would not get much value for the
     *       cost of the ambiguity. The other alternative is hasKey + hasValue,
     *       but that feels more specific than it needs to be, or just me?
     *
     *       This also has a very "set" vibe to it. What other structures have a
     *       `has` as part of its semantics? Lists usually opt for `contains` or
     *       have `indexOf` return something nonesense to indicate no result.
     *
     *       "What if we implement Set" would be weird because we would also get
     *       `add` and `remove` and all the potential set operations. But is it
     *       that bad really? The `add`, yes. The `remove`, a bit yes (ambig.)
     *       But we want the `has` and we don't care for the operations much.
     *       So we could extract `has` to another interface which Set extends,
     *       allowing us to implement that here?
     *
     *       Or we _do_ care about the set operations, if we can get the naming
     *       right we can also intersect values, diff, etc. Works for arrays.
     *       In that case the Set interface only has `add` and `remove` that
     *       are throwing us off. We could move `add` and `remove` to another
     *       interface like a `Bag` or merge into `Container`?
     */
    function has($key): bool;

    /**
     * @return mixed The first key that is associated with the given value, or
     *               NULL if the value could not be found.
     *
     * @todo This could potentially double as a `has`, just like `indexOf` can.
     *
     * @todo The NULL return here forces the contraint that maps can't have NULL
     *       values, which implementations should enforce themselves. Ideally we
     *       should be able to do reverse lookups like these, but we have no way
     *       to indicate "no key" without being ambiguous.
     *
     *       We could choose not to support lookups by value. It's a very simple
     *       thing to implement (foreach, if value equal value, return key), so
     *       there wouldn't be a large capability gap. We could instead look at
     *       bi-directional later on? Or leave this to iteration-based projects.
     *
     * @todo Naming - find, lookup,
     */
    function find($value);

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
 * Indicates that a structure supports tree traversals.
 */
interface TreeTraversal
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


/**
 * Basic list, dynamic size, contiguous, no ops at the front.
 */
final class Vector implements
    Traversable,
    ArrayAccess,
    Container,
    Clearable,
    Sortable,
    Reversable,
    OffsetAccess,
    Sequence,
    Stack
    {}


/**
 * Not necessarily literally a linked list, but has the same theoretical props.
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
    Traversable,
    ArrayAccess,
    Container,
    Clearable,
    Sortable,
    Reversable,
    OffsetAccess,
    Sequence,
    Stack,
    Deque
    {}


/**
 * The standard hashtable, implemented as Map in ext-ds currently. It is based
 * on the PHP array but is not identical. We can implement OffsetAccess because
 * insertion order is preserved.
 */
final class HashMap implements
    Traversable,
    ArrayAccess,
    Container,
    Clearable,
    Sortable,
    Reversable,
    OffsetAccess,
    Collection,
    Map
    {}


/**
 * The Set equivalent of HashMap. Might not use the same internals as HashMap.
 */
final class HashSet implements
    Traversable,
    ArrayAccess,
    Container,
    Clearable,
    Sortable,
    Reversable,
    OffsetAccess,
    Collection,
    Set
    {}


/**
 * A structure that is always sorted and does not allow duplicate elements.
 *
 * See: https://github.com/php-ds/ext-ds/issues/121
 */
final class BinarySearchTree implements
    Traversable,
    TreeTraversal,
    Container,
    Clearable,
    Set
    {}


/**
 * A queue implementation that does not require hashing, a heap, or comparison.
 *
 * @todo This will be based on LinkedList, but LinkedList can't implement Queue
 *       because it might have to implement destructive traversal. There are
 *       some notes on Stack and Queue that would need to be resolved before we
 *       can decide whether this structure is necessary at all.
 */
final class LinkedQueue implements
    Traversable,
    Container,
    Clearable,
    Queue
    {}


/**
 * Binary heap-based queue with a mixed-type priority value associated with the
 * key. Iteration is destructive because that is how a heap works, and matches
 * the declared behaviour by Queue (destructive iteration).
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
final class PriorityQueue implements Traversable, Container, Clearable, Queue
{
    // push(value, priority = null)
}


/**
 * Stable binary heap with an optional comparator, defaulting to minimum. Could
 * potentially remove the nullable which clears up the min/max ambiguity.
 *
 * -> __construct(callable $comparator = null)
 */
final class Heap implements Traversable, Container, Clearable
{
    // push
    // pop
    // peek
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
    // __construct(int $capacity)

    // capacity(): int
    // realloc(int): void

    // clone(): self
    // clear(): void
}
