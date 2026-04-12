--TEST--
Set: reverse on clone does not affect original
--FILE--
<?php
$a = new Ds\Set([1, 2, 3]);
$b = clone $a;
$b->reverse();

echo "a: " . implode(",", $a->toArray()) . "\n";
echo "b: " . implode(",", $b->toArray()) . "\n";
?>
--EXPECT--
a: 1,2,3
b: 3,2,1
