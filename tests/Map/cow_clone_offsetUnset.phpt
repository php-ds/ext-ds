--TEST--
Map: offsetUnset on clone does not affect original
--FILE--
<?php
$a = new Ds\Map();
$a->put("a", 1);
$a->put("b", 2);

$b = clone $a;
unset($b["a"]);

echo "a count: " . $a->count() . "\n";
echo "a has a: " . var_export($a->hasKey("a"), true) . "\n";
echo "b count: " . $b->count() . "\n";
echo "b has a: " . var_export($b->hasKey("a"), true) . "\n";
?>
--EXPECT--
a count: 2
a has a: true
b count: 1
b has a: false
