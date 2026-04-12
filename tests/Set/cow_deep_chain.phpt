--TEST--
Set: deep clone chain — c = clone(clone(a)), all independent
--FILE--
<?php
$a = new Ds\Set([1, 2]);
$b = clone $a;
$c = clone $b;

$a->add(10);
$b->add(20);
$c->add(30);

echo "a: " . implode(",", $a->toArray()) . "\n";
echo "b: " . implode(",", $b->toArray()) . "\n";
echo "c: " . implode(",", $c->toArray()) . "\n";
?>
--EXPECT--
a: 1,2,10
b: 1,2,20
c: 1,2,30
