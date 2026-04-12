--TEST--
Map::sort - sort by value in-place
--FILE--
<?php
// Natural sort
$m = new Ds\Map(["a" => 3, "b" => 1, "c" => 2]);
$m->sort();
$result = [];
foreach ($m as $k => $v) {
    $result[] = "$k=$v";
}
echo "sorted: " . implode(",", $result) . "\n";

// Custom comparator - reverse order
$m2 = new Ds\Map(["a" => 1, "b" => 2, "c" => 3]);
$m2->sort(function($a, $b) {
    return $b <=> $a;
});
$result2 = [];
foreach ($m2 as $k => $v) {
    $result2[] = "$k=$v";
}
echo "reverse: " . implode(",", $result2) . "\n";

// String values
$m3 = new Ds\Map(["x" => "cherry", "y" => "apple", "z" => "banana"]);
$m3->sort();
$result3 = [];
foreach ($m3 as $k => $v) {
    $result3[] = "$k=$v";
}
echo "strings: " . implode(",", $result3) . "\n";

// Sort is in-place
echo "count: " . $m->count() . "\n";

// Empty map sort
$empty = new Ds\Map();
$empty->sort();
echo "empty: " . $empty->count() . "\n";

// Single element
$single = new Ds\Map(["a" => 1]);
$single->sort();
echo "single: " . $single->first()->key . "=" . $single->first()->value . "\n";

// Keys stay with their values
$m4 = new Ds\Map(["c_key" => 10, "a_key" => 30, "b_key" => 20]);
$m4->sort();
$result4 = [];
foreach ($m4 as $k => $v) {
    $result4[] = "$k=$v";
}
echo "keys with values: " . implode(",", $result4) . "\n";
?>
--EXPECT--
sorted: b=1,c=2,a=3
reverse: c=3,b=2,a=1
strings: y=apple,z=banana,x=cherry
count: 3
empty: 0
single: a=1
keys with values: c_key=10,b_key=20,a_key=30
