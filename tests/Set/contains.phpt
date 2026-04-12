--TEST--
Set::contains - checks if all given values are in the set
--FILE--
<?php
$s = new Ds\Set([1, 2, 3, "a", "b", null, false]);

// Single value
echo "contains 1: " . var_export($s->contains(1), true) . "\n";
echo "contains 4: " . var_export($s->contains(4), true) . "\n";
echo "contains 'a': " . var_export($s->contains("a"), true) . "\n";
echo "contains null: " . var_export($s->contains(null), true) . "\n";
echo "contains false: " . var_export($s->contains(false), true) . "\n";

// Multiple values - all must be present (AND logic)
echo "contains 1,2: " . var_export($s->contains(1, 2), true) . "\n";
echo "contains 1,4: " . var_export($s->contains(1, 4), true) . "\n";
echo "contains 1,2,3: " . var_export($s->contains(1, 2, 3), true) . "\n";

// Strict type checking
echo "contains '1': " . var_export($s->contains("1"), true) . "\n";
echo "contains 0: " . var_export($s->contains(0), true) . "\n";
echo "contains true: " . var_export($s->contains(true), true) . "\n";

// Empty set
$empty = new Ds\Set();
echo "empty contains 1: " . var_export($empty->contains(1), true) . "\n";

// Key objects
class IntKey implements Ds\Key {
    public function __construct(private int $id) {}
    public function hash(): mixed { return $this->id; }
    public function equals(mixed $other): bool {
        return $other instanceof self && $this->id === $other->id;
    }
}

$s2 = new Ds\Set();
$s2->add(new IntKey(1), new IntKey(2));
echo "key contains match: " . var_export($s2->contains(new IntKey(1)), true) . "\n";
echo "key contains miss: " . var_export($s2->contains(new IntKey(3)), true) . "\n";
echo "key contains both: " . var_export($s2->contains(new IntKey(1), new IntKey(2)), true) . "\n";
echo "key contains one miss: " . var_export($s2->contains(new IntKey(1), new IntKey(3)), true) . "\n";

// No arguments returns true
echo "contains(): " . var_export($s->contains(), true) . "\n";
?>
--EXPECT--
contains 1: true
contains 4: false
contains 'a': true
contains null: true
contains false: true
contains 1,2: true
contains 1,4: false
contains 1,2,3: true
contains '1': false
contains 0: false
contains true: false
empty contains 1: false
key contains match: true
key contains miss: false
key contains both: true
key contains one miss: false
contains(): true
