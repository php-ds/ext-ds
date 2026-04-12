--TEST--
Heap: multiple clones share data, each separates independently
--FILE--
<?php
$a = new Ds\Heap();
$a->push(1, 2, 3);

$b = clone $a;
$c = clone $a;

$b->push(10);
$c->pop();

echo "a count: " . $a->count() . "\n";
echo "b count: " . $b->count() . "\n";
echo "c count: " . $c->count() . "\n";
echo "a peek: " . $a->peek() . "\n";
echo "b peek: " . $b->peek() . "\n";
echo "c peek: " . $c->peek() . "\n";
?>
--EXPECT--
a count: 3
b count: 4
c count: 2
a peek: 3
b peek: 10
c peek: 2
