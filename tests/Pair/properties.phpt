--TEST--
Pair: accessing key and value properties, readonly enforcement, complex types
--FILE--
<?php
// Access key and value
$p = new Ds\Pair("hello", 42);
echo "key: " . $p->key . "\n";
echo "value: " . $p->value . "\n";

// Readonly: cannot write to key
try {
    $p->key = "world";
    echo "ERROR: should have thrown\n";
} catch (\Error $e) {
    echo "key readonly: yes\n";
}

// Readonly: cannot write to value
try {
    $p->value = 99;
    echo "ERROR: should have thrown\n";
} catch (\Error $e) {
    echo "value readonly: yes\n";
}

// Key and value unchanged after failed writes
echo "key still: " . $p->key . "\n";
echo "value still: " . $p->value . "\n";

// Cannot unset properties
try {
    unset($p->key);
    echo "ERROR: should have thrown\n";
} catch (\Error $e) {
    echo "unset key throws: yes\n";
}

// Complex types: array key
$p2 = new Ds\Pair([1, 2], "array-key");
echo "array key: " . implode(",", $p2->key) . "\n";

// Complex types: object key
$obj = new stdClass();
$obj->id = 5;
$p3 = new Ds\Pair($obj, "object-key");
echo "object key id: " . $p3->key->id . "\n";

// Complex types: null
$p4 = new Ds\Pair(null, null);
echo "null key is null: " . var_export($p4->key === null, true) . "\n";
echo "null value is null: " . var_export($p4->value === null, true) . "\n";

// Complex types: nested array
$p5 = new Ds\Pair("nested", ["a" => [1, 2], "b" => [3, 4]]);
echo "nested value: " . $p5->value["a"][0] . "," . $p5->value["b"][1] . "\n";

// isset works on properties
echo "isset key: " . var_export(isset($p->key), true) . "\n";
echo "isset value: " . var_export(isset($p->value), true) . "\n";
echo "isset null key: " . var_export(isset($p4->key), true) . "\n";

// Property access on pair with zero/false values
$p6 = new Ds\Pair(0, false);
echo "zero key: " . var_export($p6->key, true) . "\n";
echo "false value: " . var_export($p6->value, true) . "\n";
?>
--EXPECT--
key: hello
value: 42
key readonly: yes
value readonly: yes
key still: hello
value still: 42
unset key throws: yes
array key: 1,2
object key id: 5
null key is null: true
null value is null: true
nested value: 1,4
isset key: true
isset value: true
isset null key: false
zero key: 0
false value: false