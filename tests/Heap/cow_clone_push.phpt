--TEST--
Heap: push on clone does not affect original
--FILE--
<?php
$a = new Ds\Heap();
$a->push(1, 2, 3);

$b = clone $a;
$b->push(10);

echo "a: " . $a->count() . "\n";
echo "b: " . $b->count() . "\n";
echo "a peek: " . $a->peek() . "\n";
echo "b peek: " . $b->peek() . "\n";
?>
--EXPECT--
a: 3
b: 4
a peek: 3
b peek: 10
