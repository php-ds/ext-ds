--TEST--
Heap::toArray - returns array in heap order (sorted descending for max-heap)
--FILE--
<?php
// toArray on empty heap
$h = new Ds\Heap();
var_dump($h->toArray());

// toArray returns descending order
$h = new Ds\Heap([3, 1, 4, 1, 5]);
$arr = $h->toArray();
echo "toArray: " . implode(",", $arr) . "\n";

// toArray does not consume the heap
echo "count after toArray: " . $h->count() . "\n";
echo "peek after toArray: " . $h->peek() . "\n";

// toArray is idempotent
$arr2 = $h->toArray();
echo "second toArray: " . implode(",", $arr2) . "\n";

// toArray with single element
$h2 = new Ds\Heap([42]);
echo "single: " . implode(",", $h2->toArray()) . "\n";

// toArray with duplicates
$h3 = new Ds\Heap([2, 2, 2, 1, 1]);
echo "duplicates: " . implode(",", $h3->toArray()) . "\n";

// toArray with strings
$h4 = new Ds\Heap(["banana", "apple", "cherry"]);
echo "strings: " . implode(",", $h4->toArray()) . "\n";

// toArray on min-heap returns ascending order
$h5 = new Ds\Heap([3, 1, 4, 1, 5], fn($a, $b) => $b <=> $a);
echo "min-heap: " . implode(",", $h5->toArray()) . "\n";

// toArray returns sequential integer keys
$h6 = new Ds\Heap([10, 30, 20]);
$arr3 = $h6->toArray();
echo "keys: " . implode(",", array_keys($arr3)) . "\n";
?>
--EXPECT--
array(0) {
}
toArray: 5,4,3,1,1
count after toArray: 5
peek after toArray: 5
second toArray: 5,4,3,1,1
single: 42
duplicates: 2,2,2,1,1
strings: cherry,banana,apple
min-heap: 1,1,3,4,5
keys: 0,1,2
