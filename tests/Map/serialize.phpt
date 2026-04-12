--TEST--
Map::serialize/unserialize - round-trip serialization
--FILE--
<?php
// Basic round-trip
$m = new Ds\Map(["a" => 1, "b" => 2, "c" => 3]);
$s = serialize($m);
$m2 = unserialize($s);

echo "type: " . get_class($m2) . "\n";
echo "count: " . $m2->count() . "\n";
echo "a=" . $m2->get("a") . " b=" . $m2->get("b") . " c=" . $m2->get("c") . "\n";

// Integer keys
$m3 = new Ds\Map([10 => "x", 20 => "y"]);
$m4 = unserialize(serialize($m3));
echo "int keys: " . $m4->get(10) . "," . $m4->get(20) . "\n";

// Mixed value types
$m5 = new Ds\Map(["int" => 42, "str" => "hello", "bool" => true, "null" => null, "float" => 3.14]);
$m6 = unserialize(serialize($m5));
echo "int: " . $m6->get("int") . "\n";
echo "str: " . $m6->get("str") . "\n";
echo "bool: " . var_export($m6->get("bool"), true) . "\n";
echo "null: " . var_export($m6->get("null"), true) . "\n";
echo "float: " . $m6->get("float") . "\n";

// Empty map
$empty = new Ds\Map();
$empty2 = unserialize(serialize($empty));
echo "empty: " . $empty2->count() . "\n";

// Array values
$m7 = new Ds\Map(["arr" => [1, 2, 3]]);
$m8 = unserialize(serialize($m7));
echo "array: " . implode(",", $m8->get("arr")) . "\n";

// Serialized map is independent
$m9 = new Ds\Map(["a" => 1]);
$m10 = unserialize(serialize($m9));
$m10->put("b", 2);
echo "orig: " . $m9->count() . "\n";
echo "copy: " . $m10->count() . "\n";

// Preserves order
$m11 = new Ds\Map();
$m11->put("c", 3);
$m11->put("a", 1);
$m11->put("b", 2);
$m12 = unserialize(serialize($m11));
$order = [];
foreach ($m12 as $k => $v) { $order[] = $k; }
echo "order: " . implode(",", $order) . "\n";
?>
--EXPECT--
type: Ds\Map
count: 3
a=1 b=2 c=3
int keys: x,y
int: 42
str: hello
bool: true
null: NULL
float: 3.14
empty: 0
array: 1,2,3
orig: 1
copy: 2
order: c,a,b
