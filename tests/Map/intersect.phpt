--TEST--
Map::intersect - keys in both maps
--FILE--
<?php
$a = new Ds\Map(["a" => 1, "b" => 2, "c" => 3]);
$b = new Ds\Map(["b" => 20, "c" => 30, "d" => 40]);

$inter = $a->intersect($b);
echo "count: " . $inter->count() . "\n";
echo "has b: " . var_export($inter->hasKey("b"), true) . "\n";
echo "has c: " . var_export($inter->hasKey("c"), true) . "\n";
echo "has a: " . var_export($inter->hasKey("a"), true) . "\n";
echo "has d: " . var_export($inter->hasKey("d"), true) . "\n";

// Values come from $this, not $other
echo "b value: " . $inter->get("b") . "\n";
echo "c value: " . $inter->get("c") . "\n";

// Intersect with empty
$empty = new Ds\Map();
$inter2 = $a->intersect($empty);
echo "with empty: " . $inter2->count() . "\n";

// Empty intersect with non-empty
$inter3 = $empty->intersect($a);
echo "empty with non-empty: " . $inter3->count() . "\n";

// Intersect with self
$inter4 = $a->intersect($a);
echo "self intersect: " . $inter4->count() . "\n";

// Intersect returns new map
echo "original count: " . $a->count() . "\n";

// Intersect with no common keys
$c = new Ds\Map(["x" => 1, "y" => 2]);
$d = new Ds\Map(["a" => 1, "b" => 2]);
$inter5 = $c->intersect($d);
echo "no common: " . $inter5->count() . "\n";
?>
--EXPECT--
count: 2
has b: true
has c: true
has a: false
has d: false
b value: 2
c value: 3
with empty: 0
empty with non-empty: 0
self intersect: 3
original count: 3
no common: 0
