--TEST--
Map::clear - remove all pairs
--FILE--
<?php
$m = new Ds\Map(["a" => 1, "b" => 2, "c" => 3]);
echo "before clear: " . $m->count() . "\n";

$m->clear();
echo "after clear: " . $m->count() . "\n";
echo "is empty: " . var_export($m->isEmpty(), true) . "\n";
echo "capacity: " . $m->capacity() . "\n";

// Clear an already empty map
$m->clear();
echo "double clear: " . $m->count() . "\n";

// Can add after clear
$m->put("x", 99);
echo "after re-add: " . $m->count() . "\n";
echo "value: " . $m->get("x") . "\n";
?>
--EXPECT--
before clear: 3
after clear: 0
is empty: true
capacity: 8
double clear: 0
after re-add: 1
value: 99
