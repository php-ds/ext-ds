--TEST--
Map::union - all keys from both maps, other values take precedence
--FILE--
<?php
$a = new Ds\Map(["a" => 1, "b" => 2, "c" => 3]);
$b = new Ds\Map(["b" => 20, "c" => 30, "d" => 40]);

$union = $a->union($b);
echo "count: " . $union->count() . "\n";

// All keys present
echo "has a: " . var_export($union->hasKey("a"), true) . "\n";
echo "has b: " . var_export($union->hasKey("b"), true) . "\n";
echo "has c: " . var_export($union->hasKey("c"), true) . "\n";
echo "has d: " . var_export($union->hasKey("d"), true) . "\n";

// Values from $b take precedence for common keys
echo "a value: " . $union->get("a") . "\n";
echo "b value: " . $union->get("b") . "\n";
echo "c value: " . $union->get("c") . "\n";
echo "d value: " . $union->get("d") . "\n";

// Union with empty
$empty = new Ds\Map();
$union2 = $a->union($empty);
echo "with empty: " . $union2->count() . "\n";

// Empty union with non-empty
$union3 = $empty->union($a);
echo "empty with non-empty: " . $union3->count() . "\n";

// Union returns new map
echo "orig a count: " . $a->count() . "\n";
echo "orig b count: " . $b->count() . "\n";

// Union with self
$union4 = $a->union($a);
echo "self union: " . $union4->count() . "\n";

// No overlapping keys
$c = new Ds\Map(["x" => 1, "y" => 2]);
$d = new Ds\Map(["a" => 3, "b" => 4]);
$union5 = $c->union($d);
echo "no overlap: " . $union5->count() . "\n";
?>
--EXPECT--
count: 4
has a: true
has b: true
has c: true
has d: true
a value: 1
b value: 20
c value: 30
d value: 40
with empty: 3
empty with non-empty: 3
orig a count: 3
orig b count: 3
self union: 3
no overlap: 4
