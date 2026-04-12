--TEST--
Seq: multiple sequential iterations yield same results
--FILE--
<?php
$s = new Ds\Seq(["a", "b", "c"]);

$r1 = [];
foreach ($s as $v) $r1[] = $v;

$r2 = [];
foreach ($s as $v) $r2[] = $v;

echo implode(",", $r1) . "\n";
echo implode(",", $r2) . "\n";
echo ($r1 === $r2 ? "same" : "different") . "\n";
?>
--EXPECT--
a,b,c
a,b,c
same
