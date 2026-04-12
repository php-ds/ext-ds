--TEST--
Heap::pop - removes and returns max element, maintains heap property
--FILE--
<?php
// Pop on empty throws UnderflowException
$h = new Ds\Heap();
try {
    $h->pop();
    echo "ERROR: should have thrown\n";
} catch (\UnderflowException $e) {
    echo "empty pop throws: yes\n";
}

// Pop returns elements in descending order (max-heap)
$h = new Ds\Heap([3, 1, 4, 1, 5, 9, 2, 6]);
$result = [];
while ($h->count() > 0) {
    $result[] = $h->pop();
}
echo "popped: " . implode(",", $result) . "\n";

// Count decreases after pop
$h2 = new Ds\Heap([10, 20, 30]);
echo "before: " . $h2->count() . "\n";
$val = $h2->pop();
echo "popped: $val\n";
echo "after: " . $h2->count() . "\n";

// Pop with duplicates
$h3 = new Ds\Heap([3, 3, 3, 1, 1]);
$result2 = [];
while ($h3->count() > 0) {
    $result2[] = $h3->pop();
}
echo "duplicates: " . implode(",", $result2) . "\n";

// Pop single element
$h4 = new Ds\Heap([42]);
echo "single pop: " . $h4->pop() . "\n";
echo "now empty: " . var_export($h4->isEmpty(), true) . "\n";

// Pop after push maintains heap order
$h5 = new Ds\Heap();
$h5->push(1);
$h5->push(10);
$h5->push(5);
echo "first pop: " . $h5->pop() . "\n";
$h5->push(8);
echo "second pop: " . $h5->pop() . "\n";
echo "third pop: " . $h5->pop() . "\n";
echo "fourth pop: " . $h5->pop() . "\n";

// Pop from empty after popping all throws
try {
    $h5->pop();
    echo "ERROR: should have thrown\n";
} catch (\UnderflowException $e) {
    echo "exhausted pop throws: yes\n";
}

// Pop with negative numbers
$h6 = new Ds\Heap([-5, -1, -10, -3]);
$result3 = [];
while ($h6->count() > 0) {
    $result3[] = $h6->pop();
}
echo "negatives: " . implode(",", $result3) . "\n";

// Pop with strings
$h7 = new Ds\Heap(["banana", "apple", "cherry", "date"]);
$result4 = [];
while ($h7->count() > 0) {
    $result4[] = $h7->pop();
}
echo "strings: " . implode(",", $result4) . "\n";
?>
--EXPECT--
empty pop throws: yes
popped: 9,6,5,4,3,2,1,1
before: 3
popped: 30
after: 2
duplicates: 3,3,3,1,1
single pop: 42
now empty: true
first pop: 10
second pop: 8
third pop: 5
fourth pop: 1
exhausted pop throws: yes
negatives: -1,-3,-5,-10
strings: date,cherry,banana,apple
