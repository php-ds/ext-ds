--TEST--
Map: nested foreach iteration
--FILE--
<?php
$m = new Ds\Map(["x" => 1, "y" => 2]);
$result = [];
foreach ($m as $k1 => $v1) {
    foreach ($m as $k2 => $v2) {
        $result[] = "$k1$k2";
    }
}
echo implode(",", $result) . "\n";
?>
--EXPECT--
xx,xy,yx,yy
