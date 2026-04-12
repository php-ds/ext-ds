--TEST--
Set::count - returns the number of elements
--FILE--
<?php
// Empty set
$s = new Ds\Set();
echo "empty: " . $s->count() . "\n";
echo "count(): " . count($s) . "\n";

// With elements
$s = new Ds\Set([1, 2, 3]);
echo "three: " . $s->count() . "\n";
echo "count(): " . count($s) . "\n";

// After add
$s->add(4);
echo "after add: " . $s->count() . "\n";

// After duplicate add
$s->add(1);
echo "after dup: " . $s->count() . "\n";

// After remove
$s->remove(1);
echo "after remove: " . $s->count() . "\n";

// After clear
$s->clear();
echo "after clear: " . $s->count() . "\n";

// Large set
$s = new Ds\Set(range(1, 1000));
echo "1000: " . $s->count() . "\n";
?>
--EXPECT--
empty: 0
count(): 0
three: 3
count(): 3
after add: 4
after dup: 4
after remove: 3
after clear: 0
1000: 1000
