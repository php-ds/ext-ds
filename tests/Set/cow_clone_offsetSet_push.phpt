--TEST--
Set: offsetSet (push via []) on clone does not affect original
--FILE--
<?php
$a = new Ds\Set([1, 2, 3]);
$b = clone $a;

$b[] = 4;

echo "a: " . implode(",", $a->toArray()) . "\n";
echo "b: " . implode(",", $b->toArray()) . "\n";
?>
--EXPECT--
a: 1,2,3
b: 1,2,3,4
