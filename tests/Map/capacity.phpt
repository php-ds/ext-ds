--TEST--
Map::capacity - check capacity growth and shrinkage
--FILE--
<?php
$m = new Ds\Map();
echo "initial: " . $m->capacity() . "\n"; // 8

// Fill up to trigger growth
for ($i = 0; $i < 8; $i++) {
    $m->put($i, $i);
}
echo "after 8 puts: " . $m->capacity() . "\n";

// Capacity should have grown by now or at next insert
$m->put(8, 8);
$cap = $m->capacity();
echo "after 9 puts: " . ($cap > 8 ? "grew" : "same") . "\n";

// After clear, capacity should reset
$m->clear();
echo "after clear: " . $m->capacity() . "\n"; // 8

// MIN_CAPACITY constant
echo "MIN_CAPACITY: " . Ds\Map::MIN_CAPACITY . "\n";
?>
--EXPECT--
initial: 8
after 8 puts: 8
after 9 puts: grew
after clear: 8
MIN_CAPACITY: 8
