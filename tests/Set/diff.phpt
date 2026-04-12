--TEST--
Set::diff - returns elements in this but not in other
--FILE--
<?php
$a = new Ds\Set([1, 2, 3, 4]);
$b = new Ds\Set([3, 4, 5, 6]);

// Basic diff
$diff = $a->diff($b);
echo "a diff b: " . $diff->join(",") . "\n";
echo "a diff b count: " . $diff->count() . "\n";

// Reverse diff
$diff2 = $b->diff($a);
echo "b diff a: " . $diff2->join(",") . "\n";

// Original sets unchanged
echo "a unchanged: " . $a->join(",") . "\n";
echo "b unchanged: " . $b->join(",") . "\n";

// Diff with empty set
$empty = new Ds\Set();
$diff3 = $a->diff($empty);
echo "a diff empty: " . $diff3->join(",") . "\n";

// Empty diff with set
$diff4 = $empty->diff($a);
echo "empty diff a: " . $diff4->count() . "\n";

// Diff with self
$diff5 = $a->diff($a);
echo "a diff a: " . $diff5->count() . "\n";

// Diff with superset
$super = new Ds\Set([1, 2, 3, 4, 5]);
$diff6 = $a->diff($super);
echo "a diff superset: " . $diff6->count() . "\n";

// Diff with disjoint set
$disjoint = new Ds\Set([10, 20, 30]);
$diff7 = $a->diff($disjoint);
echo "a diff disjoint: " . $diff7->join(",") . "\n";

// Returns new Set instance
echo "new instance: " . var_export($diff !== $a && $diff !== $b, true) . "\n";
?>
--EXPECT--
a diff b: 1,2
a diff b count: 2
b diff a: 5,6
a unchanged: 1,2,3,4
b unchanged: 3,4,5,6
a diff empty: 1,2,3,4
empty diff a: 0
a diff a: 0
a diff superset: 0
a diff disjoint: 1,2,3,4
new instance: true
