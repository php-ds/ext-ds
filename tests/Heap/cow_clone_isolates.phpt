--TEST--
Heap: clone isolates — mutating clone does not affect original
--FILE--
<?php
$a = new Ds\Heap();
$a->push(1, 2, 3);

$b = clone $a;
$b->push(4, 5);

echo "a count: " . $a->count() . "\n";
echo "b count: " . $b->count() . "\n";
echo "a peek: " . $a->peek() . "\n";
echo "b peek: " . $b->peek() . "\n";
?>
--EXPECT--
a count: 3
b count: 5
a peek: 3
b peek: 5
