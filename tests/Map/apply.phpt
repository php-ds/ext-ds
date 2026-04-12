--TEST--
Map::apply - transform values in-place via callback
--FILE--
<?php
// Basic apply: double all values
$m = new Ds\Map(["a" => 1, "b" => 2, "c" => 3]);
$m->apply(function($key, $value) {
    return $value * 2;
});
echo "doubled: " . $m->get("a") . "," . $m->get("b") . "," . $m->get("c") . "\n";

// Apply receives both key and value
$m = new Ds\Map(["x" => 10, "y" => 20]);
$m->apply(function($key, $value) {
    return $key . "=" . $value;
});
echo "concat: " . $m->get("x") . "," . $m->get("y") . "\n";

// Apply on empty map is a no-op
$m = new Ds\Map();
$m->apply(function($key, $value) {
    return $value;
});
echo "empty apply: " . $m->count() . "\n";

// Apply with type change
$m = new Ds\Map(["a" => 1, "b" => 2]);
$m->apply(function($key, $value) {
    return (string)$value;
});
echo "type: " . gettype($m->get("a")) . "\n";

// Apply does not change keys, only values
$m = new Ds\Map(["a" => 1, "b" => 2]);
$m->apply(function($key, $value) {
    return $value + 100;
});
echo "keys preserved: " . var_export($m->hasKey("a"), true) . "," . var_export($m->hasKey("b"), true) . "\n";
echo "values: " . $m->get("a") . "," . $m->get("b") . "\n";
?>
--EXPECT--
doubled: 2,4,6
concat: x=10,y=20
empty apply: 0
type: string
keys preserved: true,true
values: 101,102
