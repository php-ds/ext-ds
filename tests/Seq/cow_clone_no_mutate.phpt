--TEST--
Seq: clone without mutation shares data (no unnecessary copy)
--FILE--
<?php
$a = new Ds\Seq([1, 2, 3]);
$b = clone $a;

// Both should have identical data without triggering a separation
echo "a: " . implode(",", $a->toArray()) . "\n";
echo "b: " . implode(",", $b->toArray()) . "\n";
echo "equal: " . var_export($a->toArray() === $b->toArray(), true) . "\n";
?>
--EXPECT--
a: 1,2,3
b: 1,2,3
equal: true
