--TEST--
Heap::clear - empties heap and resets capacity
--FILE--
<?php
$h = new Ds\Heap();
$h->push(5, 3, 8, 1, 9);

echo "before clear: count=" . $h->count() . "\n";
echo "before clear: empty=" . var_export($h->isEmpty(), true) . "\n";

$result = $h->clear();
echo "returns void: " . ($result === null ? "yes" : "no") . "\n";
echo "after clear: count=" . $h->count() . "\n";
echo "after clear: empty=" . var_export($h->isEmpty(), true) . "\n";
echo "after clear: capacity=" . $h->capacity() . "\n";

// Can push after clear
$h->push(42);
echo "after push: count=" . $h->count() . "\n";
echo "after push: peek=" . $h->peek() . "\n";

// Clear already empty heap
$h2 = new Ds\Heap();
$h2->clear();
echo "clear empty: count=" . $h2->count() . "\n";

// Clear heap with large allocation
$h3 = new Ds\Heap();
$h3->allocate(1000);
$h3->push(1, 2, 3);
$h3->clear();
echo "clear allocated: capacity=" . $h3->capacity() . "\n";
?>
--EXPECT--
before clear: count=5
before clear: empty=false
returns void: yes
after clear: count=0
after clear: empty=true
after clear: capacity=8
after push: count=1
after push: peek=42
clear empty: count=0
clear allocated: capacity=8
