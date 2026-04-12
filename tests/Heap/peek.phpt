--TEST--
Heap::peek - returns max element without removing it
--FILE--
<?php
// Peek on empty throws UnderflowException
$h = new Ds\Heap();
try {
    $h->peek();
    echo "ERROR: should have thrown\n";
} catch (\UnderflowException $e) {
    echo "empty peek throws: yes\n";
}

// Peek returns max
$h->push(3, 1, 5, 2, 4);
echo "peek: " . $h->peek() . "\n";
echo "count after peek: " . $h->count() . "\n";

// Peek is idempotent
echo "peek again: " . $h->peek() . "\n";
echo "peek again: " . $h->peek() . "\n";
echo "count still: " . $h->count() . "\n";

// Peek updates after pop
$h->pop();
echo "after pop, peek: " . $h->peek() . "\n";

// Peek with single element
$h2 = new Ds\Heap();
$h2->push(42);
echo "single: " . $h2->peek() . "\n";

// Peek with duplicates
$h3 = new Ds\Heap([5, 5, 5]);
echo "duplicates: " . $h3->peek() . "\n";

// Peek with negative numbers
$h4 = new Ds\Heap([-3, -1, -5]);
echo "negatives: " . $h4->peek() . "\n";

// Peek with strings (max-heap alphabetical)
$h5 = new Ds\Heap(["banana", "apple", "cherry"]);
echo "strings: " . $h5->peek() . "\n";

// Min-heap peek returns minimum
$h6 = new Ds\Heap([3, 1, 5], fn($a, $b) => $b <=> $a);
echo "min-heap: " . $h6->peek() . "\n";
?>
--EXPECT--
empty peek throws: yes
peek: 5
count after peek: 5
peek again: 5
peek again: 5
count still: 5
after pop, peek: 4
single: 42
duplicates: 5
negatives: -1
strings: cherry
min-heap: 1
