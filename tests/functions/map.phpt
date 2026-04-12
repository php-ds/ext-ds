--TEST--
\Ds\map() functional constructor
--FILE--
<?php
// Empty
$m = \Ds\map();
echo "empty: " . $m->count() . "\n";
var_dump($m instanceof Ds\Map);

// From associative array (key => value)
$m = \Ds\map(["a" => 1, "b" => 2, "c" => 3]);
echo "count: " . $m->count() . "\n";
echo "a: " . $m->get("a") . "\n";
echo "b: " . $m->get("b") . "\n";
echo "c: " . $m->get("c") . "\n";

// From generator with keys
function gen() { yield "x" => 10; yield "y" => 20; }
$m = \Ds\map(gen());
echo "gen x: " . $m->get("x") . "\n";
echo "gen y: " . $m->get("y") . "\n";
?>
--EXPECT--
empty: 0
bool(true)
count: 3
a: 1
b: 2
c: 3
gen x: 10
gen y: 20
