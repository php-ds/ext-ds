--TEST--
Set::allocate - pre-allocates capacity
--FILE--
<?php
$s = new Ds\Set();
$initial = $s->capacity();
echo "initial capacity: $initial\n";

// Allocate more
$s->allocate(100);
$cap = $s->capacity();
echo "after allocate(100) >= 100: " . var_export($cap >= 100, true) . "\n";

// Allocating less does not shrink
$s->allocate(10);
echo "after allocate(10) still >= 100: " . var_export($s->capacity() >= 100, true) . "\n";

// Allocate zero on fresh set is fine
$s2 = new Ds\Set();
$s2->allocate(0);
echo "allocate(0) ok: true\n";

// Negative capacity throws OutOfRangeException
try {
    $s->allocate(-1);
} catch (OutOfRangeException $e) {
    echo "negative: " . $e->getMessage() . "\n";
}

// Capacity is power of 2
$s3 = new Ds\Set();
$s3->allocate(33);
$cap = $s3->capacity();
echo "allocate(33) is power of 2: " . var_export(($cap & ($cap - 1)) === 0, true) . "\n";
echo "allocate(33) >= 33: " . var_export($cap >= 33, true) . "\n";
?>
--EXPECT--
initial capacity: 8
after allocate(100) >= 100: true
after allocate(10) still >= 100: true
allocate(0) ok: true
negative: Capacity must be non-negative, -1 given
allocate(33) is power of 2: true
allocate(33) >= 33: true
