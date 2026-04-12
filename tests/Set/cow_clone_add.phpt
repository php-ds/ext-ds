--TEST--
Set: add on clone does not affect original
--FILE--
<?php
$a = new Ds\Set([1, 2]);
$b = clone $a;

$b->add(3, 4, 5);

echo "a: " . $a->count() . "\n";
echo "b: " . $b->count() . "\n";
echo "a contains 3: " . var_export($a->contains(3), true) . "\n";
echo "b contains 3: " . var_export($b->contains(3), true) . "\n";
?>
--EXPECT--
a: 2
b: 5
a contains 3: false
b contains 3: true
