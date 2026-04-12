--TEST--
Map::ksorted - return new map sorted by key
--FILE--
<?php
// Natural sort
$m = new Ds\Map(["c" => 3, "a" => 1, "b" => 2]);
$sorted = $m->ksorted();

// Original unchanged
$origKeys = [];
foreach ($m as $k => $v) { $origKeys[] = $k; }
echo "original: " . implode(",", $origKeys) . "\n";

// Sorted result
$sortedKeys = [];
foreach ($sorted as $k => $v) { $sortedKeys[] = "$k=$v"; }
echo "sorted: " . implode(",", $sortedKeys) . "\n";

// Custom comparator
$m2 = new Ds\Map(["a" => 1, "c" => 3, "b" => 2]);
$rev = $m2->ksorted(function($a, $b) {
    return $b <=> $a;
});
$revKeys = [];
foreach ($rev as $k => $v) { $revKeys[] = "$k=$v"; }
echo "reverse: " . implode(",", $revKeys) . "\n";

// Returns a Map
echo "type: " . get_class($sorted) . "\n";

// Empty map
$empty = new Ds\Map();
$sortedEmpty = $empty->ksorted();
echo "empty: " . $sortedEmpty->count() . "\n";

// Integer keys
$m3 = new Ds\Map([3 => "c", 1 => "a", 2 => "b"]);
$s3 = $m3->ksorted();
$r3 = [];
foreach ($s3 as $k => $v) { $r3[] = "$k=$v"; }
echo "int sorted: " . implode(",", $r3) . "\n";
?>
--EXPECT--
original: c,a,b
sorted: a=1,b=2,c=3
reverse: c=3,b=2,a=1
type: Ds\Map
empty: 0
int sorted: 1=a,2=b,3=c
