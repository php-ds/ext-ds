--TEST--
Set::add - adds unique values, ignores duplicates
--FILE--
<?php
$s = new Ds\Set();

// Add single value
$s->add(1);
echo "after add(1): " . $s->count() . "\n";

// Add duplicate
$s->add(1);
echo "after dup add(1): " . $s->count() . "\n";

// Add multiple values at once
$s->add(2, 3, 4);
echo "after add(2,3,4): " . $s->count() . "\n";

// Add with duplicates in the arguments
$s->add(5, 5, 6);
echo "after add(5,5,6): " . $s->count() . "\n";

// Strict type distinction: 0, false, "", null, "0" are all different
$s2 = new Ds\Set();
$s2->add(0);
$s2->add(false);
$s2->add("");
$s2->add(null);
$s2->add("0");
echo "distinct falsy count: " . $s2->count() . "\n";

// Strings vs integers
$s3 = new Ds\Set();
$s3->add(1);
$s3->add("1");
echo "1 vs '1' count: " . $s3->count() . "\n";

// Float vs int
$s4 = new Ds\Set();
$s4->add(1);
$s4->add(1.0);
echo "1 vs 1.0 count: " . $s4->count() . "\n";

// true vs 1
$s5 = new Ds\Set();
$s5->add(true);
$s5->add(1);
echo "true vs 1 count: " . $s5->count() . "\n";

// Objects: different instances are different values
$s6 = new Ds\Set();
$a = new stdClass();
$b = new stdClass();
$s6->add($a);
$s6->add($b);
$s6->add($a); // duplicate
echo "objects count: " . $s6->count() . "\n";

// Key objects: dedup by equals
class IntKey implements Ds\Key {
    public function __construct(private int $id) {}
    public function hash(): mixed { return $this->id; }
    public function equals(mixed $other): bool {
        return $other instanceof self && $this->id === $other->id;
    }
}
$s7 = new Ds\Set();
$s7->add(new IntKey(1));
$s7->add(new IntKey(2));
$s7->add(new IntKey(1)); // duplicate by equals
echo "key objects count: " . $s7->count() . "\n";

// Add no arguments
$s8 = new Ds\Set([1, 2]);
$s8->add();
echo "add() no args: " . $s8->count() . "\n";
?>
--EXPECT--
after add(1): 1
after dup add(1): 1
after add(2,3,4): 4
after add(5,5,6): 6
distinct falsy count: 5
1 vs '1' count: 2
1 vs 1.0 count: 2
true vs 1 count: 2
objects count: 2
key objects count: 2
add() no args: 2
