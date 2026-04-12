--TEST--
Map::count - count pairs in map
--FILE--
<?php
$m = new Ds\Map();
echo "empty: " . $m->count() . "\n";
echo "count(): " . count($m) . "\n";

$m->put("a", 1);
echo "after 1 put: " . $m->count() . "\n";

$m->put("b", 2);
$m->put("c", 3);
echo "after 3 puts: " . $m->count() . "\n";

// Overwriting a key doesn't increase count
$m->put("a", 100);
echo "after overwrite: " . $m->count() . "\n";

// Remove decreases count
$m->remove("a");
echo "after remove: " . $m->count() . "\n";

// count() function works too
echo "count function: " . count($m) . "\n";

// Clear resets count
$m->clear();
echo "after clear: " . $m->count() . "\n";
?>
--EXPECT--
empty: 0
count(): 0
after 1 put: 1
after 3 puts: 3
after overwrite: 3
after remove: 2
count function: 2
after clear: 0
