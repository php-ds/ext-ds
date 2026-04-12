--TEST--
Map: reverse on clone does not affect original
--FILE--
<?php
$a = new Ds\Map();
$a->put("a", 1);
$a->put("b", 2);
$a->put("c", 3);

$b = clone $a;
$b->reverse();

$aKeys = [];
foreach ($a as $k => $v) $aKeys[] = $k;
$bKeys = [];
foreach ($b as $k => $v) $bKeys[] = $k;

echo "a keys: " . implode(",", $aKeys) . "\n";
echo "b keys: " . implode(",", $bKeys) . "\n";
?>
--EXPECT--
a keys: a,b,c
b keys: c,b,a
