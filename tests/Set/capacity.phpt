--TEST--
Set::capacity - returns current capacity
--FILE--
<?php
$s = new Ds\Set();
echo "initial: " . $s->capacity() . "\n";

// MIN_CAPACITY constant
echo "MIN_CAPACITY: " . Ds\Set::MIN_CAPACITY . "\n";

// Capacity grows as elements are added
$s = new Ds\Set();
for ($i = 0; $i < 8; $i++) {
    $s->add($i);
}
$cap8 = $s->capacity();
echo "after 8 elements: $cap8\n";

// Adding one more should trigger growth
$s->add(8);
$cap9 = $s->capacity();
echo "grew after 9th: " . var_export($cap9 > $cap8, true) . "\n";

// Capacity is always a power of 2
echo "power of 2: " . var_export(($cap9 & ($cap9 - 1)) === 0, true) . "\n";

// Capacity after clear resets
$s->clear();
echo "after clear: " . $s->capacity() . "\n";
?>
--EXPECT--
initial: 8
MIN_CAPACITY: 8
after 8 elements: 8
grew after 9th: true
power of 2: true
after clear: 8
