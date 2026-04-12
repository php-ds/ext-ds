--TEST--
Heap::capacity - initial capacity and growth behavior
--FILE--
<?php
$h = new Ds\Heap();

// Initial capacity is 8
echo "initial: " . $h->capacity() . "\n";

// Capacity should not change until we exceed it
for ($i = 0; $i < 8; $i++) {
    $h->push($i);
}
echo "after 8 pushes: " . $h->capacity() . "\n";

// Push one more to trigger growth
$h->push(99);
$grown = $h->capacity();
echo "after 9 pushes, grew: " . ($grown > 8 ? "yes" : "no") . "\n";

// Fill up to many elements and verify capacity always >= count
$h2 = new Ds\Heap();
for ($i = 0; $i < 100; $i++) {
    $h2->push($i);
    if ($h2->capacity() < $h2->count()) {
        echo "ERROR: capacity < count at $i\n";
        break;
    }
}
echo "100 pushes, capacity >= count: yes\n";
echo "capacity after 100: " . $h2->capacity() . "\n";

// Capacity on a heap constructed with values
$h3 = new Ds\Heap([1, 2, 3]);
echo "constructed with 3: capacity >= 3: " . ($h3->capacity() >= 3 ? "yes" : "no") . "\n";
?>
--EXPECT--
initial: 8
after 8 pushes: 8
after 9 pushes, grew: yes
100 pushes, capacity >= count: yes
capacity after 100: 128
constructed with 3: capacity >= 3: yes
