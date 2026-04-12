--TEST--
Heap::isEmpty - check if heap has no elements
--FILE--
<?php
// New heap is empty
$h = new Ds\Heap();
echo "new: " . var_export($h->isEmpty(), true) . "\n";

// After push, not empty
$h->push(1);
echo "after push: " . var_export($h->isEmpty(), true) . "\n";

// After pop back to zero
$h->pop();
echo "after pop: " . var_export($h->isEmpty(), true) . "\n";

// Constructed with values
$h2 = new Ds\Heap([1, 2, 3]);
echo "constructed: " . var_export($h2->isEmpty(), true) . "\n";

// After clear
$h2->clear();
echo "after clear: " . var_export($h2->isEmpty(), true) . "\n";

// Empty constructed
$h3 = new Ds\Heap([]);
echo "empty array: " . var_export($h3->isEmpty(), true) . "\n";

// Push multiple then pop all
$h4 = new Ds\Heap();
$h4->push(1, 2, 3);
$h4->pop();
$h4->pop();
echo "one left: " . var_export($h4->isEmpty(), true) . "\n";
$h4->pop();
echo "all popped: " . var_export($h4->isEmpty(), true) . "\n";
?>
--EXPECT--
new: true
after push: false
after pop: true
constructed: false
after clear: true
empty array: true
one left: false
all popped: true
