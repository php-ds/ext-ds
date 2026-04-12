--TEST--
Nested: Seq containing Seq instances
--FILE--
<?php
$inner1 = new Ds\Seq([1, 2]);
$inner2 = new Ds\Seq([3, 4]);
$outer = new Ds\Seq([$inner1, $inner2]);

echo "outer count: " . $outer->count() . "\n";
echo "inner1: " . implode(",", $outer->get(0)->toArray()) . "\n";
echo "inner2: " . implode(",", $outer->get(1)->toArray()) . "\n";

// Modifying inner1 directly should affect what outer sees (same reference)
$inner1->push(99);
echo "after push to inner1: " . implode(",", $outer->get(0)->toArray()) . "\n";
?>
--EXPECT--
outer count: 2
inner1: 1,2
inner2: 3,4
after push to inner1: 1,2,99
