--TEST--
Map::toArray - convert to associative PHP array
--FILE--
<?php
// String keys
$m = new Ds\Map(["a" => 1, "b" => 2, "c" => 3]);
$arr = $m->toArray();
echo "type: " . gettype($arr) . "\n";
echo "count: " . count($arr) . "\n";
echo "a=" . $arr["a"] . " b=" . $arr["b"] . " c=" . $arr["c"] . "\n";

// Integer keys
$m2 = new Ds\Map([10 => "x", 20 => "y"]);
$arr2 = $m2->toArray();
echo "int: " . $arr2[10] . "," . $arr2[20] . "\n";

// Empty map
$empty = new Ds\Map();
$arr3 = $empty->toArray();
echo "empty: " . count($arr3) . "\n";
echo "empty is array: " . var_export(is_array($arr3), true) . "\n";

// Preserves value types
$m3 = new Ds\Map(["int" => 1, "float" => 1.5, "str" => "hello", "bool" => true, "null" => null]);
$arr4 = $m3->toArray();
echo "int type: " . gettype($arr4["int"]) . "\n";
echo "float type: " . gettype($arr4["float"]) . "\n";
echo "str type: " . gettype($arr4["str"]) . "\n";
echo "bool type: " . gettype($arr4["bool"]) . "\n";
echo "null type: " . gettype($arr4["null"]) . "\n";

// Nested array values
$m4 = new Ds\Map(["arr" => [1, 2, 3]]);
$arr5 = $m4->toArray();
echo "nested: " . implode(",", $arr5["arr"]) . "\n";

// Returns a copy - modifying array doesn't affect map
$m5 = new Ds\Map(["a" => 1]);
$arr6 = $m5->toArray();
$arr6["a"] = 999;
echo "map unchanged: " . $m5->get("a") . "\n";
?>
--EXPECT--
type: array
count: 3
a=1 b=2 c=3
int: x,y
empty: 0
empty is array: true
int type: integer
float type: double
str type: string
bool type: boolean
null type: NULL
nested: 1,2,3
map unchanged: 1
