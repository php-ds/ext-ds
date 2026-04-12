--TEST--
Map: clear on clone does not affect original
--FILE--
<?php
$a = new Ds\Map();
$a->put("a", 1);
$a->put("b", 2);

$b = clone $a;
$b->clear();

echo "a: " . $a->count() . "\n";
echo "b: " . $b->count() . "\n";
echo "a[a]: " . $a->get("a") . "\n";
?>
--EXPECT--
a: 2
b: 0
a[a]: 1
