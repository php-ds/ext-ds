<?php

interface Mappable
{
    /**
     * @return static
     */
    function map(callable cb): Mappable;
}


/**
 * An iterator that yields values modified by a callback, preserving keys.
 */
class MapIterator implements IteratorAggregate
{
    private $collection;
    private $callback;

    public function __construct(Mappable $collection, callable $callback)
    {
        $this->collection = $collection;
        $this->callback   = $callback;
    }

    public function getIterator()
    {
        foreach ($this->collection as $key => $value) {
            yield $key => $this->callback($value);
        }
    }
}


/**
 * An iterator that yields no values.
 */
class EmptyIterator implements IteratorAggregate
{
    public function getIterator()
    {
        yield from [];
    }
}


/**
 * A collection that is backed by an iterator which is only evaluated as needed.
 */
abstract class LazyCollection implements IteratorAggregate, Mappable
{
    protected $iter;

    public function __construct(Iterator $iter = null)
    {
        $this->iter = $iter ?? (new EmptyIterator);
    }

    /**
     * Evaluates the remaining values in the iterator.
     *
     * @todo What happens when we rewind the iterator?
     * @todo When we clone $this, what happens to the iterator?
     */
    abstract protected function evaluate(): Iterator;

    /**
     * Create a copy if referenced more than once, otherwise return self.
     */
    protected function copyOnWrite()
    {
        return (internal_refcount_func($this) > 1) ? clone $this : $this;
    }

    /**
     * Generates a new collection from this collection keys and mapped values.
     *
     * @return static
     */
    public function map(callable $cb): Collection
    {
        /* External */
        $MapIterator = function (Collection $c) {
            foreach ($collection as $index => $value) {
                yield $index => cb($value);
            }
        };

        return new static(new MapIterator($this));
    }
}


/**
 * Basic list, but lazy.
 */
final class LazyList extends LazyCollection
{
    /**
     * Internal storage.
     */
    private $data = [];

    /**
     * Evaluates the remaining values in the iterator.
     */
    protected function evaluate(): Iterator
    {
        for ($iter = $this->iter; $iter->valid(); $iter->next()) {
            $value = $iter->current();

            /* Add to the cache. */
            $this->data[] = $value;

            yield $value;
        }
    }

    /**
     * Appends a single value and returns the resulting Sequence, which could
     * either be this instance or a clone.
     */
    public function push($value): Sequence
    {
        $result = $this->copyOnWrite();

        $result->data[] = $value;

        return $result;
    }

    /**
     * Yield from cached data first, then from the iterator, caching as we go.
     */
    public function getIterator()
    {
        $index = 0;

        foreach ($this->data as $value) {
            yield $index++ => $value;
        }

        /* TODO: What happens when we rewind the iterator? */
        foreach ($this->evaluate() as $value) {
            yield $index++ => $value;
        }
    }
}


/**
 * Basic set of distinct values.
 */
final class LazySet extends LazyCollection
{
    /**
     * Internal storage.
     */
    private $data = [];

    /**
     * Evaluates the remaining values in the iterator.
     */
    protected function evaluate(): Iterator
    {
        for ($iter = $this->iter; $iter->valid(); $iter->next()) {
            $value = $iter->current();

            /* Do not include the value if already cached. */
            if (isset($this->data[$value])) {
                continue;
            }

            /* Add to the cache. */
            $this->data[$value] = true;

            yield $value;
        }
    }

    /**
     * Adds a value to this Set if the value is not already in this Set.
     */
    public function add($value): Sequence
    {
        /* Do not do anything at all if the operation has no effect. */
        if (isset($this->data[$value])) {
            return $this;
        }

        /* Evaluate the remaining values in the iterator */
        foreach ($this->evaluate() as $candidate) {
            if ($value === $candidate) {
                return $this;
            }
        }

        /* We are about to make an update, so check if we need to copy first. */
        $result = $this->copyOnWrite();

        /* Add the value to the set. */
        $result->data[$value] = true;

        return $result;
    }

    /**
     * Determines if this Set contains the given value.
     */
    public function has($value): bool
    {
        if (isset($this->data[$value])) {
            return true;
        }

        /* Evaluate the remaining values in the iterator */
        foreach ($this->evaluate() as $candidate) {
            if ($value === $candidate) {
                return true;
            }
        }

        return false;
    }

    /**
     * Iterator using a sequential index of the data store's keys as values.
     */
    public function getIterator()
    {
        $index = 0;

        foreach ($this->data as $value => $_) {
            yield $index++ => $value;
        }

        foreach ($this->evaluate() as $value) {
            yield $index++ => $value;
        }
    }

    /**
     * Returns the values of this set as a list.
     */
    public function toList(): LazyList
    {
        return new LazyList($this);
    }
}


$set = new LazySet();
$set->add(1);
$set->add(2);
$set->add(3);

// $set has not been copied at all because nothing else is referencing it yet.

// This creates a list of all odd values in the set.
// `toList` and `filter` are lazy (decorated iterators, no eager evaluation)
$odds = $set->toList()->filter('isodd');
// Refcount of $set is now 2.

$copy = $set;
// Refcount of $set is now 3.

// When we update the copy, we are actually updating $set. But its refcount is 3,
// so others depend on it. We can therefore not update the original, so we create
// a copy instead by separating $other from $map (and copying the internal store).
//
// $odds refcount is 2
// $copy refcount is 1
$copy = $copy->add(5);

// If we evaluate $odds, it builds the internal cache using the iterator.
foreach ($odds as $odd) {} // 1, 3

// Once all values have been cached by the list, the iterator is cleared.
// This brings $odds' refcount back to 1.

// We could have achieved this by using `$set->filter('isodd')` as well,

