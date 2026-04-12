--TEST--
Map: put on clone does not affect original
--FILE--
<?php
$a = new Ds\Map();
$a->put("x", 1);

$b = clone $a;
$b->put("y", 2);
$b->put("z", 3);

echo "a count: " . $a->count() . "\n";
echo "b count: " . $b->count() . "\n";
echo "a has y: " . var_export($a->hasKey("y"), true) . "\n";
echo "b has y: " . var_export($b->hasKey("y"), true) . "\n";
?>
--EXPECT--
a count: 1
b count: 3
a has y: false
b has y: true
