--TEST--
Map: copy() shares data (COW), mutations are isolated
--FILE--
<?php
$a = new Ds\Map();
$a->put("x", 1);
$a->put("y", 2);

$b = $a->copy();
$b->put("z", 3);

echo "a count: " . $a->count() . "\n";
echo "b count: " . $b->count() . "\n";
echo "a has z: " . var_export($a->hasKey("z"), true) . "\n";
echo "b has z: " . var_export($b->hasKey("z"), true) . "\n";
?>
--EXPECT--
a count: 2
b count: 3
a has z: false
b has z: true
