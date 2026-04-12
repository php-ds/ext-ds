--TEST--
Map::map - transform values into new map
--FILE--
<?php
// Basic map: double values
$m = new Ds\Map(["a" => 1, "b" => 2, "c" => 3]);
$doubled = $m->map(function($key, $value) {
    return $value * 2;
});
echo "doubled: " . $doubled->get("a") . "," . $doubled->get("b") . "," . $doubled->get("c") . "\n";

// Original unchanged
echo "original: " . $m->get("a") . "," . $m->get("b") . "," . $m->get("c") . "\n";

// Map with key usage
$m2 = new Ds\Map(["x" => 10, "y" => 20]);
$result = $m2->map(function($key, $value) {
    return "$key:$value";
});
echo "with key: " . $result->get("x") . "," . $result->get("y") . "\n";

// Returns a Map
echo "type: " . get_class($doubled) . "\n";

// Map preserves keys
echo "has a: " . var_export($doubled->hasKey("a"), true) . "\n";
echo "count: " . $doubled->count() . "\n";

// Empty map
$empty = new Ds\Map();
$mapped = $empty->map(function($k, $v) { return $v; });
echo "empty: " . $mapped->count() . "\n";

// Map with type change
$m3 = new Ds\Map(["a" => 1, "b" => 2]);
$strings = $m3->map(function($k, $v) { return (string)$v; });
echo "type change: " . gettype($strings->get("a")) . "\n";
?>
--EXPECT--
doubled: 2,4,6
original: 1,2,3
with key: x:10,y:20
type: Ds\Map
has a: true
count: 3
empty: 0
type change: string
