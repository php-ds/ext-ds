--TEST--
Map::reversed - return new reversed map
--FILE--
<?php
$m = new Ds\Map(["a" => 1, "b" => 2, "c" => 3]);

$rev = $m->reversed();

// Original unchanged
$origResult = [];
foreach ($m as $k => $v) { $origResult[] = "$k=$v"; }
echo "original: " . implode(",", $origResult) . "\n";

// Reversed result
$revResult = [];
foreach ($rev as $k => $v) { $revResult[] = "$k=$v"; }
echo "reversed: " . implode(",", $revResult) . "\n";

// Returns a Map
echo "type: " . get_class($rev) . "\n";

// Empty
$empty = new Ds\Map();
$revEmpty = $empty->reversed();
echo "empty: " . $revEmpty->count() . "\n";

// Single element
$single = new Ds\Map(["x" => 1]);
$revSingle = $single->reversed();
echo "single: " . $revSingle->first()->key . "=" . $revSingle->first()->value . "\n";

// Original unmodified after reversing
$m->put("d", 4);
echo "rev has d: " . var_export($rev->hasKey("d"), true) . "\n";
?>
--EXPECT--
original: a=1,b=2,c=3
reversed: c=3,b=2,a=1
type: Ds\Map
empty: 0
single: x=1
rev has d: false
