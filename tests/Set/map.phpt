--TEST--
Set::map - applies callback and returns new Set
--FILE--
<?php
$s = new Ds\Set([1, 2, 3, 4]);

// Basic map
$doubled = $s->map(fn($v) => $v * 2);
echo "doubled: " . $doubled->join(",") . "\n";
echo "count: " . $doubled->count() . "\n";

// Original unchanged
echo "original: " . $s->join(",") . "\n";

// Map that deduplicates (mapped values collide)
$s2 = new Ds\Set([1, 2, 3, 4, 5, 6]);
$modded = $s2->map(fn($v) => $v % 3);
echo "mod 3 count: " . $modded->count() . "\n";

// Map to strings
$str = $s->map(fn($v) => "item_$v");
echo "strings: " . $str->join(",") . "\n";

// Map empty set
$empty = new Ds\Set();
$result = $empty->map(fn($v) => $v);
echo "empty map: " . $result->count() . "\n";

// Map to same type
$identity = $s->map(fn($v) => $v);
echo "identity: " . $identity->join(",") . "\n";

// Map changes type
$bools = $s->map(fn($v) => $v > 2);
echo "bools count: " . $bools->count() . "\n";

// Returns new Set instance
echo "new instance: " . var_export($doubled !== $s, true) . "\n";
?>
--EXPECT--
doubled: 2,4,6,8
count: 4
original: 1,2,3,4
mod 3 count: 3
strings: item_1,item_2,item_3,item_4
empty map: 0
identity: 1,2,3,4
bools count: 2
new instance: true
