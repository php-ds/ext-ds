--TEST--
Seq::capacity - capacity growth and shrink behavior
--FILE--
<?php
// Default capacity
$s = new Ds\Seq();
var_dump($s->capacity()); // 8

// Capacity grows by doubling when full
$s = new Ds\Seq();
for ($i = 0; $i < 8; $i++) {
    $s->push($i);
}
var_dump($s->capacity()); // 8 (exactly full)

$s->push(8); // 9th element triggers doubling
var_dump($s->capacity()); // 16

// Fill to 16
for ($i = 9; $i < 16; $i++) {
    $s->push($i);
}
var_dump($s->capacity()); // 16

$s->push(16); // 17th element triggers doubling
var_dump($s->capacity()); // 32

// Capacity shrinks when size <= capacity/4
// We have 17 elements, capacity 32. Pop down to 8 (which is 32/4).
for ($i = 0; $i < 9; $i++) {
    $s->pop();
}
// Now size = 8, capacity/4 = 8, size <= capacity/4 so should halve
var_dump($s->count());
var_dump($s->capacity()); // 16

// Capacity never goes below MIN_CAPACITY (8)
$s = new Ds\Seq([1, 2, 3]);
$s->clear();
var_dump($s->capacity()); // 8
?>
--EXPECT--
int(8)
int(8)
int(16)
int(16)
int(32)
int(8)
int(16)
int(8)
