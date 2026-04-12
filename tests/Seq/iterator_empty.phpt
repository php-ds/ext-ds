--TEST--
Seq: foreach on empty sequence
--FILE--
<?php
$s = new Ds\Seq();
$count = 0;
foreach ($s as $value) {
    $count++;
}
echo "iterations: $count\n";
?>
--EXPECT--
iterations: 0
