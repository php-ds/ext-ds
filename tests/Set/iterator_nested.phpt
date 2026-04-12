--TEST--
Set: nested foreach iteration
--FILE--
<?php
$s = new Ds\Set([1, 2]);
$result = [];
foreach ($s as $a) {
    foreach ($s as $b) {
        $result[] = "$a:$b";
    }
}
echo implode(",", $result) . "\n";
?>
--EXPECT--
1:1,1:2,2:1,2:2
