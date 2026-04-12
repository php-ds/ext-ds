--TEST--
Map: basic foreach iteration yields key => value
--FILE--
<?php
$m = new Ds\Map();
$m->put("a", 1);
$m->put("b", 2);
$m->put("c", 3);

$result = [];
foreach ($m as $key => $value) {
    $result[] = "$key=$value";
}
echo implode(",", $result) . "\n";
echo "count after: " . $m->count() . "\n";
?>
--EXPECT--
a=1,b=2,c=3
count after: 3
