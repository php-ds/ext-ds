--TEST--
Heap: custom comparator creates min-heap
--FILE--
<?php
// Min-heap: reverse comparison
$h = new Ds\Heap([], fn($a, $b) => $b <=> $a);
$h->push(5, 3, 8, 1, 9, 2);

echo "peek (min): " . $h->peek() . "\n";
echo "count: " . $h->count() . "\n";

// Pop order should be ascending
$result = [];
while ($h->count() > 0) {
    $result[] = $h->pop();
}
echo "popped: " . implode(",", $result) . "\n";

// Min-heap constructed with values
$h2 = new Ds\Heap([10, 50, 30, 20, 40], fn($a, $b) => $b <=> $a);
echo "min peek: " . $h2->peek() . "\n";
$result2 = [];
while ($h2->count() > 0) {
    $result2[] = $h2->pop();
}
echo "min popped: " . implode(",", $result2) . "\n";

// Push after construction maintains min-heap
$h3 = new Ds\Heap([5, 10], fn($a, $b) => $b <=> $a);
$h3->push(1);
echo "after push(1) peek: " . $h3->peek() . "\n";
$h3->push(20);
echo "after push(20) peek: " . $h3->peek() . "\n";

// Min-heap with negative numbers
$h4 = new Ds\Heap([-1, -5, -3, 0, 2], fn($a, $b) => $b <=> $a);
$result3 = [];
while ($h4->count() > 0) {
    $result3[] = $h4->pop();
}
echo "negatives: " . implode(",", $result3) . "\n";

// Min-heap with duplicates
$h5 = new Ds\Heap([3, 1, 3, 1, 2], fn($a, $b) => $b <=> $a);
$result4 = [];
while ($h5->count() > 0) {
    $result4[] = $h5->pop();
}
echo "duplicates: " . implode(",", $result4) . "\n";

// Min-heap with strings
$h6 = new Ds\Heap(["cherry", "apple", "banana"], fn($a, $b) => $b <=> $a);
$result5 = [];
while ($h6->count() > 0) {
    $result5[] = $h6->pop();
}
echo "strings: " . implode(",", $result5) . "\n";
?>
--EXPECT--
peek (min): 1
count: 6
popped: 1,2,3,5,8,9
min peek: 10
min popped: 10,20,30,40,50
after push(1) peek: 1
after push(20) peek: 1
negatives: -5,-3,-1,0,2
duplicates: 1,1,2,3,3
strings: apple,banana,cherry
