--TEST--
Set::reversed - returns a new reversed Set
--FILE--
<?php
$s = new Ds\Set([1, 2, 3, 4, 5]);

// Basic reversed
$r = $s->reversed();
echo "reversed: " . $r->join(",") . "\n";

// Original unchanged
echo "original: " . $s->join(",") . "\n";

// Single element
$single = new Ds\Set([42]);
$r2 = $single->reversed();
echo "single: " . $r2->join(",") . "\n";

// Empty set
$empty = new Ds\Set();
$r3 = $empty->reversed();
echo "empty: " . $r3->count() . "\n";

// Strings
$str = new Ds\Set(["a", "b", "c"]);
$r4 = $str->reversed();
echo "strings: " . $r4->join(",") . "\n";

// Returns new Set
echo "new instance: " . var_export($r !== $s, true) . "\n";
?>
--EXPECT--
reversed: 5,4,3,2,1
original: 1,2,3,4,5
single: 42
empty: 0
strings: c,b,a
new instance: true
