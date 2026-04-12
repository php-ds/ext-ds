--TEST--
Set::jsonSerialize - JSON encoding
--FILE--
<?php
// Basic encoding
$s = new Ds\Set([1, 2, 3]);
echo json_encode($s) . "\n";

// Strings
$s2 = new Ds\Set(["a", "b", "c"]);
echo json_encode($s2) . "\n";

// Mixed types
$s3 = new Ds\Set([1, "two", 3.14, true, null]);
echo json_encode($s3) . "\n";

// Empty set
$empty = new Ds\Set();
echo json_encode($empty) . "\n";

// Single element
$single = new Ds\Set([42]);
echo json_encode($single) . "\n";

// Nested structure
$s4 = new Ds\Set([[1, 2], [3, 4]]);
echo json_encode($s4) . "\n";

// Encodes as JSON array (not object)
$json = json_encode($s);
$decoded = json_decode($json, true);
echo "is array: " . var_export(array_is_list($decoded), true) . "\n";
?>
--EXPECT--
[1,2,3]
["a","b","c"]
[1,"two",3.14,true,null]
[]
[42]
[[1,2],[3,4]]
is array: true
