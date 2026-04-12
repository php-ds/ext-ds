--TEST--
Set::serialize/unserialize - round-trip serialization
--FILE--
<?php
// Basic round-trip
$s = new Ds\Set([1, 2, 3]);
$serialized = serialize($s);
$unserialized = unserialize($serialized);
echo "round-trip: " . $unserialized->join(",") . "\n";
echo "count: " . $unserialized->count() . "\n";
echo "type: " . get_class($unserialized) . "\n";

// Empty set
$empty = new Ds\Set();
$s2 = unserialize(serialize($empty));
echo "empty: " . $s2->count() . "\n";
echo "empty isEmpty: " . var_export($s2->isEmpty(), true) . "\n";

// Strings
$s3 = new Ds\Set(["hello", "world"]);
$s4 = unserialize(serialize($s3));
echo "strings: " . $s4->join(",") . "\n";

// Mixed types
$s5 = new Ds\Set([1, "two", 3.14, true, null]);
$s6 = unserialize(serialize($s5));
echo "mixed count: " . $s6->count() . "\n";
echo "contains 1: " . var_export($s6->contains(1), true) . "\n";
echo "contains 'two': " . var_export($s6->contains("two"), true) . "\n";
echo "contains null: " . var_export($s6->contains(null), true) . "\n";

// Unserialized is independent copy
$original = new Ds\Set([1, 2, 3]);
$copy = unserialize(serialize($original));
$copy->add(4);
echo "original after copy mod: " . $original->count() . "\n";
echo "copy after mod: " . $copy->count() . "\n";

// __serialize / __unserialize
$s7 = new Ds\Set([10, 20, 30]);
$data = $s7->__serialize();
echo "serialize data: " . implode(",", $data) . "\n";

$s8 = new Ds\Set();
$s8->__unserialize([40, 50, 60]);
echo "unserialize: " . $s8->join(",") . "\n";
?>
--EXPECT--
round-trip: 1,2,3
count: 3
type: Ds\Set
empty: 0
empty isEmpty: true
strings: hello,world
mixed count: 5
contains 1: true
contains 'two': true
contains null: true
original after copy mod: 3
copy after mod: 4
serialize data: 10,20,30
unserialize: 40,50,60
