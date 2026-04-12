--TEST--
Heap: clone preserves custom comparator and COW isolation
--FILE--
<?php
// Min-heap via reversed comparator
$a = new Ds\Heap([], function($x, $y) { return $y <=> $x; });
$a->push(3, 1, 2);

$b = clone $a;
$b->push(0);

echo "a peek: " . $a->peek() . "\n"; // min-heap: 1
echo "b peek: " . $b->peek() . "\n"; // min-heap: 0
echo "a count: " . $a->count() . "\n";
echo "b count: " . $b->count() . "\n";

// Pop all from a to verify order
$aVals = [];
while ($a->count() > 0) $aVals[] = $a->pop();
echo "a popped: " . implode(",", $aVals) . "\n";

// Pop all from b to verify order
$bVals = [];
while ($b->count() > 0) $bVals[] = $b->pop();
echo "b popped: " . implode(",", $bVals) . "\n";
?>
--EXPECT--
a peek: 1
b peek: 0
a count: 3
b count: 4
a popped: 1,2,3
b popped: 0,1,2,3
