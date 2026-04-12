--TEST--
Heap: constructor accepts iterable values and optional comparator
--FILE--
<?php
// Default max-heap with array values
$h = new Ds\Heap([3, 1, 4, 1, 5]);
echo "count: " . $h->count() . "\n";
echo "peek: " . $h->peek() . "\n";

$vals = [];
while ($h->count() > 0) $vals[] = $h->pop();
echo "popped: " . implode(",", $vals) . "\n";

// Min-heap with comparator
$h2 = new Ds\Heap([3, 1, 4, 1, 5], function($a, $b) { return $b <=> $a; });
echo "min peek: " . $h2->peek() . "\n";

$vals2 = [];
while ($h2->count() > 0) $vals2[] = $h2->pop();
echo "min popped: " . implode(",", $vals2) . "\n";

// Empty constructor still works
$h3 = new Ds\Heap();
echo "empty count: " . $h3->count() . "\n";

// Values from traversable (generator)
function gen() { yield 10; yield 20; yield 30; }
$h4 = new Ds\Heap(gen());
echo "gen peek: " . $h4->peek() . "\n";
echo "gen count: " . $h4->count() . "\n";
?>
--EXPECT--
count: 5
peek: 5
popped: 5,4,3,1,1
min peek: 1
min popped: 1,1,3,4,5
empty count: 0
gen peek: 30
gen count: 3
