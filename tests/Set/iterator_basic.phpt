--TEST--
Set: basic foreach iteration
--FILE--
<?php
$s = new Ds\Set([10, 20, 30]);
$result = [];
foreach ($s as $value) {
    $result[] = $value;
}
echo implode(",", $result) . "\n";
echo "count after: " . $s->count() . "\n";
?>
--EXPECT--
10,20,30
count after: 3
