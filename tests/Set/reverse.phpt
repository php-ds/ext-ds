--TEST--
Set::reverse - reverses the set in-place
--FILE--
<?php
$s = new Ds\Set([1, 2, 3, 4, 5]);

// Basic reverse
$s->reverse();
echo "reversed: " . $s->join(",") . "\n";

// Double reverse restores original order
$s->reverse();
echo "double reversed: " . $s->join(",") . "\n";

// Single element
$single = new Ds\Set([42]);
$single->reverse();
echo "single: " . $single->join(",") . "\n";

// Two elements
$two = new Ds\Set(["a", "b"]);
$two->reverse();
echo "two: " . $two->join(",") . "\n";

// Empty set
$empty = new Ds\Set();
$empty->reverse();
echo "empty count: " . $empty->count() . "\n";

// Returns void (no chaining)
$s2 = new Ds\Set([1, 2, 3]);
$result = $s2->reverse();
echo "returns null: " . var_export($result, true) . "\n";
?>
--EXPECT--
reversed: 5,4,3,2,1
double reversed: 1,2,3,4,5
single: 42
two: b,a
empty count: 0
returns null: NULL
