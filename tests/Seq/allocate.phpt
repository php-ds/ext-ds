--TEST--
Seq::allocate - pre-allocate capacity
--FILE--
<?php
$s = new Ds\Seq();
var_dump($s->capacity()); // default MIN_CAPACITY = 8

// Allocate less than current capacity does nothing
$s->allocate(4);
var_dump($s->capacity());

// Allocate exactly current capacity does nothing
$s->allocate(8);
var_dump($s->capacity());

// Allocate more rounds up to next power of 2
$s->allocate(9);
var_dump($s->capacity());

// Allocate power of 2
$s->allocate(32);
var_dump($s->capacity());

// Allocate 0 does nothing
$s->allocate(0);
var_dump($s->capacity());

// Allocate does not affect size
$s = new Ds\Seq([1, 2, 3]);
$s->allocate(100);
var_dump($s->count());
var_dump($s->capacity());

// Negative capacity throws
try {
    $s->allocate(-1);
} catch (OutOfRangeException $e) {
    echo "OutOfRangeException\n";
}
?>
--EXPECT--
int(8)
int(8)
int(8)
int(16)
int(32)
int(32)
int(3)
int(128)
OutOfRangeException
