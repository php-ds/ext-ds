--TEST--
Heap::json_encode - encodes heap as JSON array in heap order
--FILE--
<?php
// Empty heap
$h = new Ds\Heap();
echo json_encode($h) . "\n";

// Integers
$h = new Ds\Heap([3, 1, 4, 1, 5]);
echo json_encode($h) . "\n";

// Strings
$h2 = new Ds\Heap(["banana", "apple", "cherry"]);
echo json_encode($h2) . "\n";

// Single element
$h3 = new Ds\Heap([42]);
echo json_encode($h3) . "\n";

// Mixed types (floats)
$h4 = new Ds\Heap([1.5, 3.7, 2.1]);
echo json_encode($h4) . "\n";

// json_encode does not consume the heap
$h5 = new Ds\Heap([10, 20, 30]);
json_encode($h5);
echo "count after json: " . $h5->count() . "\n";

// Min-heap
$h6 = new Ds\Heap([3, 1, 4], fn($a, $b) => $b <=> $a);
echo json_encode($h6) . "\n";
?>
--EXPECT--
[]
[5,4,3,1,1]
["cherry","banana","apple"]
[42]
[3.7,2.1,1.5]
count after json: 3
[1,3,4]
