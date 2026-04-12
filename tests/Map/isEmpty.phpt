--TEST--
Map::isEmpty - check if map has no pairs
--FILE--
<?php
$m = new Ds\Map();
echo "new: " . var_export($m->isEmpty(), true) . "\n";

$m->put("a", 1);
echo "after put: " . var_export($m->isEmpty(), true) . "\n";

$m->remove("a");
echo "after remove: " . var_export($m->isEmpty(), true) . "\n";

$m->put("x", 1);
$m->put("y", 2);
$m->clear();
echo "after clear: " . var_export($m->isEmpty(), true) . "\n";

// From non-empty construction
$m2 = new Ds\Map(["a" => 1]);
echo "from array: " . var_export($m2->isEmpty(), true) . "\n";

// From empty array
$m3 = new Ds\Map([]);
echo "from empty array: " . var_export($m3->isEmpty(), true) . "\n";
?>
--EXPECT--
new: true
after put: false
after remove: true
after clear: true
from array: false
from empty array: true
