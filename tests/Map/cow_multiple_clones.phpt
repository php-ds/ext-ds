--TEST--
Map: multiple clones share data, each separates independently
--FILE--
<?php
$a = new Ds\Map();
$a->put("a", 1);

$b = clone $a;
$c = clone $a;
$d = clone $b;

$b->put("b", 2);
$c->put("c", 3);
$d->put("d", 4);

echo "a: " . $a->count() . "\n";
echo "b: " . $b->count() . "\n";
echo "c: " . $c->count() . "\n";
echo "d: " . $d->count() . "\n";
?>
--EXPECT--
a: 1
b: 2
c: 2
d: 2
