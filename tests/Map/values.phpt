--TEST--
Map::values - get all values as a Seq
--FILE--
<?php
$m = new Ds\Map(["a" => 1, "b" => 2, "c" => 3]);
$vals = $m->values();

echo "type: " . get_class($vals) . "\n";
echo "count: " . $vals->count() . "\n";

// Values in insertion order
echo "val 0: " . $vals->get(0) . "\n";
echo "val 1: " . $vals->get(1) . "\n";
echo "val 2: " . $vals->get(2) . "\n";

// Empty map
$empty = new Ds\Map();
$ev = $empty->values();
echo "empty: " . $ev->count() . "\n";

// Values with mixed types
$m2 = new Ds\Map(["int" => 1, "str" => "hello", "null" => null, "bool" => true]);
$v2 = $m2->values();
echo "mixed count: " . $v2->count() . "\n";

// Values is independent copy
$m3 = new Ds\Map(["x" => 1]);
$v3 = $m3->values();
$v3->push(99);
echo "map count: " . $m3->count() . "\n";
echo "seq count: " . $v3->count() . "\n";

// Duplicate values are preserved
$m4 = new Ds\Map(["a" => 1, "b" => 1, "c" => 1]);
$v4 = $m4->values();
echo "dup values count: " . $v4->count() . "\n";
echo "all ones: " . $v4->get(0) . "," . $v4->get(1) . "," . $v4->get(2) . "\n";
?>
--EXPECT--
type: Ds\Seq
count: 3
val 0: 1
val 1: 2
val 2: 3
empty: 0
mixed count: 4
map count: 1
seq count: 2
dup values count: 3
all ones: 1,1,1
