--TEST--
Map::diff - keys in this but not in other
--FILE--
<?php
$a = new Ds\Map(["a" => 1, "b" => 2, "c" => 3]);
$b = new Ds\Map(["b" => 20, "c" => 30, "d" => 40]);

$diff = $a->diff($b);
echo "diff count: " . $diff->count() . "\n";
echo "has a: " . var_export($diff->hasKey("a"), true) . "\n";
echo "has b: " . var_export($diff->hasKey("b"), true) . "\n";
echo "a value: " . $diff->get("a") . "\n";

// Diff with empty map returns copy of original
$empty = new Ds\Map();
$diff2 = $a->diff($empty);
echo "diff with empty: " . $diff2->count() . "\n";

// Diff of empty with non-empty
$diff3 = $empty->diff($a);
echo "empty diff: " . $diff3->count() . "\n";

// Diff with identical map
$diff4 = $a->diff($a);
echo "self diff: " . $diff4->count() . "\n";

// Diff is a new map, not modifying original
$a->put("z", 99);
echo "original has z: " . var_export($a->hasKey("z"), true) . "\n";
echo "diff has z: " . var_export($diff->hasKey("z"), true) . "\n";

// Diff with integer keys
$c = new Ds\Map([1 => "a", 2 => "b", 3 => "c"]);
$d = new Ds\Map([2 => "x", 4 => "y"]);
$diff5 = $c->diff($d);
echo "int diff count: " . $diff5->count() . "\n";
echo "has 1: " . var_export($diff5->hasKey(1), true) . "\n";
echo "has 3: " . var_export($diff5->hasKey(3), true) . "\n";
?>
--EXPECT--
diff count: 1
has a: true
has b: false
a value: 1
diff with empty: 3
empty diff: 0
self diff: 0
original has z: true
diff has z: false
int diff count: 2
has 1: true
has 3: true
