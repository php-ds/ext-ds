--TEST--
Map: apply on clone does not affect original
--FILE--
<?php
$a = new Ds\Map();
$a->put("a", 1);
$a->put("b", 2);

$b = clone $a;
$b->apply(function($k, $v) { return $v * 10; });

echo "a[a]: " . $a->get("a") . "\n";
echo "a[b]: " . $a->get("b") . "\n";
echo "b[a]: " . $b->get("a") . "\n";
echo "b[b]: " . $b->get("b") . "\n";
?>
--EXPECT--
a[a]: 1
a[b]: 2
b[a]: 10
b[b]: 20
