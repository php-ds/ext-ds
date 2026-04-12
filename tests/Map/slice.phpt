--TEST--
Map::slice - extract a sub-map by position
--FILE--
<?php
$m = new Ds\Map(["a" => 1, "b" => 2, "c" => 3, "d" => 4, "e" => 5]);

// Slice with offset only - takes rest
$s1 = $m->slice(2);
$r = [];
foreach ($s1 as $k => $v) { $r[] = "$k=$v"; }
echo "offset 2: " . implode(",", $r) . "\n";

// Slice with offset and length
$s2 = $m->slice(1, 2);
$r2 = [];
foreach ($s2 as $k => $v) { $r2[] = "$k=$v"; }
echo "offset 1 len 2: " . implode(",", $r2) . "\n";

// Slice from beginning
$s3 = $m->slice(0, 3);
$r3 = [];
foreach ($s3 as $k => $v) { $r3[] = "$k=$v"; }
echo "offset 0 len 3: " . implode(",", $r3) . "\n";

// Negative offset (from end)
$s4 = $m->slice(-2);
$r4 = [];
foreach ($s4 as $k => $v) { $r4[] = "$k=$v"; }
echo "offset -2: " . implode(",", $r4) . "\n";

// Negative offset with length
$s5 = $m->slice(-3, 2);
$r5 = [];
foreach ($s5 as $k => $v) { $r5[] = "$k=$v"; }
echo "offset -3 len 2: " . implode(",", $r5) . "\n";

// Length 0
$s6 = $m->slice(0, 0);
echo "len 0: " . $s6->count() . "\n";

// Length exceeds size
$s7 = $m->slice(0, 100);
echo "len 100: " . $s7->count() . "\n";

// Returns a new Map
echo "type: " . get_class($s2) . "\n";
echo "original count: " . $m->count() . "\n";

// Negative length
$s8 = $m->slice(1, -1);
$r8 = [];
foreach ($s8 as $k => $v) { $r8[] = "$k=$v"; }
echo "offset 1 len -1: " . implode(",", $r8) . "\n";

// Empty map slice
$empty = new Ds\Map();
$s9 = $empty->slice(0);
echo "empty: " . $s9->count() . "\n";
?>
--EXPECT--
offset 2: c=3,d=4,e=5
offset 1 len 2: b=2,c=3
offset 0 len 3: a=1,b=2,c=3
offset -2: d=4,e=5
offset -3 len 2: c=3,d=4
len 0: 0
len 100: 5
type: Ds\Map
original count: 5
offset 1 len -1: b=2,c=3,d=4
empty: 0
