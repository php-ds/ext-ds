--TEST--
Set::sorted - returns a new sorted Set
--FILE--
<?php
$s = new Ds\Set([3, 1, 4, 5, 2]);

// Default sort
$sorted = $s->sorted();
echo "sorted: " . $sorted->join(",") . "\n";

// Original unchanged
echo "original: " . $s->join(",") . "\n";

// Custom comparator (descending)
$desc = $s->sorted(fn($a, $b) => $b - $a);
echo "descending: " . $desc->join(",") . "\n";

// Strings
$s2 = new Ds\Set(["banana", "apple", "cherry"]);
$sorted2 = $s2->sorted();
echo "strings: " . $sorted2->join(",") . "\n";

// Empty set
$empty = new Ds\Set();
$sorted3 = $empty->sorted();
echo "empty: " . $sorted3->count() . "\n";

// Single element
$single = new Ds\Set([42]);
$sorted4 = $single->sorted();
echo "single: " . $sorted4->join(",") . "\n";

// Returns new Set
echo "new instance: " . var_export($sorted !== $s, true) . "\n";

// Original still unchanged after all operations
echo "original still: " . $s->join(",") . "\n";
?>
--EXPECT--
sorted: 1,2,3,4,5
original: 3,1,4,5,2
descending: 5,4,3,2,1
strings: apple,banana,cherry
empty: 0
single: 42
new instance: true
original still: 3,1,4,5,2
