--TEST--
Set::first - returns the first value
--FILE--
<?php
$s = new Ds\Set([10, 20, 30]);
echo "first: " . $s->first() . "\n";

// Single element
$s2 = new Ds\Set(["only"]);
echo "single: " . $s2->first() . "\n";

// Preserves insertion order
$s3 = new Ds\Set();
$s3->add("c");
$s3->add("a");
$s3->add("b");
echo "insertion order: " . $s3->first() . "\n";

// First with null as first element
$s4 = new Ds\Set([null, 1, 2]);
echo "null first: " . var_export($s4->first(), true) . "\n";

// Empty set throws UnderflowException
try {
    $empty = new Ds\Set();
    $empty->first();
} catch (UnderflowException $e) {
    echo "empty: " . $e->getMessage() . "\n";
}

// Does not remove the element
echo "still has 3: " . $s->count() . "\n";
?>
--EXPECT--
first: 10
single: only
insertion order: c
null first: NULL
empty: Unexpected empty state
still has 3: 3
