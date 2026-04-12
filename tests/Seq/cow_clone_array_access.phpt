--TEST--
Seq: array access write/unset on clone does not affect original
--FILE--
<?php
$a = new Ds\Seq([1, 2, 3]);
$b = clone $a;

// Write via array access
$b[0] = 99;
$b[] = 4;

echo "a: " . implode(",", $a->toArray()) . "\n";
echo "b: " . implode(",", $b->toArray()) . "\n";

// Unset via array access
$c = clone $a;
unset($c[0]);

echo "a after unset on c: " . implode(",", $a->toArray()) . "\n";
echo "c: " . implode(",", $c->toArray()) . "\n";
?>
--EXPECT--
a: 1,2,3
b: 99,2,3,4
a after unset on c: 1,2,3
c: 2,3
