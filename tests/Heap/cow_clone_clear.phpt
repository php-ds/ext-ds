--TEST--
Heap: clear on clone does not affect original
--FILE--
<?php
$a = new Ds\Heap();
$a->push(1, 2, 3);

$b = clone $a;
$b->clear();

echo "a count: " . $a->count() . "\n";
echo "b count: " . $b->count() . "\n";
echo "a peek: " . $a->peek() . "\n";
?>
--EXPECT--
a count: 3
b count: 0
a peek: 3
