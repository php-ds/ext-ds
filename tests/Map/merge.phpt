--TEST--
Map::merge - return new map with merged values
--FILE--
<?php
$m = new Ds\Map(["a" => 1, "b" => 2]);

// Merge with array, existing key overwritten
$merged = $m->merge(["b" => 20, "c" => 30]);
echo "merged count: " . $merged->count() . "\n";
echo "a: " . $merged->get("a") . "\n";
echo "b: " . $merged->get("b") . "\n";
echo "c: " . $merged->get("c") . "\n";

// Original unchanged
echo "orig count: " . $m->count() . "\n";
echo "orig b: " . $m->get("b") . "\n";

// Merge with empty array
$merged2 = $m->merge([]);
echo "merge empty: " . $merged2->count() . "\n";

// Merge with iterator
$merged3 = $m->merge(new ArrayIterator(["x" => 10]));
echo "merge iter count: " . $merged3->count() . "\n";
echo "has x: " . var_export($merged3->hasKey("x"), true) . "\n";

// Merge returns a Map
echo "type: " . get_class($merged) . "\n";

// Merge with all new keys
$merged4 = $m->merge(["c" => 3, "d" => 4]);
echo "all new count: " . $merged4->count() . "\n";

// Merge from empty map
$empty = new Ds\Map();
$merged5 = $empty->merge(["a" => 1]);
echo "from empty: " . $merged5->count() . "\n";
echo "val: " . $merged5->get("a") . "\n";
?>
--EXPECT--
merged count: 3
a: 1
b: 20
c: 30
orig count: 2
orig b: 2
merge empty: 2
merge iter count: 3
has x: true
type: Ds\Map
all new count: 4
from empty: 1
val: 1
