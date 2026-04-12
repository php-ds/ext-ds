--TEST--
Map: put during iteration does not affect iterator
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
        $map->put("d", 4);
        $map->put("a", 99);
    }
}

echo "Iterator saw: " . implode(",", $keys) . "\n";
echo "Map count: " . $map->count() . "\n";
echo "Map[a]: " . $map->get("a") . "\n";
echo "Map[d]: " . $map->get("d") . "\n";
?>
--EXPECT--
Iterator saw: a,b,c
Map count: 4
Map[a]: 99
Map[d]: 4
