--TEST--
Pair: serialize and unserialize
--FILE--
<?php
$p = new Ds\Pair("key", [1, 2, 3]);
$s = serialize($p);
$p2 = unserialize($s);

echo "key: " . $p2->key . "\n";
echo "value: " . implode(",", $p2->value) . "\n";
echo "equal: " . var_export($p->key === $p2->key && $p->value === $p2->value, true) . "\n";
?>
--EXPECT--
key: key
value: 1,2,3
equal: true
