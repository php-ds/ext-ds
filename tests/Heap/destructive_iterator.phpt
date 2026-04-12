--TEST--
Heap: iteration is destructive on a clone, original unchanged
--FILE--
<?php
// Basic: foreach yields elements in heap order
$h = new Ds\Heap([3, 1, 4, 1, 5, 9]);
$result = [];
foreach ($h as $value) {
    $result[] = $value;
}
echo "iterated: " . implode(",", $result) . "\n";

// Original is unchanged after iteration
echo "count after foreach: " . $h->count() . "\n";
echo "peek after foreach: " . $h->peek() . "\n";

// Multiple iterations yield same result
$result2 = [];
foreach ($h as $value) {
    $result2[] = $value;
}
echo "second iteration: " . implode(",", $result2) . "\n";
echo "count still: " . $h->count() . "\n";

// Modifying heap during iteration is safe (iterates a clone)
$h2 = new Ds\Heap([10, 20, 30]);
$result3 = [];
foreach ($h2 as $value) {
    $result3[] = $value;
    if ($value === 30) {
        $h2->push(100); // push to original during iteration
    }
}
echo "during push: " . implode(",", $result3) . "\n";
echo "original after: " . $h2->count() . "\n";
echo "original peek: " . $h2->peek() . "\n";

// Iteration of empty heap
$h3 = new Ds\Heap();
$count = 0;
foreach ($h3 as $value) {
    $count++;
}
echo "empty iteration count: $count\n";

// Iteration order matches pop order
$h4 = new Ds\Heap([7, 2, 9, 4, 6]);
$iterResult = [];
foreach ($h4 as $v) {
    $iterResult[] = $v;
}
$popResult = [];
$h4copy = $h4->copy();
while ($h4copy->count() > 0) {
    $popResult[] = $h4copy->pop();
}
echo "iter matches pop: " . var_export($iterResult === $popResult, true) . "\n";

// Min-heap iteration
$h5 = new Ds\Heap([5, 3, 8, 1], fn($a, $b) => $b <=> $a);
$result5 = [];
foreach ($h5 as $value) {
    $result5[] = $value;
}
echo "min-heap iterated: " . implode(",", $result5) . "\n";
echo "min-heap count after: " . $h5->count() . "\n";

// Iterator keys are sequential
$h6 = new Ds\Heap([30, 10, 20]);
$keys = [];
foreach ($h6 as $key => $value) {
    $keys[] = $key;
}
echo "keys: " . implode(",", $keys) . "\n";
?>
--EXPECT--
iterated: 9,5,4,3,1,1
count after foreach: 6
peek after foreach: 9
second iteration: 9,5,4,3,1,1
count still: 6
during push: 30,20,10
original after: 4
original peek: 100
empty iteration count: 0
iter matches pop: true
min-heap iterated: 1,3,5,8
min-heap count after: 4
keys: 0,1,2
