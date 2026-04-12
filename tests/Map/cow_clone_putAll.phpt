--TEST--
Map: putAll on clone does not affect original
--FILE--
<?php
$a = new Ds\Map();
$a->put("a", 1);

$b = clone $a;
$b->putAll(["b" => 2, "c" => 3]);

echo "a count: " . $a->count() . "\n";
echo "b count: " . $b->count() . "\n";
echo "a has b: " . var_export($a->hasKey("b"), true) . "\n";
?>
--EXPECT--
a count: 1
b count: 3
a has b: false
