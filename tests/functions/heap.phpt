--TEST--
\Ds\heap() functional constructor
--FILE--
<?php
// Empty max-heap (default)
$h = \Ds\heap();
echo "empty: " . $h->count() . "\n";
var_dump($h instanceof Ds\Heap);

// Max-heap from array
$h = \Ds\heap([3, 1, 4, 1, 5]);
echo "count: " . $h->count() . "\n";
echo "peek: " . $h->peek() . "\n";

$vals = [];
while ($h->count() > 0) $vals[] = $h->pop();
echo "max order: " . implode(",", $vals) . "\n";

// Min-heap with comparator
$h = \Ds\heap([3, 1, 4, 1, 5], function($a, $b) { return $b <=> $a; });
echo "min peek: " . $h->peek() . "\n";

$vals = [];
while ($h->count() > 0) $vals[] = $h->pop();
echo "min order: " . implode(",", $vals) . "\n";

// From generator
function gen() { yield 100; yield 50; yield 75; }
$h = \Ds\heap(gen());
echo "gen peek: " . $h->peek() . "\n";
?>
--EXPECT--
empty: 0
bool(true)
count: 5
peek: 5
max order: 5,4,3,1,1
min peek: 1
min order: 1,1,3,4,5
gen peek: 100
