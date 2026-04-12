--TEST--
Set: multiple clones share data, each separates independently
--FILE--
<?php
$a = new Ds\Set([1]);
$b = clone $a;
$c = clone $a;
$d = clone $b;

$b->add(2);
$c->add(3);
$d->add(4);

echo "a: " . implode(",", $a->toArray()) . "\n";
echo "b: " . implode(",", $b->toArray()) . "\n";
echo "c: " . implode(",", $c->toArray()) . "\n";
echo "d: " . implode(",", $d->toArray()) . "\n";
?>
--EXPECT--
a: 1
b: 1,2
c: 1,3
d: 1,4
