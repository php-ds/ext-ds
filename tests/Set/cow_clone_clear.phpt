--TEST--
Set: clear on clone does not affect original
--FILE--
<?php
$a = new Ds\Set([1, 2, 3]);
$b = clone $a;
$b->clear();

echo "a: " . $a->count() . "\n";
echo "b: " . $b->count() . "\n";
echo "a: " . implode(",", $a->toArray()) . "\n";
?>
--EXPECT--
a: 3
b: 0
a: 1,2,3
