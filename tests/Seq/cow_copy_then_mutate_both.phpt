--TEST--
Seq: copy() then mutate both — fully independent
--FILE--
<?php
$a = new Ds\Seq([1, 2, 3]);
$b = $a->copy();

$a->set(0, 99);
$b->pop();

echo "a: " . implode(",", $a->toArray()) . "\n";
echo "b: " . implode(",", $b->toArray()) . "\n";
?>
--EXPECT--
a: 99,2,3
b: 1,2
