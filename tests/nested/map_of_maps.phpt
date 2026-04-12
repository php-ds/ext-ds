--TEST--
Nested: Map containing Map values
--FILE--
<?php
$inner = new Ds\Map(["x" => 1, "y" => 2]);
$outer = new Ds\Map();
$outer->put("coords", $inner);

echo "outer count: " . $outer->count() . "\n";
echo "x: " . $outer->get("coords")->get("x") . "\n";
echo "y: " . $outer->get("coords")->get("y") . "\n";

// Clone outer — inner map reference is shared
$clone = clone $outer;
$clone->get("coords")->put("z", 3);

// Original inner map is also affected (shallow clone of outer)
echo "original has z: " . var_export($outer->get("coords")->hasKey("z"), true) . "\n";
?>
--EXPECT--
outer count: 1
x: 1
y: 2
original has z: true
