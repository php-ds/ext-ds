--TEST--
Map: dimension write ($map[key] = val) on clone does not affect original
--FILE--
<?php
$a = new Ds\Map();
$a->put("a", 1);
$a->put("b", 2);

$b = clone $a;

// Direct dimension write
$b["a"] = 99;
$b["c"] = 3;

echo "a[a]: " . $a->get("a") . "\n";
echo "a count: " . $a->count() . "\n";
echo "b[a]: " . $b->get("a") . "\n";
echo "b count: " . $b->count() . "\n";
?>
--EXPECT--
a[a]: 1
a count: 2
b[a]: 99
b count: 3
