--TEST--
Set: clone isolates — mutating clone does not affect original
--FILE--
<?php
$a = new Ds\Set([1, 2, 3]);
$b = clone $a;

$b->add(4, 5);
$b->remove(1);

echo "a: " . implode(",", $a->toArray()) . "\n";
echo "b: " . implode(",", $b->toArray()) . "\n";
echo "a count: " . $a->count() . "\n";
echo "b count: " . $b->count() . "\n";
?>
--EXPECT--
a: 1,2,3
b: 2,3,4,5
a count: 3
b count: 4
