--TEST--
Set::get - gets value by positional index
--FILE--
<?php
$s = new Ds\Set(["a", "b", "c", "d"]);

// Basic positional access
echo "get(0): " . $s->get(0) . "\n";
echo "get(1): " . $s->get(1) . "\n";
echo "get(2): " . $s->get(2) . "\n";
echo "get(3): " . $s->get(3) . "\n";

// Positive index out of range
try {
    $s->get(4);
} catch (OutOfRangeException $e) {
    echo "index 4: " . $e->getMessage() . "\n";
}

// Negative index out of range
try {
    $s->get(-1);
} catch (OutOfRangeException $e) {
    echo "index -1: " . $e->getMessage() . "\n";
}

// Empty set
try {
    $empty = new Ds\Set();
    $empty->get(0);
} catch (OutOfRangeException $e) {
    echo "empty get(0): " . $e->getMessage() . "\n";
}

// Single element
$single = new Ds\Set([42]);
echo "single get(0): " . $single->get(0) . "\n";

// Large index
try {
    $s->get(100);
} catch (OutOfRangeException $e) {
    echo "index 100: " . $e->getMessage() . "\n";
}
?>
--EXPECT--
get(0): a
get(1): b
get(2): c
get(3): d
index 4: Index out of range: 4, expected 0 <= x <= 3
index -1: Index out of range: -1, expected 0 <= x <= 3
empty get(0): Index out of range: 0
single get(0): 42
index 100: Index out of range: 100, expected 0 <= x <= 3
