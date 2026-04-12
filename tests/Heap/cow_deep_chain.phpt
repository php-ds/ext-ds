--TEST--
Heap: deep clone chain — c = clone(clone(a)), all independent
--FILE--
<?php
$a = new Ds\Heap([5, 3]);
$b = clone $a;
$c = clone $b;

$a->push(10);
$b->push(20);
$c->push(30);

echo "a peek: " . $a->peek() . "\n";
echo "b peek: " . $b->peek() . "\n";
echo "c peek: " . $c->peek() . "\n";
echo "a count: " . $a->count() . "\n";
echo "b count: " . $b->count() . "\n";
echo "c count: " . $c->count() . "\n";
?>
--EXPECT--
a peek: 10
b peek: 20
c peek: 30
a count: 3
b count: 3
c count: 3
