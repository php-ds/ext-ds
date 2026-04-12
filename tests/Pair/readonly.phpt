--TEST--
Pair: readonly properties and basic operations
--FILE--
<?php
$p = new Ds\Pair("hello", 42);
echo "key: " . $p->key . "\n";
echo "value: " . $p->value . "\n";

// Clone (readonly class, clone is the way to copy)
$p2 = clone $p;
echo "clone key: " . $p2->key . "\n";
echo "clone value: " . $p2->value . "\n";

// toArray
$arr = $p->toArray();
echo "array: " . $arr['key'] . "=" . $arr['value'] . "\n";

// JSON
echo "json: " . json_encode($p) . "\n";

// Trying to set should throw
try {
    $p->key = "world";
    echo "ERROR: should have thrown\n";
} catch (\Error $e) {
    echo "readonly: yes\n";
}
?>
--EXPECT--
key: hello
value: 42
clone key: hello
clone value: 42
array: hello=42
json: {"key":"hello","value":42}
readonly: yes
