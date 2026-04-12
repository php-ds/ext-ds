--TEST--
Set::union - returns all elements from both sets
--FILE--
<?php
$a = new Ds\Set([1, 2, 3]);
$b = new Ds\Set([3, 4, 5]);

// Basic union
$result = $a->union($b);
echo "a union b: " . $result->join(",") . "\n";
echo "count: " . $result->count() . "\n";

// Commutative (same elements, possibly different order)
$result2 = $b->union($a);
echo "b union a count: " . $result2->count() . "\n";

// Originals unchanged
echo "a: " . $a->join(",") . "\n";
echo "b: " . $b->join(",") . "\n";

// Union with empty
$empty = new Ds\Set();
$result3 = $a->union($empty);
echo "a union empty: " . $result3->join(",") . "\n";

// Empty union with set
$result4 = $empty->union($a);
echo "empty union a: " . $result4->join(",") . "\n";

// Union with self
$result5 = $a->union($a);
echo "a union a: " . $result5->join(",") . "\n";

// Disjoint sets
$disjoint = new Ds\Set([10, 20]);
$result6 = $a->union($disjoint);
echo "disjoint: " . $result6->join(",") . "\n";

// Identical sets
$clone = new Ds\Set([1, 2, 3]);
$result7 = $a->union($clone);
echo "identical: " . $result7->join(",") . "\n";
echo "identical count: " . $result7->count() . "\n";

// Returns new Set
echo "new instance: " . var_export($result !== $a && $result !== $b, true) . "\n";
?>
--EXPECT--
a union b: 1,2,3,4,5
count: 5
b union a count: 5
a: 1,2,3
b: 3,4,5
a union empty: 1,2,3
empty union a: 1,2,3
a union a: 1,2,3
disjoint: 1,2,3,10,20
identical: 1,2,3
identical count: 3
new instance: true
