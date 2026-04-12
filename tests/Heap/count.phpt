--TEST--
Heap::count - element count in various states
--FILE--
<?php
// Empty heap
$h = new Ds\Heap();
echo "empty: " . $h->count() . "\n";
echo "count() matches count(\$h): " . var_export(count($h) === $h->count(), true) . "\n";

// After single push
$h->push(1);
echo "after 1 push: " . $h->count() . "\n";

// After multiple pushes
$h->push(2, 3, 4);
echo "after 3 more: " . $h->count() . "\n";

// After pop
$h->pop();
echo "after 1 pop: " . $h->count() . "\n";

// Pop all
while ($h->count() > 0) {
    $h->pop();
}
echo "after pop all: " . $h->count() . "\n";

// After clear
$h->push(1, 2, 3);
$h->clear();
echo "after clear: " . $h->count() . "\n";

// Constructed with values
$h2 = new Ds\Heap([10, 20, 30, 40, 50]);
echo "constructed with 5: " . $h2->count() . "\n";

// Count with duplicates
$h3 = new Ds\Heap([1, 1, 1, 1]);
echo "with duplicates: " . $h3->count() . "\n";

// Large count
$h4 = new Ds\Heap();
for ($i = 0; $i < 1000; $i++) {
    $h4->push($i);
}
echo "after 1000 pushes: " . $h4->count() . "\n";
?>
--EXPECT--
empty: 0
count() matches count($h): true
after 1 push: 1
after 3 more: 4
after 1 pop: 3
after pop all: 0
after clear: 0
constructed with 5: 5
with duplicates: 4
after 1000 pushes: 1000
