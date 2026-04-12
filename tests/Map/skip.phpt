--TEST--
Map::skip - get pair at positional index
--FILE--
<?php
$m = new Ds\Map(["a" => 1, "b" => 2, "c" => 3]);

// Basic skip
$pair = $m->skip(0);
echo "type: " . get_class($pair) . "\n";
echo "pos 0: " . $pair->key . "=" . $pair->value . "\n";

$pair1 = $m->skip(1);
echo "pos 1: " . $pair1->key . "=" . $pair1->value . "\n";

$pair2 = $m->skip(2);
echo "pos 2: " . $pair2->key . "=" . $pair2->value . "\n";

// Out of range - positive
try {
    $m->skip(3);
    echo "ERROR: should have thrown\n";
} catch (OutOfRangeException $e) {
    echo "out of range +: " . $e->getMessage() . "\n";
}

// Out of range - negative
try {
    $m->skip(-1);
    echo "ERROR: should have thrown\n";
} catch (OutOfRangeException $e) {
    echo "out of range -: " . $e->getMessage() . "\n";
}

// Empty map
try {
    $empty = new Ds\Map();
    $empty->skip(0);
    echo "ERROR: should have thrown\n";
} catch (OutOfRangeException $e) {
    echo "empty: " . $e->getMessage() . "\n";
}

// Single element
$single = new Ds\Map(["only" => 42]);
$p = $single->skip(0);
echo "single: " . $p->key . "=" . $p->value . "\n";

// Skip does not remove
echo "count: " . $m->count() . "\n";
?>
--EXPECT--
type: Ds\Pair
pos 0: a=1
pos 1: b=2
pos 2: c=3
out of range +: Index out of range: 3, expected 0 <= x <= 2
out of range -: Index out of range: -1, expected 0 <= x <= 2
empty: Index out of range: 0
single: only=42
count: 3
