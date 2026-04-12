--TEST--
Map: remove during iteration does not affect iterator
--FILE--
<?php
$map = new Ds\Map();
$map->put("a", 1);
$map->put("b", 2);
$map->put("c", 3);

$keys = [];
foreach ($map as $k => $v) {
    $keys[] = $k;
    if ($k === "a") {
        $map->remove("b");
        $map->remove("c");
    }
}

echo "Iterator saw: " . implode(",", $keys) . "\n";
echo "Map count: " . $map->count() . "\n";
?>
--EXPECT--
Iterator saw: a,b,c
Map count: 1
