--TEST--
Set: copy() shares data (COW), mutations are isolated
--FILE--
<?php
$a = new Ds\Set([1, 2, 3]);
$b = $a->copy();

$b->add(4);
$a->add(5);

echo "a: " . implode(",", $a->toArray()) . "\n";
echo "b: " . implode(",", $b->toArray()) . "\n";
?>
--EXPECT--
a: 1,2,3,5
b: 1,2,3,4
