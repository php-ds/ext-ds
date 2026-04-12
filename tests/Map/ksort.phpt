--TEST--
Map::ksort - sort by key in-place
--FILE--
<?php
// Natural sort
$m = new Ds\Map(["c" => 3, "a" => 1, "b" => 2]);
$m->ksort();
$result = [];
foreach ($m as $k => $v) {
    $result[] = "$k=$v";
}
echo "sorted: " . implode(",", $result) . "\n";

// Integer keys
$m2 = new Ds\Map([3 => "c", 1 => "a", 2 => "b"]);
$m2->ksort();
$result2 = [];
foreach ($m2 as $k => $v) {
    $result2[] = "$k=$v";
}
echo "int sorted: " . implode(",", $result2) . "\n";

// Custom comparator - reverse order
$m3 = new Ds\Map(["a" => 1, "b" => 2, "c" => 3]);
$m3->ksort(function($a, $b) {
    return $b <=> $a;
});
$result3 = [];
foreach ($m3 as $k => $v) {
    $result3[] = "$k=$v";
}
echo "reverse: " . implode(",", $result3) . "\n";

// Sort is in-place
$m4 = new Ds\Map(["b" => 2, "a" => 1]);
$m4->ksort();
echo "count: " . $m4->count() . "\n";

// Empty map sort
$empty = new Ds\Map();
$empty->ksort();
echo "empty sort: " . $empty->count() . "\n";

// Single element
$single = new Ds\Map(["a" => 1]);
$single->ksort();
echo "single: " . $single->first()->key . "=" . $single->first()->value . "\n";
?>
--EXPECT--
sorted: a=1,b=2,c=3
int sorted: 1=a,2=b,3=c
reverse: c=3,b=2,a=1
count: 2
empty sort: 0
single: a=1
