--TEST--
Map::hasValue - check if value exists (strict comparison)
--FILE--
<?php
$m = new Ds\Map(["a" => 1, "b" => 2, "c" => 3]);

echo "has 1: " . var_export($m->hasValue(1), true) . "\n";
echo "has 99: " . var_export($m->hasValue(99), true) . "\n";

// Strict comparison - "1" is not 1
echo "has '1': " . var_export($m->hasValue("1"), true) . "\n";

// Null value
$m->put("n", null);
echo "has null: " . var_export($m->hasValue(null), true) . "\n";

// Boolean values
$m->put("t", true);
$m->put("f", false);
echo "has true: " . var_export($m->hasValue(true), true) . "\n";
echo "has false: " . var_export($m->hasValue(false), true) . "\n";

// Array value
$m->put("arr", [1, 2, 3]);
echo "has array: " . var_export($m->hasValue([1, 2, 3]), true) . "\n";
echo "has different array: " . var_export($m->hasValue([1, 2]), true) . "\n";

// Object value
$obj = new stdClass();
$obj->x = 1;
$m->put("obj", $obj);
echo "has same obj: " . var_export($m->hasValue($obj), true) . "\n";

// Empty map
$empty = new Ds\Map();
echo "empty has 1: " . var_export($empty->hasValue(1), true) . "\n";

// Float value
$m2 = new Ds\Map(["pi" => 3.14]);
echo "has 3.14: " . var_export($m2->hasValue(3.14), true) . "\n";
echo "has 3.15: " . var_export($m2->hasValue(3.15), true) . "\n";

// String value
$m3 = new Ds\Map(["greeting" => "hello"]);
echo "has hello: " . var_export($m3->hasValue("hello"), true) . "\n";
echo "has world: " . var_export($m3->hasValue("world"), true) . "\n";
?>
--EXPECT--
has 1: true
has 99: false
has '1': false
has null: true
has true: true
has false: true
has array: true
has different array: false
has same obj: true
empty has 1: false
has 3.14: true
has 3.15: false
has hello: true
has world: false
