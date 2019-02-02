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
 * Base exception class, to allow catching any library-related exception.
 */
class Exception extends \Exception {}


/**
 * Should be thrown when a structure is accessed unexpectedly, or in a way that
 * is undefined, or to avoid ambiguous results.
 */
class AccessException extends Exception {}


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
     * @return static A sorted copy or this structure sorted in-place.
     */
    function sort(callable $comparator = null);
}


/**
 * Indicates that the elements of a structure can be represented by an array
 * without altering the structure of the data, without any warnings or errors.
 */
interface Arrayable
{
    function toArray(): array;
}


/**
 * Indicates that a structure can be duplicated without loss of data.
 */
interface Cloneable
{
    /**
     * @return static a copy of this structure, consistent with `clone`.
     */
    function clone();
}


/**
 * Indicates that a structure can be cleared.
 */
interface Clearable
{
    function clear();
}


/**
 * Indicates that a structure can be accessed using a zero-based integer index.
 *
 * @todo Maybe a better name? Hints at linear-time access a bit.
 *       OffsetAccess, OrdinalAccess, ...
 */
interface LinearAccess
{
    /**
     * @return mixed The first element, which could be a key or value.
     *               Implementations should specify what happens when there are
     *               no elements (eg. return NULL or throw AccessException).
     */
    function first();

    /**
     * @return mixed The last element, which could be a key or value.
     *               Implementations should specify what happens when there are
     *               no elements (eg. return NULL or throw AccessException).
     */
    function last();

    /**
     * @return mixed The value at the
     *
     * @throws AccessException if the index is out of range.
     *
     * @todo Potential for a better name, like "skip" or "atIndex"?
     *
     * @todo We could change this to `get`, but that conflicts with structures
     *       like HashMap in which `get` does a table lookup by key. If we do
     *       want to change this to `get`, it would mean that HashMap can't
     *       implement it, which may not be such a bad thing? Feels _slightly_
     *       odd for HashMap to implement linear access, but because it already
     *       does (similar internal structure as arrays, insertion order pres.),
     *       it's an easy value-add at no cost. Will need some help with this.
     */
    function at(int $index);

    /**
     * @return int the index of the value, or NULL if it could not be found.
     */
    function indexOf($value): ?int;
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
 * Indicates that a structure is designed to process all elements to produce a
 * new value as a result.
 *
 * @todo this feels like it belongs in an iterator library, but we get some nice
 *       wins doing these internally, because we have low-level access to the
 *       structures without the need to invoke iterators or class lookups to
 *       determine the type of the result (for cases like map, filter etc).
 *       How else would you know how to create the instance? Clone?
 */
interface Collection
{

    /**
     * @param callable $callback Called with value and key, should return what
     *                           should replace the value. Keys are preserved.
     *
     * @return static A copy of $this with each value replaced by the result of
     *                called the callback with that value (and its index/key).
     *                Keys are always preserved.
     */
    function map(callable $callback): Collection;

    /**
     * @param callback $predicate Called with value and key, determines whether
     *                            the value should be included in the result.
     *                            Should return TRUE if the value should be
     *                            included, FALSE otherwise. Keys are preserved.
     *
     * @return Collection A collection of the same type, containing all keys and
     *                    values for which the predicate returned TRUE.
     */

    function filter(callable $predicate): Collection;

    /**
     * Reduces the collection to a single value using a callback function
     *
     * @param callback $callback Called with the current value of the result and
     *                           the value and key of the current iteration.
     *                           The result is set to $initial at the start.
     *
     * @param mixed|null $initial The initial value of the result argument.
     *
     * @return mixed The final value of the result.
     */
    function reduce(callable $callback, $initial = null);

    /**
     * @return bool TRUE if empty pr the predicate returns TRUE for any element
     *              in the collection, FALSE otherwise.
     *
     * @todo Many other implementations (lodash, js) use `some` here, but I have
     *       always liked the phrasing of "any" and "all". PHP doesn't have an
     *       array equivalent for these anyway, so we have no reference for
     *       consistency. Always be innovating, or accept the convention?
     */
    function any(callable $predicate): bool;

    /**
     * @return bool TRUE if not empty and the predicate returns TRUE for all
     *              elements in the collection, FALSE otherwise.
     */
    function all(callable $predicate): bool;

    /**
     * @return string The result of concatenating the string representations of
     *                all values, separated by the given $glue.
     */
    function join(string $glue = ""): string;
}


/**
 * Indicates that a structure contains a series of contiguous elements.
 */
interface Sequence
{
    /**
     * @return mixed The value at the given position.
     *
     * @throws AccessException if the index is out of range [0, size)
     */
    function get(int $index);

    /**
     * Sets the value at the given position.
     *
     * @throws AccessException if the index is out of range [0, size)
     */
    function set(int $index, $value);

    /**
     * Removes the value at the given position, moving all successive
     * values one position towards the front.
     *
     * @throws AccessException if the index is out of range [0, size)
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
     * @throws AccessException if the index is out of range [0, size]
     */
    function insert(int $index, $value);
}


/**
 * Indicates that a structure is designed to operate efficiently at either end.
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
     * @throws AccessException if the structure is empty.
     *
     * @todo should we leave it up to the impl to throw or return NULL? I think
     *       we should aim for consistency by requiring a throw when empty. This
     *       should be consistent with LinearAccess' first and last.
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
 *
 *
 *
 *
 */
interface Stack
{
    function push($value);

    /**
     * @todo What happens when the stack is empty? AccessException?
     *       We need a way to ensure that a stack can know when it is empty,
     *       otherwise we can't implement iteration:
     *
     *           while (!$this->isEmpty()) {
     *               yield $this->pop();
     *           }
     *
     *       Maybe Stack can extend Container? Or at least Countable? What does
     *       that mean for other structures that could extend Container? We can
     *       define a policy like "only extend core interfaces" but I'm not sure
     *       that that will always be wise. I don't see an issue with extension,
     *       but I wanted to attept composition first and see how far we get.
     *
     *       We could also extend IteratorAggregate here and require that it
     *       returns an iterator that is seekable which we can wrap, or simply
     *       an instance of a reverse iterator interface? Or Stack can extend
     *       something like "ReverseIterator"?
     *
     *       I think we should avoid IteratorAggregate as an unnecessary layer.
     *       We could create our own interface which has methods like end(),
     *       prev() and valid()? Create the iterator internally when implemented.
     *
     *       I'm starting to see more and more iterator-domain stuff creeping in
     *       here. Is it counter-intuitive to separate iterators from ds? Why
     *       can't iterators be considered data structures? Or why can't a ds
     *       library also provide some iterators that are designed to work with
     *       the structures. @Levi I think this is what you meant by external
     *       iterators, so potentially on the same page here?
     */
    function pop();

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
     * See notes on Stack::pop -^
     */
    function pop();

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
     * @return Set A set from all values in $this, and all values from $other.
     *
     * @todo Naming - or, union, merge
     */
    function or(Set $other): Set;

    /**
     * @return Set A set from values in either $this or $other, but not in both.
     */
    function xor(Set $other): Set;

    /**
     * @return Set A set from values in $this that are not in $other.
     *
     * @todo Naming - not, diff, without
     */
    function not(Set $other): Set;

    /**
     * @return Set A set from values in $this that are not in $other.
     *
     * @todo Naming - and, intersect
     */
    function and(Set $other): Set;
}


/**
 * A structure that associates one value with another and provides the ability
 * to query or adjust associations efficiently.
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

    /**
     * @return Map The result of adding all associations from $map to $this.
     *
     * @todo Feels a bit isolated here - isn't this part of the set operations
     *       discussion? If so, this is already the equivalent of or/union, so
     *       it is probably smart to consider unified naming that applies to the
     *       semantics of both structures.
     */
    function merge(Map $map): Map;
}


/**
 * A low-level memory allocation abstraction which could be useful to build
 * structures in userland that do not want to use arrays or other structures
 * as a buffer.
 *
 * We have to be very careful to not go too far with this. Memory access should
 * be extremely well guarded here. We MUST have a sensible maximum capacity that
 * can be allocated, so that we do not expose the potential to crash anything.
 *
 * @todo is it safe to use undefined or should we set all elements to NULL?
 *       or we can convert to NULL when undefined is read? Would be strange to
 *       expose undefined to userland. Attempting to read an undefined value is
 *       almost certainly a bug though, so we could throw an exception if this
 *       happens rather than magically converting it to NULL (which would also
 *       make it ambiguous because you can't distinguish between null and undef)
 *
 * @todo What should clear do? I think that capacity should be preserved, but
 *       all values should be set the their initial state, ie. undefined
 */
final class Allocation implements \ArrayAccess, Cloneable, Clearable
{
    public function __construct(int $capacity) {}

    // capacity
    // realloc

    // clone
    // clear
}

/**
 * Basic list, dynamic size, contiguous, no ops at the front.
 */
final class Vector implements \ArrayAccess, Container, Cloneable, Clearable, Sortable, Reversable, LinearAccess, Collection, Sequence, Stack
{
    // get
    // set
    // unset
    // insert

    // push
    // pop
    // peek

    // map
    // reduce
    // filter
    // any
    // all
    // join

    // at
    // indexOf
    // first
    // last

    // sort
    // reverse

    // clone
    // clear
    // count
    // isEmpty
}

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
final class LinkedList implements \ArrayAccess, Container, Cloneable, Clearable, Sortable, Reversable, LinearAccess, Collection, Sequence, Stack, Deque
{
    // get
    // set
    // unset
    // insert

    // push
    // pop
    // peek

    // shift
    // unshift

    // map
    // reduce
    // filter
    // any
    // all
    // join

    // at
    // indexOf
    // first
    // last

    // sort
    // reverse

    // clone
    // clear
    // count
    // isEmpty
}

/**
 * The standard hash table, implemented as Map in ext-ds currently. It is modelled
 * after the PHP array but is not identical. We can implement LinearAccess because
 * insertion order is preserved. I don't believe we need a LinkedMap or anything
 * like that because the existing implementation is already very, very good. I did
 * experiment with open addressing for  a long time but I could not out-perform
 * the existing model.
 *
 * Keys can be any scalar value (@todo potentially not NULL though), or object
 * that implements Hashable. I don't think we should support object keys that do
 * not implement Hashable, which is a big side-step from ext-ds currently which
 * falls back to spl_object_hash. If non-hashable objects must be used as keys,
 * the implementation should use a BST instead, which does not rely on hashing.
 *
 * Alternatively, we could impelement a SortedMap using BST internally, which
 * removes the need for hashing and has the added benefit of sorted keys. This
 * could pretty easily be implemented in userland because it's such a simple
 * composition - I'm not convinced that we need to implement it internally. A
 * counter argument might be that we should standardise here while we can,
 * provide a dedicated implementation of a map that does not require hashing,
 * and is sorted by key. We could also accept an optional comparator.
 *
 * Something to consider is a SortedMap interface, implemented as TreeMap,
 * Java style, but I think a SortedMap implementation is sufficient. I can't
 * imagine many valid alternative implementations. We could also call the impl
 * TreeMap and not have "SortedMap" be anything at all.
 */
final class HashMap implements \ArrayAccess, Container, Cloneable, Clearable, Sortable, Reversable, LinearAccess, Collection, Map
{
    // get
    // set/put <Hashable | scalar, *>
    // unset
    // has
    // find (null if value not found)

    // keys
    // values

    // merge

    // map
    // reduce
    // filter
    // any
    // all
    // join

    // at
    // indexOf
    // first
    // last

    // sort
    // reverse

    // clone
    // clear
    // count
    // isEmpty
}

/**
 * The Set equivalent of HashMap.
 */
final class HashSet implements \ArrayAccess, Container, Cloneable, Clearable, Sortable, Reversable, LinearAccess, Collection, Set
{
    // add <Hashable | scalar>
    // remove
    // has

    // or
    // xor
    // not
    // and

    // map
    // reduce
    // filter
    // any
    // all
    // join

    // at
    // indexOf
    // first
    // last

    // sort
    // reverse

    // clone
    // clear
    // count
    // isEmpty
}

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
final class PriorityQueue implements Iterator, Container, Cloneable, Clearable, Queue
{
    // push (value, priority = null)
    // pop
    // peek

    // clone
    // clear
    // count
    // isEmpty
}

/**
 * A queue implementation that does not require hashing, a heap, or comparison.
 *
 * @todo This will be based on LinkedList, but LinkedList can't implement Queue
 *       because it might have to implement destructive traversal. There are
 *       some notes on Stack and Queue that would need to be resolved before we
 *       can decide whether this structure is necessary at all.
 */
final class LinkedQueue implements Container, Cloneable, Clearable, Queue
{
    // push
    // pop
    // peek

    // clone
    // clear
    // count

    // isEmpty
}

/**
 * A structure that is always sorted and does not allow duplicate elements.
 *
 * See: https://github.com/php-ds/ext-ds/issues/121
 */
final class BinarySearchTree implements Iterator, Container, Cloneable, Clearable
{
    // add
    // remove
    // has

    // clone
    // clear
    // count
    // isEmpty

    public function inorder() : Iterator {}   // <-- default iterator?
    public function preorder() : Iterator {}
    public function postorder() : Iterator {}
}

/**
 * Stable binary heap with an optional comparator, defaulting to minimum. Could
 * potentially remove the nullable which clears up the min/max ambiguity.
 */
final class Heap implements Iterator, Container, Cloneable, Clearable
{
    public function __construct(callable $comparator = null) {}

    // push
    // pop
    // peek

    // clone
    // clear
    // count
    // isEmpty
}
