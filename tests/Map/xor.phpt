--TEST--
Map::xor - symmetric difference, keys in either but not both
--FILE--
<?php
$a = new Ds\Map(["a" => 1, "b" => 2, "c" => 3]);
$b = new Ds\Map(["b" => 20, "c" => 30, "d" => 40]);

$xor = $a->xor($b);
echo "count: " . $xor->count() . "\n";
echo "has a: " . var_export($xor->hasKey("a"), true) . "\n";
echo "has d: " . var_export($xor->hasKey("d"), true) . "\n";
echo "has b: " . var_export($xor->hasKey("b"), true) . "\n";
echo "has c: " . var_export($xor->hasKey("c"), true) . "\n";

// Values from their respective maps
echo "a value: " . $xor->get("a") . "\n";
echo "d value: " . $xor->get("d") . "\n";

// Xor with empty = all from this
$empty = new Ds\Map();
$xor2 = $a->xor($empty);
echo "with empty: " . $xor2->count() . "\n";

// Empty xor with non-empty = all from other
$xor3 = $empty->xor($a);
echo "empty with non-empty: " . $xor3->count() . "\n";

// Xor with self = empty
$xor4 = $a->xor($a);
echo "self xor: " . $xor4->count() . "\n";

// No overlapping keys = all keys
$c = new Ds\Map(["x" => 1, "y" => 2]);
$d = new Ds\Map(["a" => 3, "b" => 4]);
$xor5 = $c->xor($d);
echo "no overlap: " . $xor5->count() . "\n";

// Completely overlapping keys = empty
$e = new Ds\Map(["a" => 1, "b" => 2]);
$f = new Ds\Map(["a" => 10, "b" => 20]);
$xor6 = $e->xor($f);
echo "full overlap: " . $xor6->count() . "\n";

// Returns new map
echo "orig a count: " . $a->count() . "\n";
?>
--EXPECT--
count: 2
has a: true
has d: true
has b: false
has c: false
a value: 1
d value: 40
with empty: 3
empty with non-empty: 3
self xor: 0
no overlap: 4
full overlap: 0
orig a count: 3
