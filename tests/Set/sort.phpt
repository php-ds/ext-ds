--TEST--
Set::sort - sorts the set in-place
--FILE--
<?php
// Default sort (ascending)
$s = new Ds\Set([3, 1, 4, 1, 5, 9, 2, 6]);
$s->sort();
echo "sorted ints: " . $s->join(",") . "\n";

// Strings
$s2 = new Ds\Set(["banana", "apple", "cherry"]);
$s2->sort();
echo "sorted strings: " . $s2->join(",") . "\n";

// Custom comparator (descending)
$s3 = new Ds\Set([3, 1, 4, 5, 2]);
$s3->sort(fn($a, $b) => $b - $a);
echo "descending: " . $s3->join(",") . "\n";

// Custom comparator (string length)
$s4 = new Ds\Set(["bb", "a", "ccc", "dddd"]);
$s4->sort(fn($a, $b) => strlen($a) - strlen($b));
echo "by length: " . $s4->join(",") . "\n";

// Already sorted
$s5 = new Ds\Set([1, 2, 3]);
$s5->sort();
echo "already sorted: " . $s5->join(",") . "\n";

// Single element
$single = new Ds\Set([42]);
$single->sort();
echo "single: " . $single->join(",") . "\n";

// Empty set
$empty = new Ds\Set();
$empty->sort();
echo "empty: " . $empty->count() . "\n";

// Returns void
$s6 = new Ds\Set([3, 1, 2]);
$result = $s6->sort();
echo "returns null: " . var_export($result, true) . "\n";
?>
--EXPECT--
sorted ints: 1,2,3,4,5,6,9
sorted strings: apple,banana,cherry
descending: 5,4,3,2,1
by length: a,bb,ccc,dddd
already sorted: 1,2,3
single: 42
empty: 0
returns null: NULL
