--TEST--
Map: offsetSet on clone does not affect original
--FILE--
<?php
$a = new Ds\Map();
$a->put("a", 1);

$b = clone $a;
$b["b"] = 2;
$b["a"] = 99;

echo "a count: " . $a->count() . "\n";
echo "a[a]: " . $a->get("a") . "\n";
echo "b count: " . $b->count() . "\n";
echo "b[a]: " . $b->get("a") . "\n";
?>
--EXPECT--
a count: 1
a[a]: 1
b count: 2
b[a]: 99
