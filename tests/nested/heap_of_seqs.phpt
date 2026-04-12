--TEST--
Nested: Heap containing Seq values with custom comparator
--FILE--
<?php
// Heap that compares Seq objects by their sum
$h = new Ds\Heap([], function($a, $b) {
    return array_sum($a->toArray()) <=> array_sum($b->toArray());
});

$h->push(new Ds\Seq([1, 2]));       // sum = 3
$h->push(new Ds\Seq([10, 20]));     // sum = 30
$h->push(new Ds\Seq([5]));          // sum = 5

echo "count: " . $h->count() . "\n";
echo "peek sum: " . array_sum($h->peek()->toArray()) . "\n";

$sums = [];
while ($h->count() > 0) {
    $sums[] = array_sum($h->pop()->toArray());
}
echo "order: " . implode(",", $sums) . "\n";
?>
--EXPECT--
count: 3
peek sum: 30
order: 30,5,3
