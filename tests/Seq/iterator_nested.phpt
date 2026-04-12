--TEST--
Seq: nested foreach iteration
--FILE--
<?php
$s = new Ds\Seq([1, 2, 3]);
$result = [];
foreach ($s as $a) {
    foreach ($s as $b) {
        $result[] = "$a:$b";
    }
}
echo implode(",", $result) . "\n";
?>
--EXPECT--
1:1,1:2,1:3,2:1,2:2,2:3,3:1,3:2,3:3
