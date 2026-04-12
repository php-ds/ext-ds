--TEST--
Map: sort on clone does not affect original
--FILE--
<?php
$a = new Ds\Map();
$a->put("b", 3);
$a->put("a", 1);
$a->put("c", 2);

$b = clone $a;
$b->sort();

echo "a values: " . implode(",", $a->values()->toArray()) . "\n";
echo "b values: " . implode(",", $b->values()->toArray()) . "\n";
?>
--EXPECT--
a values: 3,1,2
b values: 1,2,3
