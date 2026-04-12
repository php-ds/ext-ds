--TEST--
Map: serialize/unserialize round-trip preserves data
--FILE--
<?php
$m = new Ds\Map(["a" => 1, "b" => 2]);
$s = serialize($m);
$m2 = unserialize($s);

echo "count: " . $m2->count() . "\n";
echo "a: " . $m2->get("a") . "\n";
echo "b: " . $m2->get("b") . "\n";

// Independent from original
$m2->put("c", 3);
echo "original has c: " . var_export($m->hasKey("c"), true) . "\n";
echo "unserialized has c: " . var_export($m2->hasKey("c"), true) . "\n";
?>
--EXPECT--
count: 2
a: 1
b: 2
original has c: false
unserialized has c: true
