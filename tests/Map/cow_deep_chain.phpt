--TEST--
Map: deep clone chain — c = clone(clone(a)), all independent
--FILE--
<?php
$a = new Ds\Map(["x" => 1]);
$b = clone $a;
$c = clone $b;

$a->put("a", 10);
$b->put("b", 20);
$c->put("c", 30);

echo "a count: " . $a->count() . "\n";
echo "b count: " . $b->count() . "\n";
echo "c count: " . $c->count() . "\n";
echo "a has a: " . var_export($a->hasKey("a"), true) . "\n";
echo "b has b: " . var_export($b->hasKey("b"), true) . "\n";
echo "c has c: " . var_export($c->hasKey("c"), true) . "\n";
echo "a has b: " . var_export($a->hasKey("b"), true) . "\n";
echo "c has a: " . var_export($c->hasKey("a"), true) . "\n";
?>
--EXPECT--
a count: 2
b count: 2
c count: 2
a has a: true
b has b: true
c has c: true
a has b: false
c has a: false
