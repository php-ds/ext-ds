--TEST--
Heap::push - push single and multiple values, verify heap order
--FILE--
<?php
// Push single value
$h = new Ds\Heap();
$h->push(5);
echo "single push: peek=" . $h->peek() . " count=" . $h->count() . "\n";

// Push returns void
$result = $h->push(10);
echo "returns void: " . ($result === null ? "yes" : "no") . "\n";

// Push multiple values (variadic)
$h2 = new Ds\Heap();
$h2->push(3, 1, 4, 1, 5);
echo "variadic count: " . $h2->count() . "\n";
echo "variadic peek: " . $h2->peek() . "\n";

// Push maintains heap property
$h3 = new Ds\Heap();
$h3->push(1);
echo "after push(1): " . $h3->peek() . "\n";
$h3->push(10);
echo "after push(10): " . $h3->peek() . "\n";
$h3->push(5);
echo "after push(5): " . $h3->peek() . "\n";
$h3->push(20);
echo "after push(20): " . $h3->peek() . "\n";
$h3->push(15);
echo "after push(15): " . $h3->peek() . "\n";

// Push duplicates
$h4 = new Ds\Heap();
$h4->push(5, 5, 5);
echo "duplicates count: " . $h4->count() . "\n";
echo "duplicates peek: " . $h4->peek() . "\n";

// Push negative numbers
$h5 = new Ds\Heap();
$h5->push(-1, -5, -3, 0);
echo "with negatives peek: " . $h5->peek() . "\n";

// Push strings
$h6 = new Ds\Heap();
$h6->push("cherry", "apple", "banana");
echo "strings peek: " . $h6->peek() . "\n";

// Push to existing heap
$h7 = new Ds\Heap([1, 2, 3]);
$h7->push(100);
echo "push to existing: peek=" . $h7->peek() . " count=" . $h7->count() . "\n";

// Push after pop
$h8 = new Ds\Heap([10, 20, 30]);
$h8->pop();
$h8->push(25);
echo "push after pop: peek=" . $h8->peek() . "\n";

// Push many to verify order
$h9 = new Ds\Heap();
for ($i = 0; $i < 20; $i++) {
    $h9->push($i);
}
$result = [];
while ($h9->count() > 0) {
    $result[] = $h9->pop();
}
echo "0-19 popped: " . implode(",", $result) . "\n";
?>
--EXPECT--
single push: peek=5 count=1
returns void: yes
variadic count: 5
variadic peek: 5
after push(1): 1
after push(10): 10
after push(5): 10
after push(20): 20
after push(15): 20
duplicates count: 3
duplicates peek: 5
with negatives peek: 0
strings peek: cherry
push to existing: peek=100 count=4
push after pop: peek=25
0-19 popped: 19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0
