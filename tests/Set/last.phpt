--TEST--
Set::last - returns the last value
--FILE--
<?php
$s = new Ds\Set([10, 20, 30]);
echo "last: " . $s->last() . "\n";

// Single element
$s2 = new Ds\Set(["only"]);
echo "single: " . $s2->last() . "\n";

// Preserves insertion order
$s3 = new Ds\Set();
$s3->add("c");
$s3->add("a");
$s3->add("b");
echo "insertion order: " . $s3->last() . "\n";

// Last with null as last element
$s4 = new Ds\Set([1, 2, null]);
echo "null last: " . var_export($s4->last(), true) . "\n";

// Empty set throws UnderflowException
try {
    $empty = new Ds\Set();
    $empty->last();
} catch (UnderflowException $e) {
    echo "empty: " . $e->getMessage() . "\n";
}

// Does not remove the element
echo "still has 3: " . $s->count() . "\n";
?>
--EXPECT--
last: 30
single: only
insertion order: b
null last: NULL
empty: Unexpected empty state
still has 3: 3
