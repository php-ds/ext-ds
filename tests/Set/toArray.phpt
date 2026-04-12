--TEST--
Set::toArray - converts set to indexed array
--FILE--
<?php
$s = new Ds\Set([1, 2, 3]);
$arr = $s->toArray();
echo "type: " . gettype($arr) . "\n";
echo "values: " . implode(",", $arr) . "\n";

// Keys are sequential integers starting at 0
echo "keys: " . implode(",", array_keys($arr)) . "\n";

// Empty set
$empty = new Ds\Set();
$arr2 = $empty->toArray();
echo "empty count: " . count($arr2) . "\n";
echo "empty is array: " . var_export(is_array($arr2), true) . "\n";

// Preserves insertion order
$s2 = new Ds\Set();
$s2->add("c");
$s2->add("a");
$s2->add("b");
$arr3 = $s2->toArray();
echo "order: " . implode(",", $arr3) . "\n";

// Mixed types
$s3 = new Ds\Set([1, "two", 3.14, true, null]);
$arr4 = $s3->toArray();
echo "count: " . count($arr4) . "\n";
echo "first: " . var_export($arr4[0], true) . "\n";
echo "last: " . var_export($arr4[4], true) . "\n";

// Modifying the array doesn't affect the set
$s4 = new Ds\Set([1, 2, 3]);
$arr5 = $s4->toArray();
$arr5[] = 4;
echo "set after array mod: " . $s4->count() . "\n";
?>
--EXPECT--
type: array
values: 1,2,3
keys: 0,1,2
empty count: 0
empty is array: true
order: c,a,b
count: 5
first: 1
last: NULL
set after array mod: 3
