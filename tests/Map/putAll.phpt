--TEST--
Map::putAll - add multiple pairs from iterable
--FILE--
<?php
$m = new Ds\Map();

// From associative array
$m->putAll(["a" => 1, "b" => 2, "c" => 3]);
echo "from array: " . $m->count() . "\n";
echo "a=" . $m->get("a") . " b=" . $m->get("b") . " c=" . $m->get("c") . "\n";

// Overwrites existing keys
$m->putAll(["b" => 20, "d" => 4]);
echo "after overwrite: " . $m->count() . "\n";
echo "b=" . $m->get("b") . " d=" . $m->get("d") . "\n";

// From iterator
$m2 = new Ds\Map();
$m2->putAll(new ArrayIterator(["x" => 10, "y" => 20]));
echo "from iterator: " . $m2->count() . "\n";
echo "x=" . $m2->get("x") . "\n";

// From generator
function gen() {
    yield "foo" => 100;
    yield "bar" => 200;
}
$m3 = new Ds\Map();
$m3->putAll(gen());
echo "from generator: " . $m3->count() . "\n";
echo "foo=" . $m3->get("foo") . "\n";

// From another Map
$source = new Ds\Map(["p" => 1, "q" => 2]);
$m4 = new Ds\Map();
$m4->putAll($source);
echo "from map: " . $m4->count() . "\n";

// Empty array is valid
$m5 = new Ds\Map();
$m5->putAll([]);
echo "empty array: " . $m5->count() . "\n";

// Integer-keyed array
$m6 = new Ds\Map();
$m6->putAll([10 => "a", 20 => "b"]);
echo "int keys: " . $m6->get(10) . "," . $m6->get(20) . "\n";
?>
--EXPECT--
from array: 3
a=1 b=2 c=3
after overwrite: 4
b=20 d=4
from iterator: 2
x=10
from generator: 2
foo=100
from map: 2
empty array: 0
int keys: a,b
