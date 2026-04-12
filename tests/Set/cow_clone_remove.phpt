--TEST--
Set: remove on clone does not affect original
--FILE--
<?php
$a = new Ds\Set([1, 2, 3, 4]);
$b = clone $a;

$b->remove(2, 3);

echo "a: " . implode(",", $a->toArray()) . "\n";
echo "b: " . implode(",", $b->toArray()) . "\n";
?>
--EXPECT--
a: 1,2,3,4
b: 1,4
