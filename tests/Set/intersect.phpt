--TEST--
Set::intersect - returns elements in both sets
--FILE--
<?php
$a = new Ds\Set([1, 2, 3, 4]);
$b = new Ds\Set([3, 4, 5, 6]);

// Basic intersect
$result = $a->intersect($b);
echo "a intersect b: " . $result->join(",") . "\n";
echo "count: " . $result->count() . "\n";

// Commutative
$result2 = $b->intersect($a);
echo "b intersect a: " . $result2->join(",") . "\n";

// Originals unchanged
echo "a unchanged: " . $a->join(",") . "\n";
echo "b unchanged: " . $b->join(",") . "\n";

// Intersect with empty
$empty = new Ds\Set();
$result3 = $a->intersect($empty);
echo "a intersect empty: " . $result3->count() . "\n";

// Empty intersect with set
$result4 = $empty->intersect($a);
echo "empty intersect a: " . $result4->count() . "\n";

// Intersect with self
$result5 = $a->intersect($a);
echo "a intersect a: " . $result5->join(",") . "\n";

// Disjoint sets
$disjoint = new Ds\Set([10, 20]);
$result6 = $a->intersect($disjoint);
echo "disjoint: " . $result6->count() . "\n";

// Subset
$sub = new Ds\Set([2, 3]);
$result7 = $a->intersect($sub);
echo "a intersect subset: " . $result7->join(",") . "\n";

// Returns new Set
echo "new instance: " . var_export($result !== $a && $result !== $b, true) . "\n";
?>
--EXPECT--
a intersect b: 3,4
count: 2
b intersect a: 3,4
a unchanged: 1,2,3,4
b unchanged: 3,4,5,6
a intersect empty: 0
empty intersect a: 0
a intersect a: 1,2,3,4
disjoint: 0
a intersect subset: 2,3
new instance: true
