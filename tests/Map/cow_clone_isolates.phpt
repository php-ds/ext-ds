--TEST--
Map: clone isolates — mutating clone does not affect original
--FILE--
<?php
$a = new Ds\Map();
$a->put("a", 1);
$a->put("b", 2);
$a->put("c", 3);

$b = clone $a;

$b->put("d", 4);
$b->put("a", 99);
$b->remove("b");

echo "a: " . $a->count() . "\n";
echo "b: " . $b->count() . "\n";
echo "a[a]: " . $a->get("a") . "\n";
echo "a[b]: " . $a->get("b") . "\n";
echo "b[a]: " . $b->get("a") . "\n";
echo "b has b: " . var_export($b->hasKey("b"), true) . "\n";
echo "b[d]: " . $b->get("d") . "\n";
?>
--EXPECT--
a: 3
b: 3
a[a]: 1
a[b]: 2
b[a]: 99
b has b: false
b[d]: 4
