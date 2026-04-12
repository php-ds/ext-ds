--TEST--
Map::copy - shallow copy with COW semantics
--FILE--
<?php
$a = new Ds\Map(["a" => 1, "b" => 2]);
$b = $a->copy();

// Same contents initially
echo "a count: " . $a->count() . "\n";
echo "b count: " . $b->count() . "\n";
echo "a[a]=" . $a->get("a") . " b[a]=" . $b->get("a") . "\n";

// Mutating copy does not affect original
$b->put("c", 3);
echo "a count after b put: " . $a->count() . "\n";
echo "b count after b put: " . $b->count() . "\n";
echo "a has c: " . var_export($a->hasKey("c"), true) . "\n";
echo "b has c: " . var_export($b->hasKey("c"), true) . "\n";

// Mutating original does not affect copy
$a->put("d", 4);
echo "b has d: " . var_export($b->hasKey("d"), true) . "\n";

// Copy is a different object
echo "same object: " . var_export($a === $b, true) . "\n";

// Copy of empty
$e = new Ds\Map();
$f = $e->copy();
echo "empty copy count: " . $f->count() . "\n";

// Shallow copy: objects are shared
$obj = new stdClass();
$obj->x = 1;
$m1 = new Ds\Map(["obj" => $obj]);
$m2 = $m1->copy();
$obj->x = 999;
echo "shared obj: " . $m1->get("obj")->x . "," . $m2->get("obj")->x . "\n";
?>
--EXPECT--
a count: 2
b count: 2
a[a]=1 b[a]=1
a count after b put: 2
b count after b put: 3
a has c: false
b has c: true
b has d: false
same object: false
empty copy count: 0
shared obj: 999,999
