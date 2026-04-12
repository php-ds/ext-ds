--TEST--
Heap: copy() shares data (COW), mutations are isolated
--FILE--
<?php
$a = new Ds\Heap();
$a->push(1, 2, 3);

$b = $a->copy();
$b->push(10);

echo "a count: " . $a->count() . "\n";
echo "b count: " . $b->count() . "\n";
echo "a peek: " . $a->peek() . "\n";
echo "b peek: " . $b->peek() . "\n";
?>
--EXPECT--
a count: 3
b count: 4
a peek: 3
b peek: 10
