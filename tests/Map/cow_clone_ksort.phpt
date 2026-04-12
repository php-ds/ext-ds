--TEST--
Map: ksort on clone does not affect original
--FILE--
<?php
$a = new Ds\Map();
$a->put("c", 3);
$a->put("a", 1);
$a->put("b", 2);

$b = clone $a;
$b->ksort();

$aKeys = [];
foreach ($a as $k => $v) $aKeys[] = $k;
$bKeys = [];
foreach ($b as $k => $v) $bKeys[] = $k;

echo "a keys: " . implode(",", $aKeys) . "\n";
echo "b keys: " . implode(",", $bKeys) . "\n";
?>
--EXPECT--
a keys: c,a,b
b keys: a,b,c
