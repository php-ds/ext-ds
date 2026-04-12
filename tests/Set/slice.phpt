--TEST--
Set::slice - returns a subset by position
--FILE--
<?php
$s = new Ds\Set(["a", "b", "c", "d", "e"]);

// Basic slice with offset and length
$r = $s->slice(1, 3);
echo "slice(1,3): " . $r->join(",") . "\n";

// Offset only (to end)
$r2 = $s->slice(2);
echo "slice(2): " . $r2->join(",") . "\n";

// Offset 0
$r3 = $s->slice(0, 2);
echo "slice(0,2): " . $r3->join(",") . "\n";

// Full slice
$r4 = $s->slice(0);
echo "slice(0): " . $r4->join(",") . "\n";

// Negative offset
$r5 = $s->slice(-2);
echo "slice(-2): " . $r5->join(",") . "\n";

// Negative offset with length
$r6 = $s->slice(-3, 2);
echo "slice(-3,2): " . $r6->join(",") . "\n";

// Zero length
$r7 = $s->slice(1, 0);
echo "slice(1,0) count: " . $r7->count() . "\n";

// Length exceeds remaining
$r8 = $s->slice(3, 100);
echo "slice(3,100): " . $r8->join(",") . "\n";

// Original unchanged
echo "original: " . $s->join(",") . "\n";

// Empty set
$empty = new Ds\Set();
$r9 = $empty->slice(0);
echo "empty: " . $r9->count() . "\n";

// Returns new Set
echo "new instance: " . var_export($r !== $s, true) . "\n";

// Single element slice
$single = new Ds\Set([42]);
echo "single slice(0,1): " . $single->slice(0, 1)->join(",") . "\n";
?>
--EXPECT--
slice(1,3): b,c,d
slice(2): c,d,e
slice(0,2): a,b
slice(0): a,b,c,d,e
slice(-2): d,e
slice(-3,2): c,d
slice(1,0) count: 0
slice(3,100): d,e
original: a,b,c,d,e
empty: 0
new instance: true
single slice(0,1): 42
