--TEST--
Set: sort on clone does not affect original
--FILE--
<?php
$a = new Ds\Set([3, 1, 2]);
$b = clone $a;
$b->sort();

echo "a: " . implode(",", $a->toArray()) . "\n";
echo "b: " . implode(",", $b->toArray()) . "\n";
?>
--EXPECT--
a: 3,1,2
b: 1,2,3
