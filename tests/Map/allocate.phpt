--TEST--
Map::allocate - pre-allocate capacity
--FILE--
<?php
$m = new Ds\Map();
echo "initial capacity: " . $m->capacity() . "\n";

// Allocate more than default
$m->allocate(100);
$cap = $m->capacity();
echo "after allocate(100): " . ($cap >= 100 ? "ok" : "fail") . "\n";

// Allocating less than current capacity should not shrink
$m->allocate(10);
echo "after allocate(10): " . ($m->capacity() >= 100 ? "ok" : "fail") . "\n";

// Allocate 0 is valid
$m2 = new Ds\Map();
$m2->allocate(0);
echo "allocate(0): ok\n";

// Negative capacity throws
try {
    $m2->allocate(-1);
    echo "ERROR: should have thrown\n";
} catch (OutOfRangeException $e) {
    echo "negative: " . $e->getMessage() . "\n";
}

// Capacity is always a power of 2
$m3 = new Ds\Map();
$m3->allocate(17);
$cap = $m3->capacity();
echo "power of 2 for 17: " . $cap . "\n"; // should be 32
?>
--EXPECT--
initial capacity: 8
after allocate(100): ok
after allocate(10): ok
allocate(0): ok
negative: Capacity must be non-negative, -1 given
power of 2 for 17: 32
