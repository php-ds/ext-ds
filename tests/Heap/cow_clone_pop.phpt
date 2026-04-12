--TEST--
Heap: pop on clone does not affect original
--FILE--
<?php
$a = new Ds\Heap();
$a->push(1, 2, 3);

$b = clone $a;
$popped = $b->pop();

echo "popped: " . $popped . "\n";
echo "a count: " . $a->count() . "\n";
echo "b count: " . $b->count() . "\n";
echo "a peek: " . $a->peek() . "\n";
?>
--EXPECT--
popped: 3
a count: 3
b count: 2
a peek: 3
