--TEST--
Seq: basic foreach iteration
--FILE--
<?php
$s = new Ds\Seq([10, 20, 30, 40, 50]);
$result = [];
foreach ($s as $value) {
    $result[] = $value;
}
echo implode(",", $result) . "\n";
echo "count after: " . $s->count() . "\n";
?>
--EXPECT--
10,20,30,40,50
count after: 5
