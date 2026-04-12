--TEST--
Heap::allocate - allocate capacity and verify with capacity()
--FILE--
<?php
$h = new Ds\Heap();

// Default capacity
$default = $h->capacity();
echo "default capacity: $default\n";

// Allocate more than default
$h->allocate(100);
$cap = $h->capacity();
echo "after allocate(100): " . ($cap >= 100 ? "ok" : "fail") . "\n";

// Allocate less than current does not shrink
$h->allocate(10);
echo "after allocate(10): " . ($h->capacity() >= 100 ? "ok" : "fail") . "\n";

// Allocate zero does nothing
$h->allocate(0);
echo "after allocate(0): " . ($h->capacity() >= 100 ? "ok" : "fail") . "\n";

// Push values after allocate, capacity should still hold
$h2 = new Ds\Heap();
$h2->allocate(50);
$cap2 = $h2->capacity();
for ($i = 0; $i < 10; $i++) {
    $h2->push($i);
}
echo "after 10 pushes on allocate(50): " . ($h2->capacity() >= 50 ? "ok" : "fail") . "\n";

// Allocate returns void
$result = $h2->allocate(200);
echo "returns void: " . ($result === null ? "yes" : "no") . "\n";
?>
--EXPECT--
default capacity: 8
after allocate(100): ok
after allocate(10): ok
after allocate(0): ok
after 10 pushes on allocate(50): ok
returns void: yes
