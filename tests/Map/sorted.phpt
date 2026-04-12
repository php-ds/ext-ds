--TEST--
Map::sorted - return new map sorted by value
--FILE--
<?php
// Natural sort
$m = new Ds\Map(["a" => 3, "b" => 1, "c" => 2]);
$sorted = $m->sorted();

// Original unchanged
$orig = [];
foreach ($m as $k => $v) { $orig[] = "$k=$v"; }
echo "original: " . implode(",", $orig) . "\n";

// Sorted result
$sr = [];
foreach ($sorted as $k => $v) { $sr[] = "$k=$v"; }
echo "sorted: " . implode(",", $sr) . "\n";

// Custom comparator
$rev = $m->sorted(function($a, $b) {
    return $b <=> $a;
});
$rr = [];
foreach ($rev as $k => $v) { $rr[] = "$k=$v"; }
echo "reverse: " . implode(",", $rr) . "\n";

// Returns a Map
echo "type: " . get_class($sorted) . "\n";

// Empty map
$empty = new Ds\Map();
$se = $empty->sorted();
echo "empty: " . $se->count() . "\n";

// String values
$m2 = new Ds\Map(["x" => "cherry", "y" => "apple", "z" => "banana"]);
$s2 = $m2->sorted();
$r2 = [];
foreach ($s2 as $k => $v) { $r2[] = "$k=$v"; }
echo "strings: " . implode(",", $r2) . "\n";

// Original not modified
$m->put("d", 0);
echo "sorted has d: " . var_export($sorted->hasKey("d"), true) . "\n";
?>
--EXPECT--
original: a=3,b=1,c=2
sorted: b=1,c=2,a=3
reverse: a=3,c=2,b=1
type: Ds\Map
empty: 0
strings: y=apple,z=banana,x=cherry
sorted has d: false
