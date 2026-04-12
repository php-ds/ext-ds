--TEST--
Pair::json_encode - encodes as {"key":...,"value":...}
--FILE--
<?php
// Basic JSON encoding
$p = new Ds\Pair("hello", 42);
echo json_encode($p) . "\n";

// With null values
$p2 = new Ds\Pair(null, null);
echo json_encode($p2) . "\n";

// With array value
$p3 = new Ds\Pair("data", [1, 2, 3]);
echo json_encode($p3) . "\n";

// With nested object
$obj = new stdClass();
$obj->name = "test";
$p4 = new Ds\Pair("obj", $obj);
echo json_encode($p4) . "\n";

// With boolean values
$p5 = new Ds\Pair(true, false);
echo json_encode($p5) . "\n";

// With integer key
$p6 = new Ds\Pair(1, "one");
echo json_encode($p6) . "\n";

// With float values
$p7 = new Ds\Pair(3.14, 2.71);
echo json_encode($p7) . "\n";

// With empty string
$p8 = new Ds\Pair("", "");
echo json_encode($p8) . "\n";

// Nested pair
$inner = new Ds\Pair("a", "b");
$outer = new Ds\Pair("outer", $inner);
echo json_encode($outer) . "\n";
?>
--EXPECT--
{"key":"hello","value":42}
{"key":null,"value":null}
{"key":"data","value":[1,2,3]}
{"key":"obj","value":{"name":"test"}}
{"key":true,"value":false}
{"key":1,"value":"one"}
{"key":3.14,"value":2.71}
{"key":"","value":""}
{"key":"outer","value":{"key":"a","value":"b"}}
