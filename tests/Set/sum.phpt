--TEST--
Set::sum - returns the sum of all values
--FILE--
<?php
// Integers
$s = new Ds\Set([1, 2, 3, 4, 5]);
echo "int sum: " . $s->sum() . "\n";

// Floats
$s2 = new Ds\Set([1.5, 2.5, 3.0]);
echo "float sum: " . $s2->sum() . "\n";

// Mixed int and float
$s3 = new Ds\Set([1, 2.5, 3]);
echo "mixed sum: " . $s3->sum() . "\n";

// Empty set
$empty = new Ds\Set();
echo "empty sum: " . $empty->sum() . "\n";

// Single element
$single = new Ds\Set([42]);
echo "single sum: " . $single->sum() . "\n";

// With zero
$s4 = new Ds\Set([0, 1, 2]);
echo "with zero: " . $s4->sum() . "\n";

// Negative numbers
$s5 = new Ds\Set([-1, -2, 3]);
echo "negative: " . $s5->sum() . "\n";

// Boolean values (true=1, false=0)
$s6 = new Ds\Set([true, false]);
echo "booleans: " . $s6->sum() . "\n";

// String numbers
$s7 = new Ds\Set(["10", "20"]);
echo "string nums: " . $s7->sum() . "\n";
?>
--EXPECT--
int sum: 15
float sum: 7
mixed sum: 6.5
empty sum: 0
single sum: 42
with zero: 3
negative: 0
booleans: 1
string nums: 30
