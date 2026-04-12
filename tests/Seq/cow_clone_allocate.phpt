--TEST--
Seq: allocate on clone does not affect original capacity
--FILE--
<?php
$a = new Ds\Seq([1, 2, 3]);
$origCap = $a->capacity();

$b = clone $a;
$b->allocate(1000);

echo "a capacity unchanged: " . var_export($a->capacity() === $origCap, true) . "\n";
echo "b capacity >= 1000: " . var_export($b->capacity() >= 1000, true) . "\n";
echo "a data intact: " . implode(",", $a->toArray()) . "\n";
?>
--EXPECT--
a capacity unchanged: true
b capacity >= 1000: true
a data intact: 1,2,3
