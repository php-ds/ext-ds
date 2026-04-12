--TEST--
Set::remove - removes values from the set
--FILE--
<?php
$s = new Ds\Set([1, 2, 3, 4, 5]);

// Remove single value
$s->remove(3);
echo "after remove(3): " . $s->join(",") . "\n";
echo "count: " . $s->count() . "\n";

// Remove multiple values
$s->remove(1, 5);
echo "after remove(1,5): " . $s->join(",") . "\n";

// Remove non-existent value (silent, no error)
$s->remove(99);
echo "after remove(99): " . $s->join(",") . "\n";
echo "count: " . $s->count() . "\n";

// Remove from set with duplicates in args
$s2 = new Ds\Set([1, 2, 3]);
$s2->remove(1, 1, 2);
echo "dup remove: " . $s2->join(",") . "\n";

// Remove no arguments
$s3 = new Ds\Set([1, 2, 3]);
$s3->remove();
echo "remove() no args: " . $s3->join(",") . "\n";

// Remove from empty set (no error)
$empty = new Ds\Set();
$empty->remove(1);
echo "remove from empty: " . $empty->count() . "\n";

// Strict type: remove "1" does not remove int 1
$s4 = new Ds\Set([1, 2, 3]);
$s4->remove("1");
echo "strict: " . $s4->join(",") . "\n";

// Remove with Key objects
class IntKey implements Ds\Key {
    public function __construct(private int $id) {}
    public function hash(): mixed { return $this->id; }
    public function equals(mixed $other): bool {
        return $other instanceof self && $this->id === $other->id;
    }
}

$s5 = new Ds\Set();
$s5->add(new IntKey(1), new IntKey(2), new IntKey(3));
$s5->remove(new IntKey(2));
echo "key remove count: " . $s5->count() . "\n";
echo "key has 2: " . var_export($s5->contains(new IntKey(2)), true) . "\n";
echo "key has 1: " . var_export($s5->contains(new IntKey(1)), true) . "\n";
echo "key has 3: " . var_export($s5->contains(new IntKey(3)), true) . "\n";
?>
--EXPECT--
after remove(3): 1,2,4,5
count: 4
after remove(1,5): 2,4
after remove(99): 2,4
count: 2
dup remove: 3
remove() no args: 1,2,3
remove from empty: 0
strict: 1,2,3
key remove count: 2
key has 2: false
key has 1: true
key has 3: true
