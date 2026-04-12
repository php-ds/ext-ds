--TEST--
Nested: Map with Set values
--FILE--
<?php
$m = new Ds\Map();
$m->put("fruits", new Ds\Set(["apple", "banana"]));
$m->put("vegs", new Ds\Set(["carrot", "pea"]));

echo "fruits count: " . $m->get("fruits")->count() . "\n";
echo "vegs count: " . $m->get("vegs")->count() . "\n";
echo "has apple: " . var_export($m->get("fruits")->contains("apple"), true) . "\n";
echo "has pea: " . var_export($m->get("vegs")->contains("pea"), true) . "\n";
?>
--EXPECT--
fruits count: 2
vegs count: 2
has apple: true
has pea: true
