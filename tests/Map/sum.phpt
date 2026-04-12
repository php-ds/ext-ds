--TEST--
Map::sum - sum of all values
--FILE--
<?php
// Integer sum
$m = new Ds\Map(["a" => 1, "b" => 2, "c" => 3]);
echo "int sum: " . $m->sum() . "\n";

// Float sum
$m2 = new Ds\Map(["a" => 1.5, "b" => 2.5, "c" => 3.0]);
echo "float sum: " . $m2->sum() . "\n";

// Mixed int and float
$m3 = new Ds\Map(["a" => 1, "b" => 2.5]);
echo "mixed sum: " . $m3->sum() . "\n";

// Empty map
$empty = new Ds\Map();
echo "empty sum: " . $empty->sum() . "\n";

// Negative values
$m4 = new Ds\Map(["a" => -1, "b" => -2, "c" => 3]);
echo "negative sum: " . $m4->sum() . "\n";

// Single element
$m5 = new Ds\Map(["a" => 42]);
echo "single: " . $m5->sum() . "\n";

// Zero values
$m6 = new Ds\Map(["a" => 0, "b" => 0]);
echo "zeros: " . $m6->sum() . "\n";

// String numeric values
$m7 = new Ds\Map(["a" => "10", "b" => "20"]);
echo "string nums: " . $m7->sum() . "\n";
?>
--EXPECT--
int sum: 6
float sum: 7
mixed sum: 3.5
empty sum: 0
negative sum: 0
single: 42
zeros: 0
string nums: 30
