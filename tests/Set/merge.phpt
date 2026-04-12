--TEST--
Set::merge - returns new Set with added values
--FILE--
<?php
$s = new Ds\Set([1, 2, 3]);

// Merge with array
$merged = $s->merge([3, 4, 5]);
echo "merged: " . $merged->join(",") . "\n";
echo "merged count: " . $merged->count() . "\n";

// Original unchanged
echo "original: " . $s->join(",") . "\n";

// Merge with empty array
$merged2 = $s->merge([]);
echo "merge empty: " . $merged2->join(",") . "\n";

// Merge with duplicates
$merged3 = $s->merge([1, 1, 2, 2]);
echo "merge dups: " . $merged3->join(",") . "\n";
echo "merge dups count: " . $merged3->count() . "\n";

// Merge with iterator
$merged4 = $s->merge(new ArrayIterator([6, 7]));
echo "merge iterator: " . $merged4->join(",") . "\n";

// Merge on empty set
$empty = new Ds\Set();
$merged5 = $empty->merge([1, 2]);
echo "merge on empty: " . $merged5->join(",") . "\n";

// Merge with generator
function gen() {
    yield 10;
    yield 20;
}
$merged6 = $s->merge(gen());
echo "merge gen: " . $merged6->join(",") . "\n";

// Returns new Set
echo "new instance: " . var_export($merged !== $s, true) . "\n";
?>
--EXPECT--
merged: 1,2,3,4,5
merged count: 5
original: 1,2,3
merge empty: 1,2,3
merge dups: 1,2,3
merge dups count: 3
merge iterator: 1,2,3,6,7
merge on empty: 1,2
merge gen: 1,2,3,10,20
new instance: true
