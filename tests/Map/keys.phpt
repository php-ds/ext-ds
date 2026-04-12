--TEST--
Map::keys - get all keys as a Set
--FILE--
<?php
$m = new Ds\Map(["a" => 1, "b" => 2, "c" => 3]);
$keys = $m->keys();
echo "type: " . get_class($keys) . "\n";
echo "count: " . $keys->count() . "\n";
echo "contains a: " . var_export($keys->contains("a"), true) . "\n";
echo "contains b: " . var_export($keys->contains("b"), true) . "\n";
echo "contains c: " . var_export($keys->contains("c"), true) . "\n";
echo "contains z: " . var_export($keys->contains("z"), true) . "\n";

// Keys from integer-keyed map
$m2 = new Ds\Map([10 => "x", 20 => "y"]);
$keys2 = $m2->keys();
echo "int keys count: " . $keys2->count() . "\n";
echo "contains 10: " . var_export($keys2->contains(10), true) . "\n";

// Keys from empty map
$empty = new Ds\Map();
$keys3 = $empty->keys();
echo "empty keys count: " . $keys3->count() . "\n";

// Keys returns a new set - modifying it doesn't affect the map
$m3 = new Ds\Map(["x" => 1]);
$k = $m3->keys();
$k->add("y");
echo "map still 1: " . $m3->count() . "\n";

// Mixed key types
$m4 = new Ds\Map();
$m4->put("str", 1);
$m4->put(42, 2);
$m4->put(null, 3);
$keys4 = $m4->keys();
echo "mixed keys count: " . $keys4->count() . "\n";
?>
--EXPECT--
type: Ds\Set
count: 3
contains a: true
contains b: true
contains c: true
contains z: false
int keys count: 2
contains 10: true
empty keys count: 0
map still 1: 1
mixed keys count: 3
