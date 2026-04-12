--TEST--
Map::reverse - reverse pair order in-place
--FILE--
<?php
$m = new Ds\Map(["a" => 1, "b" => 2, "c" => 3]);

$m->reverse();
$result = [];
foreach ($m as $k => $v) {
    $result[] = "$k=$v";
}
echo "reversed: " . implode(",", $result) . "\n";

// Double reverse returns to original order
$m->reverse();
$result2 = [];
foreach ($m as $k => $v) {
    $result2[] = "$k=$v";
}
echo "double: " . implode(",", $result2) . "\n";

// Reverse single element
$single = new Ds\Map(["x" => 1]);
$single->reverse();
echo "single: " . $single->first()->key . "=" . $single->first()->value . "\n";

// Reverse empty map
$empty = new Ds\Map();
$empty->reverse();
echo "empty: " . $empty->count() . "\n";

// Count preserved
echo "count: " . $m->count() . "\n";

// Reverse two elements
$m2 = new Ds\Map(["a" => 1, "b" => 2]);
$m2->reverse();
$first = $m2->first();
$last = $m2->last();
echo "two: " . $first->key . "," . $last->key . "\n";
?>
--EXPECT--
reversed: c=3,b=2,a=1
double: a=1,b=2,c=3
single: x=1
empty: 0
count: 3
two: b,a
