--TEST--
Set::xor - returns elements in either but not both
--FILE--
<?php
$a = new Ds\Set([1, 2, 3, 4]);
$b = new Ds\Set([3, 4, 5, 6]);

// Basic xor
$result = $a->xor($b);
echo "a xor b: " . $result->join(",") . "\n";
echo "count: " . $result->count() . "\n";

// Commutative (same elements)
$result2 = $b->xor($a);
echo "b xor a count: " . $result2->count() . "\n";

// Originals unchanged
echo "a: " . $a->join(",") . "\n";
echo "b: " . $b->join(",") . "\n";

// Xor with empty (same as original)
$empty = new Ds\Set();
$result3 = $a->xor($empty);
echo "a xor empty: " . $result3->join(",") . "\n";

// Empty xor with set
$result4 = $empty->xor($a);
echo "empty xor a: " . $result4->join(",") . "\n";

// Xor with self (empty result)
$result5 = $a->xor($a);
echo "a xor a: " . $result5->count() . "\n";

// Identical sets
$clone = new Ds\Set([1, 2, 3, 4]);
$result6 = $a->xor($clone);
echo "identical xor: " . $result6->count() . "\n";

// Disjoint sets (everything)
$disjoint = new Ds\Set([10, 20]);
$result7 = $a->xor($disjoint);
echo "disjoint xor: " . $result7->join(",") . "\n";

// Returns new Set
echo "new instance: " . var_export($result !== $a && $result !== $b, true) . "\n";
?>
--EXPECT--
a xor b: 1,2,5,6
count: 4
b xor a count: 4
a: 1,2,3,4
b: 3,4,5,6
a xor empty: 1,2,3,4
empty xor a: 1,2,3,4
a xor a: 0
identical xor: 0
disjoint xor: 1,2,3,4,10,20
new instance: true
