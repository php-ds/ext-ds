--TEST--
Seq: deep clone chain — c = clone(clone(a)), all independent
--FILE--
<?php
$a = new Ds\Seq([1, 2, 3]);
$b = clone $a;
$c = clone $b;

$a->push(4);
$b->push(5);
$c->push(6);

echo "a: " . implode(",", $a->toArray()) . "\n";
echo "b: " . implode(",", $b->toArray()) . "\n";
echo "c: " . implode(",", $c->toArray()) . "\n";
?>
--EXPECT--
a: 1,2,3,4
b: 1,2,3,5
c: 1,2,3,6
