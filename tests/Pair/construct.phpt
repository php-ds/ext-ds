--TEST--
Pair::__construct - with values and various types
--FILE--
<?php
// With string key and int value
$p2 = new Ds\Pair("hello", 42);
echo "string/int: key=" . $p2->key . " value=" . $p2->value . "\n";

// With int key and string value
$p3 = new Ds\Pair(1, "world");
echo "int/string: key=" . $p3->key . " value=" . $p3->value . "\n";

// With null key and value
$p4 = new Ds\Pair(null, null);
echo "null/null: key=" . var_export($p4->key, true) . " value=" . var_export($p4->value, true) . "\n";

// With array value
$p5 = new Ds\Pair("data", [1, 2, 3]);
echo "array value: " . implode(",", $p5->value) . "\n";

// With object value
$obj = new stdClass();
$obj->name = "test";
$p6 = new Ds\Pair("obj", $obj);
echo "object value: " . $p6->value->name . "\n";

// With float key
$p7 = new Ds\Pair(3.14, "pi");
echo "float key: " . $p7->key . "\n";

// With bool key
$p8 = new Ds\Pair(true, "yes");
echo "bool key: " . var_export($p8->key, true) . "\n";

// Nested pair as value
$inner = new Ds\Pair("inner-key", "inner-value");
$outer = new Ds\Pair("outer", $inner);
echo "nested: " . $outer->value->key . "=" . $outer->value->value . "\n";

// With empty string
$p10 = new Ds\Pair("", "");
echo "empty strings: key='" . $p10->key . "' value='" . $p10->value . "'\n";

// With zero
$p11 = new Ds\Pair(0, 0);
echo "zeros: key=" . $p11->key . " value=" . $p11->value . "\n";

// Constructor requires exactly 2 args
try {
    new Ds\Pair();
} catch (\ArgumentCountError $e) {
    echo "no args: error\n";
}

try {
    new Ds\Pair("only-key");
} catch (\ArgumentCountError $e) {
    echo "one arg: error\n";
}
?>
--EXPECT--
string/int: key=hello value=42
int/string: key=1 value=world
null/null: key=NULL value=NULL
array value: 1,2,3
object value: test
float key: 3.14
bool key: true
nested: inner-key=inner-value
empty strings: key='' value=''
zeros: key=0 value=0
no args: error
one arg: error
