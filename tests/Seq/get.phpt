--TEST--
Seq::get - get element at index
--FILE--
<?php
$s = new Ds\Seq(["a", "b", "c", "d"]);

// Valid indices
var_dump($s->get(0));
var_dump($s->get(1));
var_dump($s->get(2));
var_dump($s->get(3));

// Negative index: out of range
try {
    $s->get(-1);
} catch (OutOfRangeException $e) {
    echo "OutOfRangeException: negative\n";
}

// Index equal to count: out of range
try {
    $s->get(4);
} catch (OutOfRangeException $e) {
    echo "OutOfRangeException: equal to count\n";
}

// Large index: out of range
try {
    $s->get(100);
} catch (OutOfRangeException $e) {
    echo "OutOfRangeException: large\n";
}

// Empty seq
$s = new Ds\Seq();
try {
    $s->get(0);
} catch (OutOfRangeException $e) {
    echo "OutOfRangeException: empty\n";
}

// Get does not remove
$s = new Ds\Seq([1, 2, 3]);
$s->get(1);
var_dump($s->count());
?>
--EXPECT--
string(1) "a"
string(1) "b"
string(1) "c"
string(1) "d"
OutOfRangeException: negative
OutOfRangeException: equal to count
OutOfRangeException: large
OutOfRangeException: empty
int(3)
