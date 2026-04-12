--TEST--
Pair::toArray - returns associative array with key and value
--FILE--
<?php
// Basic toArray
$p = new Ds\Pair("hello", 42);
$arr = $p->toArray();
echo "keys: " . implode(",", array_keys($arr)) . "\n";
echo "key: " . $arr['key'] . "\n";
echo "value: " . $arr['value'] . "\n";

// toArray with null values
$p2 = new Ds\Pair(null, null);
$arr2 = $p2->toArray();
echo "null key: " . var_export($arr2['key'], true) . "\n";
echo "null value: " . var_export($arr2['value'], true) . "\n";

// toArray with array value
$p3 = new Ds\Pair("data", [1, 2, 3]);
$arr3 = $p3->toArray();
echo "array value: " . implode(",", $arr3['value']) . "\n";

// toArray with complex types
$obj = new stdClass();
$obj->x = 10;
$p4 = new Ds\Pair("obj", $obj);
$arr4 = $p4->toArray();
echo "object in array: " . $arr4['value']->x . "\n";

// toArray result is a plain array
$p5 = new Ds\Pair(1, 2);
$arr5 = $p5->toArray();
echo "is array: " . var_export(is_array($arr5), true) . "\n";
echo "count: " . count($arr5) . "\n";

// toArray with boolean
$p6 = new Ds\Pair(true, false);
$arr6 = $p6->toArray();
echo "bool key: " . var_export($arr6['key'], true) . "\n";
echo "bool value: " . var_export($arr6['value'], true) . "\n";
?>
--EXPECT--
keys: key,value
key: hello
value: 42
null key: NULL
null value: NULL
array value: 1,2,3
object in array: 10
is array: true
count: 2
bool key: true
bool value: false
